/*
Full-screen page interface and bounded navigation stack.
Routes buttons, renders pages and returns home after inactivity.
*/
#include <stm32f4xx_hal.h>

#include "screen.h"
#include "ui.h"
#include "lowPower.h"

//Keeps home at stack index zero; all page pointers remain externally owned.
Ui::Ui(ButtonPanel &button_panel, Page &home_page)
    : buttons(button_panel), stack(), page_count(1), last_press(0)
{
    stack[0] = &home_page;
}

//Pushes a page or replaces the top at capacity, then initialises and renders it.
void Ui::show(Page &page)
{
    if (page_count < MAX_DEPTH) ++page_count;
    stack[page_count - 1] = &page;
    page.on_enter();
    render();
}

//Pops only above home and redraws without reinitialising the revealed page.
void Ui::back(void)
{
    if (page_count > 1) --page_count;
    render();
}

//Drops the visible stack to home without invoking editor confirmation.
void Ui::go_home(void)
{
    page_count = 1;
    render();
}

//Repeats the current page draw for each u8g buffer pass needed by the 128x64 display.
void Ui::render(void)
{
    Page *current = stack[page_count - 1];
    u8g_FirstPage(&screen.u8g);
    do {
        current->draw(&screen.u8g);
    } while (u8g_NextPage(&screen.u8g));
}

//Polls buttons while waiting with unsigned HAL ticks and a 10 ms idle delay.
void Ui::run_for(uint32_t duration_ms, bool allow_stop)
{
    const uint32_t start = HAL_GetTick();
    while (HAL_GetTick() - start < duration_ms) {
        ButtonId id;
        if (buttons.poll(id)) {
            last_press = HAL_GetTick();
            stack[page_count - 1]->on_button(*this, id);
            render();
        } else {
            //After 10 s idle, discards navigation and unconfirmed edits by returning home.
            if (page_count > 1 && HAL_GetTick() - last_press >= IDLE_TIMEOUT_MS) go_home();
            if (allow_stop && page_count == 1) {
                //Home screen idle: STOP mode until the end of the period or a button press.
                const uint32_t elapsed = HAL_GetTick() - start;
                if (elapsed < duration_ms) lowPower.stop_for(duration_ms - elapsed, buttons);
            } else {
                //Sleep until the next interrupt: a button EXTI (external interrupt)wakes immediately, SysTick every 1 ms.
                __WFI();
            }
        }
    }
}
