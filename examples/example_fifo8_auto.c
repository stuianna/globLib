//Change to include appropriate MCU target
#include <stm32f103cb_core.h>
#include "fifo8.h"

//Create an instance for the FIFO object
FIFO8 tx;

//Function to reload the FIFO
void loadtx(uint8_t byte);

//FIFO memory buffer must be a log2 based number
#define TX_FIFO_SIZE 32
uint8_t tx_buffer[TX_FIFO_SIZE];

//Overriden USART ISR functions - used to trigger FIFOs
void dataAvailable(uint8_t byte);   //Called when a byte is ready to be read, used for echo
void dataSent(void);                //Called when a byte is finished being sent, used to triger FIFO

int main(void)
{

	//USART Hardware setup
	USART_setup(USART_1); 	
	USART_setRxISR(USART_1,&dataAvailable);
	USART_setTxISR(USART_1,&dataSent);

	//Setup the fifo buffer 
	FIFO8_init(&tx,FIFO8_AUTO,tx_buffer,TX_FIFO_SIZE,USART_add_put(USART_1));

	while(1){

        //Bytes are now sent using FIF08_put
        //Bytes are automatically sent when the port is free
        FIFO8_put('s');

        //Wait
        for(int i = 0;i < 0xFFF;i ++);
	}

	return 0;
}

void dataAvailable(uint8_t byte){

    //Echo bytes
    FIFO8_put(byte);
}

void dataSent(void){

    //Get another byte from the FIFO
	FIFO8_get(&tx);
}

