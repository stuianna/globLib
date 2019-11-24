#include "stm32f76xxx_core.h"
#include "fifo.hpp"
#include <printf.h>

void delayMs(uint16_t ms);
void byteSent(void);

struct holder{
	float num2 = 23.3;
	char nl = '\n';
};

struct holder buffer[8];
uint8_t buffer_usart[128];

void fifoOut(struct holder);
void fifoOut_usart(uint8_t);

cFIFO<struct holder> fifo(buffer,cFIFO<struct holder>::FIFO_MODE_TRIGGER,8,fifoOut);
cFIFO<uint8_t> fifo_usart(buffer_usart,cFIFO<uint8_t>::FIFO_MODE_AUTO,128,fifoOut_usart);

int main(void){

	USART_setup(USART_3C);
	USART_setTxISR(USART3,byteSent);
	struct holder item1;
	struct holder item2;
	struct holder item3;
	struct holder item4;

	while(1){
		fifo.put(item1);
		fifo.put(item2);
		fifo.put(item3);
		fifo.put(item4);
		fifo.flush();
		printf("OK\n\n");
		delayMs(500);
	}
}

void fifoOut(struct holder t){

	printf("%.2f",t.num2);
	printf("%c",t.nl);
}

void _putchar(char c){

	fifo_usart.put(c);
}

void byteSent(void){

	fifo_usart.get();
}

void fifoOut_usart(uint8_t c){

	USART_put(USART3,c);
}

// Generic delay function.
void delayMs(uint16_t ms){

    for(uint16_t i = 0; i < ms; i++){
        for(uint16_t j = 0; j < 0x6FF; j++){
            __ASM("NOP");
        }
    }
}
