
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_cortex.h"
#include "stm32f4xx_hal_rtc.h"

#include <time.h> 
#include <diag/Trace.h>
#include "systemClock.h"
#include "types.h"

using namespace std;

SystemClock systemClock;

SystemClock::SystemClock() {
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;

    // Enable Power Control clock
    __PWR_CLK_ENABLE();

    //The backup domain (RTC, backup registers) is left untouched so the date
    //survives resets; Rtc::init() configures it only when it was lost.

    //The voltage scaling allows optimizing the power consumption when the
    //device is clocked below the maximum system frequency, to update the
    //voltage scaling value regarding system frequency refer to product
    //datasheet.
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);


    //Enable HSE Oscillator and activate PLL with HSE as source
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

    //This assumes the HSE_VALUE is a multiple of 1MHz. If this is not
    //your case, you have to recompute these PLL constants.
    RCC_OscInitStruct.PLL.PLLM = (HSE_VALUE / 1000000u);
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    // Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
    // clocks dividers
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK
            | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

    // Handle used by setDate/setTime/getUnixTimestamp; the RTC itself is set up by Rtc::init().
    RtcHandle.Instance = RTC;
    RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;
}


void SystemClock::setDate(uint8_t day, uint8_t month, uint8_t years) {
    dateStruct.WeekDay = RTC_WEEKDAY_MONDAY;
    dateStruct.Month = month;
    dateStruct.Date = day;
    dateStruct.Year = years;
    HAL_RTC_SetDate(&RtcHandle, &dateStruct, FORMAT_BIN);
}

void SystemClock::setTime(uint8_t hours, uint8_t minute, uint8_t seconds) {
    timeStruct.Hours = hours;
    timeStruct.Minutes = minute;
    timeStruct.Seconds = seconds;
    timeStruct.TimeFormat = RTC_HOURFORMAT12_PM;
    timeStruct.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    timeStruct.StoreOperation = RTC_STOREOPERATION_RESET;
    HAL_RTC_SetTime(&RtcHandle, &timeStruct, FORMAT_BIN);
}

RTC_TimeTypeDef SystemClock::getTime() {
    HAL_RTC_GetTime(&RtcHandle, &timeStruct, FORMAT_BIN);
    return timeStruct;
}

//get time must be done before getDate

RTC_DateTypeDef SystemClock::getDate() {
    HAL_RTC_GetDate(&RtcHandle, &dateStruct, FORMAT_BIN);

    return dateStruct;
}

time_t SystemClock::getUnixTimestamp() {

    HAL_RTC_GetTime(&RtcHandle, &timeStruct, FORMAT_BIN);
    HAL_RTC_GetDate(&RtcHandle, &dateStruct, FORMAT_BIN);
    
    struct tm timeinfo = {};

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
