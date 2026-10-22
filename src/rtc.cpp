/*
STM32 RTC calendar and backup-domain markers.
Calendar values and epoch timestamps use UTC without timezone adjustment.
*/
#include <assert.h>
#include <time.h> 
#include <diag/Trace.h>

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rtc.h"
#include "stm32f4xx_hal_rtc_ex.h"

#include "rtc.h"
#include "types.h"

Rtc rtc;

//Configures dividers for the external LSE clock and 24-hour calendar representation.
Rtc::Rtc(void)
{
    handle.Init.HourFormat = RTC_HOURFORMAT_24;
    handle.Init.AsynchPrediv = 127;
    handle.Init.SynchPrediv = (LSE_VALUE / 128) - 1;
    handle.Init.OutPut = RTC_OUTPUT_DISABLE;
    handle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    handle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;

    handle.Instance = RTC;
}

//Preserves a marked backup calendar or resets it and starts the external LSE clock.
bool Rtc::init(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RTC_DateTypeDef dateStruct;
    RTC_TimeTypeDef timeStruct;

    //Enable access to Backup domain
    HAL_PWR_EnableBkUpAccess();

    //DR0 magic marks an initialised RTC; preserve its calendar across MCU resets.
    if(HAL_RTCEx_BKUPRead(&handle, RTC_BKP_DR0) == RTC_MAGIC) {
	trace_printf("RTC already initialized, leaving it alone.\n");
	//After a reset the calendar shadow registers must resync before reading.
	__HAL_RTC_WRITEPROTECTION_DISABLE(&handle);
	HAL_RTC_WaitForSynchro(&handle);
	__HAL_RTC_WRITEPROTECTION_ENABLE(&handle);
	return false;
    }
    trace_printf("Resetting RTC.\n");

    //Reset Backup domain
    __HAL_RCC_BACKUPRESET_FORCE();
    __HAL_RCC_BACKUPRESET_RELEASE();

    //Enable LSE Oscillator
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE; //Mandatory, otherwise the PLL is reconfigured!
    RCC_OscInitStruct.LSEState = RCC_LSE_ON; //External 32.768 kHz clock on OSC_IN/OSC_OUT
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) == HAL_OK) {
        //Connect LSE to RTC
        __HAL_RCC_RTC_CLKPRESCALER(RCC_RTCCLKSOURCE_LSE);
        __HAL_RCC_RTC_CONFIG(RCC_RTCCLKSOURCE_LSE);
    }

    //Enable RTC
    __HAL_RCC_RTC_ENABLE();
    trace_printf("DR0 = %x %x\n", HAL_RTCEx_BKUPRead(&handle, RTC_BKP_DR0), RTC_MAGIC);

    if (HAL_RTC_Init(&handle) != HAL_OK) {
        trace_printf("RTC error: RTC initialization failed.\n");
	assert(0);
    }

    dateStruct.WeekDay = RTC_WEEKDAY_MONDAY;
    dateStruct.Month = RTC_MONTH_JANUARY;
    dateStruct.Date = 1;
    dateStruct.Year = 15;

    timeStruct.Hours = 0;
    timeStruct.Minutes = 0;
    timeStruct.Seconds = 0;
    timeStruct.TimeFormat = RTC_HOURFORMAT12_PM;
    timeStruct.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    timeStruct.StoreOperation = RTC_STOREOPERATION_RESET;

    HAL_RTC_SetDate(&handle, &dateStruct, FORMAT_BIN);
    HAL_RTC_SetTime(&handle, &timeStruct, FORMAT_BIN);

    //A reset calendar is unconfirmed in DR1; DR0 then marks hardware initialisation.
    HAL_RTCEx_BKUPWrite(&handle, RTC_BKP_DR1, 0);
    HAL_RTCEx_BKUPWrite(&handle, RTC_BKP_DR0, RTC_MAGIC);

    return true;
}

//Reads time before date to unlock shadow registers, then converts the UTC calendar to epoch seconds.
time_t Rtc::now(void)
{
    RTC_DateTypeDef dateStruct;
    RTC_TimeTypeDef timeStruct;
    struct tm timeinfo = {};

    HAL_RTC_GetTime(&handle, &timeStruct, FORMAT_BIN);
    HAL_RTC_GetDate(&handle, &dateStruct, FORMAT_BIN);

    //Setup a tm structure based on the RTC
    timeinfo.tm_wday = dateStruct.WeekDay;
    timeinfo.tm_mon = dateStruct.Month - 1;
    timeinfo.tm_mday = dateStruct.Date;
    timeinfo.tm_year = dateStruct.Year + 100;
    timeinfo.tm_hour = timeStruct.Hours;
    timeinfo.tm_min = timeStruct.Minutes;
    timeinfo.tm_sec = timeStruct.Seconds;

    return mktime(&timeinfo);
}

//Converts UTC epoch seconds to the calendar and records whether a user confirmed the time.
void Rtc::set(time_t t, bool confirmed)
{
    struct tm tm_utc;
    gmtime_r(&t, &tm_utc);

    RTC_DateTypeDef dateStruct;
    dateStruct.WeekDay = tm_utc.tm_wday == 0 ? RTC_WEEKDAY_SUNDAY : tm_utc.tm_wday;
    dateStruct.Month = tm_utc.tm_mon + 1;
    dateStruct.Date = tm_utc.tm_mday;
    dateStruct.Year = tm_utc.tm_year - 100;

    RTC_TimeTypeDef timeStruct = {};
    timeStruct.Hours = tm_utc.tm_hour;
    timeStruct.Minutes = tm_utc.tm_min;
    timeStruct.Seconds = tm_utc.tm_sec;
    timeStruct.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    timeStruct.StoreOperation = RTC_STOREOPERATION_RESET;

    HAL_RTC_SetTime(&handle, &timeStruct, FORMAT_BIN);
    HAL_RTC_SetDate(&handle, &dateStruct, FORMAT_BIN);
    //DR1 distinguishes user-confirmed time from an estimate restored from saved tissues.
    HAL_RTCEx_BKUPWrite(&handle, RTC_BKP_DR1, confirmed ? TIME_CONFIRMED : 0);
}

bool Rtc::needs_setting(void)
{
    return HAL_RTCEx_BKUPRead(&handle, RTC_BKP_DR1) != TIME_CONFIRMED;
}

u32 Rtc::ms_of_day(void)
{
    RTC_TimeTypeDef timeStruct;
    RTC_DateTypeDef dateStruct;
    HAL_RTC_GetTime(&handle, &timeStruct, FORMAT_BIN);
    //Reading the date releases the shadow register lock taken by GetTime.
    HAL_RTC_GetDate(&handle, &dateStruct, FORMAT_BIN);

    //SubSeconds counts down from PREDIV_S to 0 within each second.
    const u32 prediv_s = handle.Init.SynchPrediv;
    const u32 sub_ms = (prediv_s - timeStruct.SubSeconds) * 1000u / (prediv_s + 1u);
    return ((timeStruct.Hours * 60u + timeStruct.Minutes) * 60u + timeStruct.Seconds) * 1000u + sub_ms;
}

void Rtc::start_wakeup(u32 ms)
{
    //LSE/16 = 2048 Hz; the 16-bit counter fires after counter + 1 periods.
    u32 counter = ms * 2048u / 1000u;
    if (counter < 1u) counter = 1u;
    if (counter > 0x10000u) counter = 0x10000u;
    HAL_RTCEx_SetWakeUpTimer_IT(&handle, counter - 1u, RTC_WAKEUPCLOCK_RTCCLK_DIV16);
    HAL_NVIC_SetPriority(RTC_WKUP_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);
}

void Rtc::stop_wakeup(void)
{
    HAL_RTCEx_DeactivateWakeUpTimer(&handle);
    __HAL_RTC_WAKEUPTIMER_EXTI_CLEAR_FLAG();
}

void Rtc::resync(void)
{
    __HAL_RTC_WRITEPROTECTION_DISABLE(&handle);
    HAL_RTC_WaitForSynchro(&handle);
    __HAL_RTC_WRITEPROTECTION_ENABLE(&handle);
}

void Rtc::wakeup_irq(void)
{
    HAL_RTCEx_WakeUpTimerIRQHandler(&handle);
}

extern "C" void RTC_WKUP_IRQHandler(void);

extern "C" void RTC_WKUP_IRQHandler(void)
{
    rtc.wakeup_irq();
}
