#include "stm32f76xxx_core.h"

#define LED_PORT    GPIOB
#define LED_PIN     0
#define BUTTON_PORT GPIOC
#define BUTTON_PIN  13

void handler(void);

int main(void){


    // Set the LED port and pin as output.
    GPIO_pinSetup(GPIO_DO,LED_PORT,LED_PIN);

    // Set the button port and pin for rising edge interrupts.
    // Funtion 'handler' is called on interrupt event.
    GPIO_ISREnable(BUTTON_PORT,BUTTON_PIN,GPIO_RISING,handler);

    // Nothing else to do.
    while(1);
}

// ISR handler for rising edge event.
void handler(void){

    // Toggle the LED pin.
    GPIO_pinToggle(LED_PORT,LED_PIN);
}
