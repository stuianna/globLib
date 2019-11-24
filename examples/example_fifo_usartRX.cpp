#include "stm32f76xxx_core.h"
#include "fifo.hpp"

void delayMs(uint16_t ms);
void byteReceived(uint8_t b);

// Assign 16 bytes for the FIFO memeory buffer.
uint8_t buffer[16];

// Function called by FIFO output (get)
void fifoOut(uint8_t);

// Create the fifo object globally so it can be placed in the ISR
cFIFO<uint8_t> fifo(buffer,cFIFO<uint8_t>::FIFO_MODE_TRIGGER,8,fifoOut);

int main(void){

	// USART setup
	USART_setup(USART_3C);
	USART_setRxISR(USART3,byteReceived);

	while(1){

		if(fifo.size() > 3){
			fifo.flush();
		}
		delayMs(500);
	}
}

// Triggered by USART RX ISR
// Add the byte to the buffer
void byteReceived(uint8_t b){

	fifo.put(b);
}

// Called when fifo.get() is called.
void fifoOut(uint8_t t){

	USART_put(USART3,t);
}

// Generic delay function.
void delayMs(uint16_t ms){

    for(uint16_t i = 0; i < ms; i++){
        for(uint16_t j = 0; j < 0x6FF; j++){
            __ASM("NOP");
        }
    }
}
