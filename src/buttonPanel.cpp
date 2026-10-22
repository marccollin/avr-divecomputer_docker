#include <stm32f4xx_hal.h>
#include <diag/Trace.h>

#include "button.h"
#include "buttonPanel.h"

ButtonPanel buttonPanel;

static_assert(NB_BUTTONS == 3, "ButtonId must match the number of buttons");

void ButtonPanel::init(void)
{
    for (unsigned i = 0; i < NB_BUTTONS; ++i) {
        buttons[i]->init();
    }
    HAL_NVIC_SetPriority(EXTI1_IRQn, 5, 0);
    HAL_NVIC_SetPriority(EXTI3_IRQn, 5, 0);
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

bool ButtonPanel::poll(ButtonId &id)
{
    for (unsigned i = 0; i < NB_BUTTONS; ++i) {
        if (buttons[i]->take_press()) {
            trace_printf("Button %u pressed\n", i + 1);
            id = static_cast<ButtonId>(i);
            return true;
        }
    }
    return false;
}
