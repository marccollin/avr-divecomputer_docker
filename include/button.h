#ifndef BUTTON_H
#define BUTTON_H

#include <stm32f4xx_hal.h>

#include "pins.h"

class Button
{
public:
	Button(const pin_t &button_pin);
	void init(void);
	inline bool is_pressed(void)
	{
		return HAL_GPIO_ReadPin(pin.port, pin.number) == GPIO_PIN_RESET;
	}
	void change_event(void);
	bool take_press(void);
	//Returns true when a press is waiting, without consuming it.
	bool pending(void) const { return press_pending; }
protected:
	const pin_t &pin;
	volatile bool press_pending;
	uint32_t last_press_tick;
};

extern Button button1;
extern Button button2;
extern Button button3;

#define NB_BUTTONS 3
extern Button * const buttons[NB_BUTTONS];

#endif