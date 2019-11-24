#include "stm32f76xxx_core.h"
#include "fifo.hpp"

void delayMs(uint16_t ms);
void byteSent(void);

// Assign 16 bytes for the FIFO memeory buffer.
uint8_t buffer[16];

// Function called by FIFO output (get)
void fifoOut(uint8_t);

// Create the fifo object globally so it can be placed in the ISR
cFIFO<uint8_t> fifo(buffer,cFIFO<uint8_t>::FIFO_MODE_AUTO,8,fifoOut);

int main(void){

	// USART setup
	USART_setup(USART_3C);
	USART_setTxISR(USART3,byteSent);

	while(1){

		// Put some bytes and the FIFO size
		fifo.put('a');
		fifo.put('b');
		fifo.put('c');
		fifo.put(fifo.size()+48);
		fifo.put('\n');

		delayMs(200);
	}
}

// Triggered by USART TX ISR
// Get another byte from FIFO
void byteSent(void){

	fifo.get();
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
