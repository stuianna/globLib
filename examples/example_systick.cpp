#include "stm32f76xxx_core.h"

 /*
	* This example uses systick to toggle an LED on GPIO output
	* The timout value is modified periodically using a NOP delay
	*/

#define LED_PORT    GPIOB
#define LED_PIN     0

void delayMs(uint16_t ms);
void handler(void);

int main(void){

	GPIO_pinSetup(GPIO_DO,LED_PORT,LED_PIN);

	// Setup systick for 500ms trigger on handler
	SYSTICK_setup(200000,handler);

	// Nothing to do
	while(1){
		delayMs(2000);
		SYSTICK_stop();
		SYSTICK_timeout(50000);
		SYSTICK_start();
		delayMs(2000);
		SYSTICK_timeout(200000);
	}
}

void handler(void){

	GPIO_pinToggle(LED_PORT,LED_PIN);
}

// Generic delay function.
void delayMs(uint16_t ms){

    for(uint16_t i = 0; i < ms; i++){
        for(uint16_t j = 0; j < 0x6FF; j++){
            __ASM("NOP");
        }
    }
}
