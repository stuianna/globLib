#include "stm32f76xxx_core.h"

void byteReceived(uint8_t byte);    // New RX ISR target

int main(void){

    USART_setup(USART_3C);
    USART_setRxISR(USART3,byteReceived);
    USART_setBaud(USART3,USART_BAUD_115200);
    USART_setStop(USART3,USART_STOP_TWO);
    USART_setParity(USART3,USART_EVEN);

    //Nothing more to do.
    while(1);
}

void byteReceived(uint8_t byte){

    USART_put(USART3,byte);
}


