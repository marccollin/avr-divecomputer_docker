/*
STOP mode for long idle periods at the surface.
SysTick, HCLK and peripheral clocks stop; RAM, registers, GPIO and the OLED keep their state.
*/
#ifndef LOWPOWER_H
#define LOWPOWER_H

#include "types.h"
#include "buttonPanel.h"

/*
Sleeps in STOP mode until the RTC wake-up timer or a button interrupt.
The caller must make sure no peripheral transfer (SPI/DMA, SD card) is in progress.
*/
class LowPower {
public:
    static const u32 MIN_STOP_MS = 50; //Shorter waits use plain WFI; restoring the PLL costs ~1 ms.

    /*
    Stops for up to ms (at most 32 s), or less if a button is pressed.
    Returns immediately if a press is already pending.
    Restores the 168 MHz clock and advances the HAL tick by the time spent stopped.
    */
    void stop_for(u32 ms, const ButtonPanel &buttons);
};

extern LowPower lowPower;

#endif
