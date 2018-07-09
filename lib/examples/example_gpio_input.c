//Change to include appropriate MCU target
#include "stm32f103c8_gpio.h"

int main(void){

    //Setup port C pin 13 for digital output
    pinSetup(GPIO_DO,PORTC,PIN13);

    //Setup port A pin 6 for digital input
    pinSetup(GPIO_DI,PORTA,PIN6);

	while(1){

        //Set output to pin input value (Inverse logic);
        pinWrite(PORTC,PIN13,pinRead(PORTA,PIN6));

        //Wait a bit
        for(int i = 0; i < 0x7FFF; i++);
	}

	return 0;
}

