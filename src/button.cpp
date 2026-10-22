#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_gpio.h>

#include "button.h"

extern "C" void EXTI1_IRQHandler(void);
extern "C" void EXTI3_IRQHandler(void);
extern "C" void EXTI9_5_IRQHandler(void);

Button button1(SW1_PIN);
Button button2(SW2_PIN);
Button button3(SW3_PIN);

//Ordered left to right as seen on the front panel (PCB places SW3 on the left).
Button *const buttons[] = {&button3, &button2, &button1};

Button::Button(const pin_t &button_pin)
	:pin(button_pin), press_pending(false), last_press_tick(0)
{
}

void Button::init(void)
{
	GPIO_InitTypeDef gpio = {};
	gpio.Pin = pin.number;
	//Active-low switches (see kicad/dc.sch): external 100k pull-up and RC
	//filter, the switch shorts to GND, so idle high and pressed low.
	gpio.Mode = GPIO_MODE_IT_FALLING;
	gpio.Speed = GPIO_SPEED_HIGH;
	gpio.Pull = GPIO_NOPULL;
	gpio.Alternate = pin.af;

	HAL_GPIO_Init(pin.port, &gpio);
	last_press_tick = HAL_GetTick() - 50u;
	__HAL_GPIO_EXTI_CLEAR_IT(pin.number);
}

void Button::change_event(void)
{
	const uint32_t now = HAL_GetTick();
	if (is_pressed() && now - last_press_tick >= 50u) {
		last_press_tick = now;
		press_pending = true;
	}
}

bool Button::take_press(void)
{
	//Atomically consume the flag without losing an interrupt arriving here.
	const uint32_t primask = __get_PRIMASK();
	__disable_irq();
	const bool pressed = press_pending;
	press_pending = false;
	__set_PRIMASK(primask);
	return pressed;
}

extern "C" void EXTI1_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
}

extern "C" void EXTI3_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
}

extern "C" void EXTI9_5_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
}

extern "C" void HAL_GPIO_EXTI_Callback(uint16_t pin)
{
	if (pin == SW1_PIN.number) button1.change_event();
	if (pin == SW2_PIN.number) button2.change_event();
	if (pin == SW3_PIN.number) button3.change_event();
}