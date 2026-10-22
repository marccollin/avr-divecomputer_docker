#ifndef RTC_H
#define RTC_H

#include <time.h>
#include <stm32f4xx_hal.h>

#include "types.h"

class Rtc
{
public:
    Rtc(void);
    //Returns true when the RTC had lost its date (backup domain was reset).
    bool init(void);
    time_t now(void);
    //confirmed=false marks an estimated time (e.g. restored from the SD card).
    void set(time_t t, bool confirmed = true);
    //True until the user sets the date/time after it was lost.
    bool needs_setting(void);
protected:
    const u32 RTC_MAGIC = 0x1badcafe;
    const u32 TIME_CONFIRMED = 0x600d;
    RTC_HandleTypeDef handle;
};

extern Rtc rtc;

#endif
