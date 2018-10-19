//Change to include appropriate MCU target
#include <stm32f103cb_core.h>
#include "fifo8.h"

//Create an instance for the USART object -> required for demo
USART serial;

//Create an instance for the FIFO object
FIFO8 tx;

//Function to reload the FIFO
void loadtx(uint8_t byte);

//FIFO memory buffer
#define TX_FIFO_SIZE 32
uint8_t tx_buffer[TX_FIFO_SIZE];

//Overriden USART ISR functions - used to trigger FIFOs
void dataAvailable(uint8_t byte);   //Called when a byte is ready to be read, used for echo
void dataSent(void);                //Called when a byte is finished being sent, used to triger FIFO
void loadtx(uint8_t byte);          //Used to reload FIFO

int main(void)
{

	//USART Hardware setup
	USART_setup(&serial,USART_1); 	
	USART_setRxISR(&serial,&dataAvailable);
	USART_setTxISR(&serial,&dataSent);

	//Setup the fifo buffer 
	FIFO8_init(&tx,FIFO8_AUTO,tx_buffer,TX_FIFO_SIZE,serial.put);

    //Retarget the put function to load the FIFO
    serial.put = &loadtx;

	while(1){

        //Serial.put now adds byte to the fifo
        //Bytes are automatically sent when the port is free
        serial.put('s');

        //Wait
        for(int i = 0;i < 0xFFF;i ++);
	}

	return 0;
}

void dataAvailable(uint8_t byte){

    //Echo bytes
    serial.put(byte);
}

void dataSent(void){

    //Get another byte from the FIFO
	FIFO8_get(&tx);
}

void loadtx(uint8_t byte){

    //Add a byte to the FIFO
	FIFO8_put(&tx,byte);
}
