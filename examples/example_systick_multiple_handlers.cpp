#include "stm32f76xxx_core.h"

#define LED_PORT    GPIOB
#define LED_PIN     0

void handler_one(void);
void handler_two(void);

/*
 * This example uses systick to toggle an output GPIO connected to LED.
 * Two handlers are assigned and alternate on each IRQ event.
 * */

int main(void){

	GPIO_pinSetup(GPIO_DO,LED_PORT,LED_PIN);

	// Setup systick for 500ms trigger on handler
	SYSTICK_setup(500000,handler_one);

	// Nothing to do
	while(1);
}

void handler_one(void){

	GPIO_pinHigh(LED_PORT,LED_PIN);
	SYSTICK_handler(handler_two);
}

void handler_two(void){

	GPIO_pinLow(LED_PORT,LED_PIN);
	SYSTICK_handler(handler_one);
}
