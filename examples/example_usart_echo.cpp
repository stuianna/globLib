#include "stm32f76xxx_core.h"

int main(void){

    // Set up USART, default settings, character echo.
    USART_setup(USART_3C);

    //Nothing more to do.
    while(1);
}


