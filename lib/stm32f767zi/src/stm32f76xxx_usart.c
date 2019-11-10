#include "stm32f76xxx_usart.h"

bool USART_setup(USART_Varient variant){

    USART_TypeDef *peripheral;

    // Map the variant to an actual port
    switch(variant){
        case USART_3A:  
            GPIO_pinSetup(GPIO_UART_RX,GPIOB,11);
            GPIO_pinSetup(GPIO_UART_TX,GPIOB,10);
            peripheral = USART3; 
            clockEnable(RCC_USART3);
            break;
        case USART_3B:  
            GPIO_pinSetup(GPIO_UART_RX,GPIOC,11);
            GPIO_pinSetup(GPIO_UART_TX,GPIOC,10);
            peripheral = USART3; 
            clockEnable(RCC_USART3);
            break;
        case USART_3C:  
            GPIO_pinSetup(GPIO_UART_RX,GPIOD,9);
            GPIO_pinSetup(GPIO_UART_TX,GPIOD,8);
            peripheral = USART3; 
            clockEnable(RCC_USART3);
            break;
        default:        
            return ERROR; 
            break;
    }

    USART_setData(peripheral,USART_DEFAULT_DATA);
    USART_setParity(peripheral,USART_DEFAULT_PARITY);
    USART_setStop(peripheral,USART_DEFAULT_STOP);
    USART_setBaud(peripheral,USART_DEFAULT_BAUD);

    peripheral->CR1 |= USART_CR1_UE;
    peripheral->CR1 |= USART_CR1_RE;
    peripheral->CR1 |= USART_CR1_TE;
    return SUCCESS;
}

void USART_put(USART_TypeDef *peripheral, uint8_t byte){

    // Wait for data to be sent.
    while(!(peripheral->ISR & USART_ISR_TXE));
    peripheral->TDR = byte;
}


uint8_t USART_get(USART_TypeDef *peripheral){

    // Wait for data to appear
    while(!(peripheral->ISR & USART_ISR_RXNE));
    return peripheral->RDR;
}

bool USART_setData(USART_TypeDef *peripheral,USART_Data data){

    bool enabled = peripheral->CR1 & USART_CR1_UE;
    peripheral->CR1 |= (data & 0x1) << 12;
    peripheral->CR1 |= ((data & 0x2) >> 1) << 28;
    peripheral->CR1 |= enabled ? USART_CR1_UE : 0;
    return SUCCESS;
}

bool  USART_setParity(USART_TypeDef *peripheral, USART_Parity parity){

    bool enabled = peripheral->CR1 & USART_CR1_UE;
    peripheral->CR1 |= (USART_CR1_PS & (parity & 0x1));
    peripheral->CR1 |= (USART_CR1_PCE & ((parity & 0x2) >> 1));
    peripheral->CR1 |= enabled ? USART_CR1_UE : 0;
    return SUCCESS;
}

bool USART_setStop(USART_TypeDef *peripheral, USART_Stop stop){

    bool enabled = peripheral->CR1 & USART_CR1_UE;
    peripheral->CR2 |= stop << USART_CR2_STOP_Pos;
    peripheral->CR1 |= enabled ? USART_CR1_UE : 0;
    return SUCCESS;

}

bool USART_setBaud(USART_TypeDef *peripheral, USART_Baud baud){

    bool enabled = peripheral->CR1 & USART_CR1_UE;
    peripheral->BRR = (uint32_t)(SystemCoreClock / baud);
    peripheral->CR1 |= enabled ? USART_CR1_UE : 0;
    return SUCCESS;
}

