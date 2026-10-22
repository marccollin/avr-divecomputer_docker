/*
STOP mode for long idle periods at the surface.
*/
#include <stm32f4xx_hal.h>

#include "lowPower.h"
#include "rtc.h"
#include "systemClock.h"
#include "timer.h"

LowPower lowPower;

static const u32 MS_PER_DAY = 86400000u;

void LowPower::stop_for(u32 ms, const ButtonPanel &buttons)
{
    if (ms < MIN_STOP_MS) {
        __WFI();
        return;
    }

    //Armed before masking interrupts: the HAL call relies on the tick for its timeout.
    rtc.start_wakeup(ms);
    const u32 before = rtc.ms_of_day();

    //With PRIMASK set, a press arriving now still wakes WFI, but its handler runs only
    //after the clock is restored, so the debounce sees a corrected tick.
    __disable_irq();
    const bool stop = !buttons.pending();
    if (stop) {
        HAL_SuspendTick();
        HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
        //Wakes on the 16 MHz HSI; HAL_RCC_ClockConfig() also restarts SysTick via HAL_InitTick().
        systemClock.configure();
    }
    __enable_irq();

    rtc.stop_wakeup();
    if (stop) {
        rtc.resync();
        //Modulo one day handles a stop that crosses midnight.
        tick_advance((rtc.ms_of_day() + MS_PER_DAY - before) % MS_PER_DAY);
    }
}
