//Change to include appropriate MCU target
#include <stm32f103cb_core.h>
#include "fifo8.h"

//Create an instance for the FIFO object
FIFO8 rx;

//Function to reload the FIFO
void loadtx(uint8_t byte);

//FIFO memory buffer
#define RX_FIFO_SIZE 8 
uint8_t rx_buffer[RX_FIFO_SIZE];

//Overriden USART ISR functions - used to trigger FIFOs
void dataAvailable(uint8_t byte);   //Called when a byte is ready to be read, used to load buffer
void getrx(uint8_t byte);           //This is called when bytes are read (dumped)

int main(void)
{

	//USART Hardware setup
	USART_setup(USART_1); 	
	USART_setRxISR(&serial,&dataAvailable);

	//Setup the fifo buffer 
	FIFO8_init(&rx,FIFO8_TRIGGER,rx_buffer,RX_FIFO_SIZE,&getrx);

	while(1){

        //Empty buffer to terminal if it has greater than four bytes
        if(FIFO8_size(&rx) >= 4){

            FIFO8_flush(&rx);
        }

	}

	return 0;
}

void dataAvailable(uint8_t byte){

    //Add the recieved to the FIFO
    FIFO8_put(&rx,byte);
}

void getrx(uint8_t byte){

    //Echo back to terminal
    USART_put(USART1,byte);
}

