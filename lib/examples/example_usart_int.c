//Change to include appropriate MCU target
#include "stm32f103c8_usart.h"

void dataAvailable(uint8_t byte);
void dataSent(void);

int main(void){

    //Setup port 1
	USART_setup(USART_1); 	

    //Retarget the ISRs to local functions
    USART_setRxISR(USART_1,&dataAvailable);
    USART_setTxISR(USART_1,&dataSent);

	while(1){

        //Nothing to do here
	}

	return 0;
}

//This is now called when a byte is available to be read
void dataAvailable(uint8_t byte){

    //Echo characters back to terminal
    USART_put(USART_1,byte);
}

//This is now called when a byte has finished being sent
void dataSent(void){

    //Nothing to do here
}
