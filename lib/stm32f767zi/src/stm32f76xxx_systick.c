#include "stm32f76xxx_systick.h"

// Estimate the ammount of ticks it takes for the IRQ to process the
// function pointer jump
#define SYSTICK_ISR_INST 10

void (*systick_handler)(void);

uint8_t SYSTICK_setup(uint32_t timeout, void (*handler)(void)){

	systick_handler = handler;
	int32_t reload = (timeout * (SystemCoreClock / 1000000)) - SYSTICK_ISR_INST;

	if((reload > 0xFFFFFF) || (reload < 0)){
		return ERROR;
	}

	NVIC_EnableIRQ(SysTick_IRQn);
	SysTick_Config(reload);
	return SUCCESS;
}

uint8_t SYSTICK_timeout(uint32_t timeout){

	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	int32_t reload = (timeout * (SystemCoreClock / 1000000)) - SYSTICK_ISR_INST;

	if((reload > 0xFFFFFF) || (reload < 0)){
		return ERROR;
	}
	SysTick->LOAD = (uint32_t)(reload - 1);
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	return SUCCESS;
}

uint32_t SYSTICK_count(void){

	return SysTick->VAL;
}

void SYSTICK_stop(void){

	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void SYSTICK_start(void){

	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

void SYSTICK_handler(void (*handler)(void)){

	systick_handler = handler;
}

void SysTick_Handler(void){

	systick_handler();
}
