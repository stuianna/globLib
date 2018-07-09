//Include a USART module to demonstrate an error message and error
#include "stm32f103c8_usart.h"

//Create a serial structure
USART serial;

int main(void){


    //Setup port 1
	USART_setup(&serial,USART_1); 	

    //Enable debug feature and set serial put as output stream
    MCU_debugEnable(serial.put);

    //GENERATE AN ERROR CONDITION, port "43" doesn't exist, this outputs an error
	USART_setup(&serial,43); 	
    

	while(1){


        //Print a random error to test functionallity
        MCU_printError(E_USART_NOBAUD); 	

        //Wait a bit for data to transmit
        for(int i = 0; i < 0x7FFFF; i++);
	}

	return 0;
}

