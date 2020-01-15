#include <stm32f76xxx_core.h>
#include <arm_math.h>

#define LED_PORT    GPIOB
#define LED_PIN     7 

void delayMs(uint16_t ms);

int main(void){

    GPIO_pinSetup(GPIO_DO,LED_PORT,LED_PIN);

    while(1){

        delayMs(500);
        GPIO_pinToggle(LED_PORT,LED_PIN);
    }
}

// Millisecond delay is an estimate
void delayMs(uint16_t ms){

    for(uint16_t i = 0; i < ms; i++){
        for(uint16_t j = 0; j < 0x1FFF; j++){
            __ASM("NOP");
        }
    }
}
