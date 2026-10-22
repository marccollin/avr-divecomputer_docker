#include <assert.h>
#include <string.h>
#include <diag/Trace.h>


#include <cortexm/ExceptionHandlers.h>
#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_rcc.h>

#include "timer.h"

TimerWheel main_timer;

TimerWheel::TimerWheel()
{
	current_slot = 0;
	memset(slots, 0, TIMER_WHEEL_SLOTS * sizeof(TimerItem*));
}

void TimerWheel::tick(void)
{
	current_slot = (current_slot + 1) % TIMER_WHEEL_SLOTS;

	TimerItem **prev = &slots[current_slot];
	TimerItem *next;
	TimerItem *resched = NULL;

	for(TimerItem *item = slots[current_slot]; item != NULL; item = next) {
		next = item->next;
		if(!item->rot--) {
			item->callback(item->arg);

			*prev = item->next;

			item->active = false;
			if(item->reload != 0) {
				item->next = resched;
				resched = item;
			}
		} else
			prev = &item->next;
	}

	for(TimerItem *item = resched; item != NULL; item = next) {
		next = item->next;
		schedule(item, item->reload, true);
	}
}

void TimerWheel::schedule(TimerItem *item, u32 ticks, bool reload)
{
	assert(item->active == false);
	assert(item->callback);
	assert(ticks > 0);

	item->rot = ticks / TIMER_WHEEL_SLOTS;
	item->reload = reload ? ticks : 0;

	NVIC_DisableIRQ(SysTick_IRQn);
	size_t slot_num = (ticks + current_slot) % TIMER_WHEEL_SLOTS;

	item->next = slots[slot_num];
	slots[slot_num] = item;
	item->active = true;

	NVIC_EnableIRQ(SysTick_IRQn);
}

bool TimerWheel::cancel(TimerItem *item)
{
	if(!item->active)
		return false;

	NVIC_DisableIRQ(SysTick_IRQn);

	for(size_t slot_num=0; slot_num < TIMER_WHEEL_SLOTS; slot_num++){
		TimerItem *slot = slots[slot_num];
		TimerItem *next;
		TimerItem **prev = &slots[slot_num];

		for(TimerItem *litem = slot; litem != NULL; litem = next) {
			next = litem->next;
			if(litem == item) {
				*prev = litem->next;
				litem->next = NULL;
				litem->active = false;
				NVIC_EnableIRQ(SysTick_IRQn);
				return true;
			} else
				prev = &litem->next;
		}
	}

	NVIC_EnableIRQ(SysTick_IRQn);
	return false;
}


//HAL tick in ms; replaces the HAL's private counter so tick_advance() can correct it after STOP mode.
static volatile uint32_t tick_ms;

void HAL_IncTick(void)
{
	tick_ms++;
}

uint32_t HAL_GetTick(void)
{
	return tick_ms;
}

//Also called by HAL_RCC_ClockConfig(), so every clock change keeps the same SysTick setup.
HAL_StatusTypeDef HAL_InitTick(uint32_t)
{
	systick_init();
	return HAL_OK;
}

void systick_init(void)
{
	SysTick_Config(HAL_RCC_GetHCLKFreq() / 8 / 1000);
	__HAL_CORTEX_SYSTICKCLK_CONFIG(SYSTICK_CLKSOURCE_HCLK_DIV8);
	NVIC_SetPriority(SysTick_IRQn, 0);
}

void tick_advance(u32 ms)
{
	const uint32_t primask = __get_PRIMASK();
	__disable_irq();
	tick_ms += ms;
	__set_PRIMASK(primask);
}

extern "C" void
SysTick_Handler(void)
{
	HAL_IncTick();
	main_timer.tick();
}

void HAL_Delay(__IO uint32_t Delay)
{
	uint32_t tickstart = 0;
	tickstart = HAL_GetTick();
	while((HAL_GetTick() - tickstart) < Delay)
		__WFI();
}
