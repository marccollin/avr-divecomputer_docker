#include <stm32f4xx_hal.h>

#include "screen.h"
#include "ui.h"

Ui::Ui(ButtonPanel &button_panel, Page &home_page)
    : buttons(button_panel), stack(), depth(1), last_press(0)
{
    stack[0] = &home_page;
}

void Ui::show(Page &page)
{
    if (depth < MAX_DEPTH) ++depth;
    stack[depth - 1] = &page;
    page.on_enter();
    render();
}

void Ui::back(void)
{
    if (depth > 1) --depth;
    render();
}

void Ui::go_home(void)
{
    depth = 1;
    render();
}

void Ui::render(void)
{
    Page *current = stack[depth - 1];
    u8g_FirstPage(&screen.u8g);
    do {
        current->draw(&screen.u8g);
    } while (u8g_NextPage(&screen.u8g));
}

void Ui::run_for(uint32_t duration_ms)
{
    const uint32_t start = HAL_GetTick();
    while (HAL_GetTick() - start < duration_ms) {
        ButtonId id;
        if (buttons.poll(id)) {
            last_press = HAL_GetTick();
            stack[depth - 1]->on_button(*this, id);
            render();
        } else {
            if (depth > 1 && HAL_GetTick() - last_press >= IDLE_TIMEOUT_MS) go_home();
            HAL_Delay(10);
        }
    }
}
