#include "stm32f76xxx_core.h"

int main(void){

    USART_setup(USART_3C);

    // Interrupts need to be disabled to use blocking mode.
    USART_disableISR(USART3);

    while(1){
        // Just echo characters
        USART_put(USART3,USART_get(USART3));
    }
}


