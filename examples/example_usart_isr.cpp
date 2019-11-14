#include "stm32f76xxx_core.h"

#define LED_PORT    GPIOB
#define LED_PIN     7 

void delayMs(uint16_t ms);
void byteReceived(uint8_t byte);    // New RX ISR target
void byteSent(void);                // New TX ISR target

int main(void){

    // GPIO setup for LED.
    GPIO_pinSetup(GPIO_DO,LED_PORT,LED_PIN);

    USART_setup(USART_3C);
    USART_setRxISR(USART3,byteReceived);
    USART_setTxISR(USART3,byteSent);

    //Nothing more to do.
    while(1);
}

void byteReceived(uint8_t byte){

    USART_put(USART3,byte);
}

void byteSent(void){

    // Flash LED to indicate TX complete
    GPIO_pinHigh(LED_PORT,LED_PIN);
    delayMs(1);             // Not good practice, but fine for example usage.
    GPIO_pinLow(LED_PORT,LED_PIN);
}

// Millisecond delay is an estimate
void delayMs(uint16_t ms){

    for(uint16_t i = 0; i < ms; i++){
        for(uint16_t j = 0; j < 0x1FFF; j++){
            __ASM("NOP");
        }
    }
}
