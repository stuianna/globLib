//Change to include appropriate MCU target
#include <stm32f103cb_gpio.h>

int main(void){

    //Setup port C pin 13 for digital output
    pinSetup(GPIO_DO,PORTC,PIN13);

	while(1){

        //Toggle the pin's output value
        pinToggle(PORTC,PIN13);

        //Wait a bit
        for(int i = 0; i < 0x7FFF; i++);
	}

	return 0;
}

