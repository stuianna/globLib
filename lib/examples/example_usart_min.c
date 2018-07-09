//Change to include appropriate MCU target
#include "stm32f103c8_usart.h"

int main(void){

    //Setup port 1
	USART_setup(USART_1); 	

    //ISR are enabled by default, they need to be disabled for blocking read
    USART_disableISR(USART_1,USART_TX);
    USART_disableISR(USART_1,USART_RX);

	while(1){

        //Echo characters back to the terminal
        USART_put(USART_1,USART_get(USART_1));
	}

	return 0;
}

