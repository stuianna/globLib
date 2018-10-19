//Change to include appropriate MCU target
#include <stm32f103cb_gpio.h>

//Function to call for interrupt event
void callme(void);

int main(void){

    //Setup port C pin 13 for digital output
    pinSetup(GPIO_DO,PORTC,PIN13);

    //Setup the ISR
    GPIO_ISREnable(PORTA, PIN6, GPIO_RISING, &callme);

	while(1){

        //Do nothing
	}

	return 0;
}

void callme(void){

        //Toggle the pin's output value
        pinToggle(PORTC,PIN13);
}

