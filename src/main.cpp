//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stm32f4xx_hal.h>
#include "stm32f4xx_hal_gpio.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <array>

#include "pressureSensor.h"
#include "rtc.h"
#include "sdcard.h"
#include "screen.h"
#include <time.h> 
#include "timer.h"
#include "types.h"
#include "pins.h"
#include "buhlmannModel.h"
#include "diveController.h"
#include "tissueStore.h"
#include "waterSensor.h"
#include "fileUtility.h"
#include "buttonPanel.h"
#include "mainPage.h"
#include "menu.h"
#include "demoPage.h"
#include "ui.h"
#include <diag/Trace.h>

static inline void enable_peripherals() {
    __GPIOA_CLK_ENABLE();
    __GPIOB_CLK_ENABLE();
    __GPIOC_CLK_ENABLE();
    __GPIOD_CLK_ENABLE();
    __GPIOE_CLK_ENABLE();
    __DMA1_CLK_ENABLE();
    __DMA1_CLK_SLEEP_ENABLE();
}

int main(void) {
    BuhlmannModel model;
    DiveController dive(model, tissueStore);
    MainPage mainPage(dive);
    MenuSystem menu(dive);
    mainPage.set_menu(menu.root());
    DemoPage demoPage;
    mainPage.set_demo(demoPage);
    Ui ui(buttonPanel, mainPage);
#ifdef DEBUG
    HAL_EnableDBGSleepMode();
    //Keeps the debugger connected in STOP mode (clocks stay on, so savings are lower).
    HAL_DBGMCU_EnableDBGStopMode();
#endif
    enable_peripherals();
    const bool clock_lost = rtc.init();

    systick_init();

    buttonPanel.init();

    trace_printf("System clock : %uHz\n", SystemCoreClock);
    screen.enable();
    pressureSensor.enable();

    trace_printf("Depth sensor ready to go.\n");

    //Date lost (power cut without VBAT): restore the last saved time from the SD card.
    //It stays flagged as unconfirmed until the user sets it from the menu.
    time_t saved;
    if (clock_lost && tissueStore.saved_time(saved)) {
        rtc.set(saved, false);
    }

    //Gases carried for the dive; a pre-dive menu will set these later.
    //Example: dive.gases().clear({0.21f, 0.0f}); dive.gases().add({0.50f, 0.0f});
    dive.gases().clear(GAS_AIR);

    pressureSensor.capture_data();
    dive.begin(pressureSensor.get_pressure_bar(), rtc.now());

    for (;;) {
        pressureSensor.capture_data();
        const float pressure = pressureSensor.get_pressure_bar();

        dive.sample(pressure, HAL_GetTick(), rtc.now());
        mainPage.update(pressure, pressureSensor.get_temperature_celcius(), rtc.now());
        ui.render();

        //1 s sampling in the water; 10 s at the surface, in STOP mode while the home page is shown.
        const bool surface = !dive.in_dive() && !waterSensor.is_wet();
        ui.run_for(surface ? 10000 : 1000, surface);
    }
}
