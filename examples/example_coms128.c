//Change to include appropriate MCU target
#include <stm32f103cb_core.h>
#include "coms128.h"

//Create an instance for the USART object -> required for demo
USART serial;

int main(void)
{

	//USART Hardware setup
	USART_setup(&serial,USART_1); 	
    COMS(serial);


    //Print to terminal
	while(1){

        printl("Some line of stuff.");       //Print with a line
        for(int i = 0; i < 0x2EEEE;i++);    //Wait
	}
    
	return 0;
}

