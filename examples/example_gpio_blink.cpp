#include "stm32f76xxx_core.h"

#define LED_PORT    GPIOB
#define LED_PIN     0

void delayMs(uint16_t ms);

int main(void){

    // Set the LED port and pin as output.
    GPIO_pinSetup(GPIO_DO,LED_PORT,LED_PIN);

    while(1){

        GPIO_pinToggle(LED_PORT,LED_PIN);
        delayMs(500);
    }
}

// Millisecond delay is an estimate
void delayMs(uint16_t ms){

    for(uint16_t i = 0; i < ms; i++){
        for(uint16_t j = 0; j < 0x6FF; j++){
            __ASM("NOP");
        }
    }
}
