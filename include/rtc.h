/*
STM32 RTC calendar and backup-domain markers.
Calendar values and epoch timestamps use UTC without timezone adjustment.
*/
#ifndef RTC_H
#define RTC_H

#include <time.h>
#include <stm32f4xx_hal.h>

#include "types.h"

/*
UTC calendar wrapper with backup-register persistence across resets.
DR0 marks RTC initialisation; DR1 marks time explicitly confirmed by the user.
*/
class Rtc
{
public:
    /*
    Prepares the 24-hour RTC handle and LSE divider; does not initialise hardware.
    */
    Rtc(void);
    /*
    Enables or resynchronises the RTC backup domain.
    Returns true if the calendar was lost and reset to the default unconfirmed date.
    */
    bool init(void);
    /*
    Returns UTC epoch seconds from the RTC calendar; no timezone or DST adjustment.
    Reads time before date to release the hardware calendar shadow lock.
    */
    time_t now(void);
    /*
    Sets the RTC from UTC epoch seconds t without timezone conversion.
    Confirmed true records user confirmation in DR1; false marks an estimated time.
    */
    void set(time_t t, bool confirmed = true);
    /*
    Returns true while DR1 lacks the user-confirmed marker, even if a date is present.
    */
    bool needs_setting(void);
    /*
    Returns the time of day in ms (0..86399999) including RTC sub-seconds.
    Used to measure time spent in STOP mode, when the HAL tick does not run.
    */
    u32 ms_of_day(void);
    /*
    Arms the wake-up timer to fire once after ms (1..32000) from the LSE/16 clock (2048 Hz).
    */
    void start_wakeup(u32 ms);
    //Disarms the wake-up timer.
    void stop_wakeup(void);
    /*
    Waits for the calendar shadow registers to resynchronise.
    Required after STOP mode before reading the time.
    */
    void resync(void);
    //Wake-up timer interrupt handler: clears the RTC and EXTI flags.
    void wakeup_irq(void);
protected:
    const u32 RTC_MAGIC = 0x1badcafe; //DR0 magic: backup-domain calendar is initialised.
    const u32 TIME_CONFIRMED = 0x600d; //DR1 magic: time was confirmed by the user.
    RTC_HandleTypeDef handle;
};

extern Rtc rtc;

#endif
