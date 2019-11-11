#include "stm32f76xxx_core.h"

#define LED_PORT    GPIOB
#define LED_PIN     0
#define BUTTON_PORT GPIOC
#define BUTTON_PIN  13

int main(void){

    // Set the LED port and pin as output.
    GPIO_pinSetup(GPIO_DO,LED_PORT,LED_PIN);

    // Set the button port and pin as input.
    GPIO_pinSetup(GPIO_DI,BUTTON_PORT,BUTTON_PIN);

    while(1){

        // Read the button pin, set the LED pin accordingly.
        if(GPIO_pinRead(BUTTON_PORT,BUTTON_PIN)){
            GPIO_pinHigh(LED_PORT,LED_PIN);
        }
        else{
            GPIO_pinLow(LED_PORT,LED_PIN);
        }
    }
}

