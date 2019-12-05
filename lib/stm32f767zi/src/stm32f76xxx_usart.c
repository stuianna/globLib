#include "stm32f76xxx_usart.h"

static void usart1_default_tx_isr(void);
static void usart1_default_rx_isr(uint8_t byte);
static void (*usart1_isr_tx)(void);
static void (*usart1_isr_rx)(uint8_t byte);

static void usart2_default_tx_isr(void);
static void usart2_default_rx_isr(uint8_t byte);
static void (*usart2_isr_tx)(void);
static void (*usart2_isr_rx)(uint8_t byte);

static void usart3_default_tx_isr(void);
static void usart3_default_rx_isr(uint8_t byte);
static void (*usart3_isr_tx)(void);
static void (*usart3_isr_rx)(uint8_t byte);

bool USART_setup(USART_Varient variant){

    USART_TypeDef *peripheral;

    // Map the variant to an actual port
    switch(variant){
        case USART_1A:
            GPIO_pinSetup(GPIO_UART_RX,GPIOA,10);
            GPIO_pinSetup(GPIO_UART_TX,GPIOA,9);
            peripheral = USART1;
            RCC_clockEnable(RCC_USART1);
            USART_setRxISR(peripheral,usart1_default_rx_isr);
            USART_setTxISR(peripheral,usart1_default_tx_isr);
            break;
        case USART_1B:
            GPIO_pinSetup(GPIO_UART_RX,GPIOB,7);
            GPIO_pinSetup(GPIO_UART_TX,GPIOB,6);
            peripheral = USART1;
            RCC_clockEnable(RCC_USART1);
            USART_setRxISR(peripheral,usart1_default_rx_isr);
            USART_setTxISR(peripheral,usart1_default_tx_isr);
            break;
        case USART_1C:
            GPIO_pinSetup(GPIO_UART_RX,GPIOB,15);
            GPIO_pinSetup(GPIO_UART_TX,GPIOB,14);
            peripheral = USART1;
            RCC_clockEnable(RCC_USART1);
            USART_setRxISR(peripheral,usart1_default_rx_isr);
            USART_setTxISR(peripheral,usart1_default_tx_isr);
            break;
        case USART_2A:
            GPIO_pinSetup(GPIO_UART_RX,GPIOA,3);
            GPIO_pinSetup(GPIO_UART_TX,GPIOA,2);
            peripheral = USART2;
            RCC_clockEnable(RCC_USART2);
            USART_setRxISR(peripheral,usart2_default_rx_isr);
            USART_setTxISR(peripheral,usart2_default_tx_isr);
            break;
        case USART_2B:
            GPIO_pinSetup(GPIO_UART_RX,GPIOD,6);
            GPIO_pinSetup(GPIO_UART_TX,GPIOD,5);
            peripheral = USART2;
            RCC_clockEnable(RCC_USART2);
            USART_setRxISR(peripheral,usart2_default_rx_isr);
            USART_setTxISR(peripheral,usart2_default_tx_isr);
            break;
        case USART_3A:
            GPIO_pinSetup(GPIO_UART_RX,GPIOB,11);
            GPIO_pinSetup(GPIO_UART_TX,GPIOB,10);
            peripheral = USART3;
            RCC_clockEnable(RCC_USART3);
            USART_setRxISR(peripheral,usart3_default_rx_isr);
            USART_setTxISR(peripheral,usart3_default_tx_isr);
            break;
        case USART_3B:
            GPIO_pinSetup(GPIO_UART_RX,GPIOC,11);
            GPIO_pinSetup(GPIO_UART_TX,GPIOC,10);
            peripheral = USART3;
            RCC_clockEnable(RCC_USART3);
            USART_setRxISR(peripheral,usart3_default_rx_isr);
            USART_setTxISR(peripheral,usart3_default_tx_isr);
            break;
        case USART_3C:
            GPIO_pinSetup(GPIO_UART_RX,GPIOD,9);
            GPIO_pinSetup(GPIO_UART_TX,GPIOD,8);
            peripheral = USART3;
            RCC_clockEnable(RCC_USART3);
            USART_setRxISR(peripheral,usart3_default_rx_isr);
            USART_setTxISR(peripheral,usart3_default_tx_isr);
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
    USART_enableISR(peripheral,USART_BOTH);
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

		// USART 1 and 6 are clocked from a (possibly) faster bus
		if((peripheral == USART1) || (peripheral == USART6)){
			peripheral->BRR = (uint32_t)(RCC_getClockSpeed(RCC_APB2_PERIPHERAL)/ baud);

		}else{

			peripheral->BRR = (uint32_t)(RCC_getClockSpeed(RCC_APB1_PERIPHERAL)/ baud);
		}
    peripheral->CR1 |= enabled ? USART_CR1_UE : 0;
    return SUCCESS;
}

bool USART_enableISR(USART_TypeDef *peripheral, USART_Isr isrType){

    if(peripheral == USART1){
        NVIC_DisableIRQ(USART1_IRQn);
    }
    else if(peripheral == USART2){
        NVIC_DisableIRQ(USART2_IRQn);
    }
    else if(peripheral == USART3){
        NVIC_DisableIRQ(USART3_IRQn);
    }

    peripheral->ICR |= USART_ICR_TCCF;
    peripheral->RQR |= USART_RQR_RXFRQ;

    if(isrType == USART_TX){
        peripheral->CR1 |= USART_CR1_TCIE;
        peripheral->CR1 &=~ USART_CR1_RXNEIE;
    }
    else if(isrType == USART_RX){
        peripheral->CR1 &= ~USART_CR1_TCIE;
        peripheral->CR1 |= USART_CR1_RXNEIE;
    }
    else if(isrType == USART_BOTH){
        peripheral->CR1 |= USART_CR1_TCIE;
        peripheral->CR1 |= USART_CR1_RXNEIE;
    }
    else{
        return ERROR;
    }

    if(peripheral == USART1){
        NVIC_EnableIRQ(USART1_IRQn);
    }
    else if(peripheral == USART2){
        NVIC_EnableIRQ(USART2_IRQn);
    }
    else if(peripheral == USART3){
        NVIC_EnableIRQ(USART3_IRQn);
    }

    return SUCCESS;
}

bool USART_disableISR(USART_TypeDef *peripheral){

    if(peripheral == USART1){
        NVIC_DisableIRQ(USART1_IRQn);
    }
    else if(peripheral == USART2){
        NVIC_DisableIRQ(USART2_IRQn);
    }
    else if(peripheral == USART3){
        NVIC_DisableIRQ(USART3_IRQn);
    }
    peripheral->CR1 &= ~USART_CR1_RXNEIE;
    peripheral->CR1 &= ~USART_CR1_TCIE;
    return SUCCESS;
}

bool USART_setRxISR(USART_TypeDef *peripheral, void (*new_handler)(uint8_t byte)){

    if(peripheral == USART1){
        usart1_isr_rx = new_handler;
    }
    else if(peripheral == USART2){
        usart2_isr_rx = new_handler;
    }
    else if(peripheral == USART3){
        usart3_isr_rx = new_handler;
    }
    else{
        return ERROR;
    }
    return SUCCESS;
}

bool USART_setTxISR(USART_TypeDef *peripheral, void (*new_handler)(void)){

    if(peripheral == USART1){
        usart1_isr_tx = new_handler;
    }
    else if(peripheral == USART2){
        usart2_isr_tx = new_handler;
    }
    else if(peripheral == USART3){
        usart3_isr_tx = new_handler;
    }
    else{
        return ERROR;
    }
    return SUCCESS;
}

void USART1_IRQHandler(void){

    if((USART1->CR1 & USART_CR1_RXNEIE) && (USART1->ISR & USART_ISR_RXNE)){
        // Interrupt occured because of RX
        usart1_isr_rx(USART1->RDR);
    }
    if((USART1->CR1 & USART_CR1_TCIE) && (USART1->ISR & USART_ISR_TC)){
        // Interrupt occured because of TX
        USART1->ICR |= USART_ICR_TCCF;
        usart1_isr_tx();
    }
}

void USART2_IRQHandler(void){

    if((USART2->CR1 & USART_CR1_RXNEIE) && (USART2->ISR & USART_ISR_RXNE)){
        // Interrupt occured because of RX
        usart2_isr_rx(USART2->RDR);
    }
    if((USART2->CR1 & USART_CR1_TCIE) && (USART2->ISR & USART_ISR_TC)){
        // Interrupt occured because of TX
        USART2->ICR |= USART_ICR_TCCF;
        usart2_isr_tx();
    }
}

void USART3_IRQHandler(void){

    if((USART3->CR1 & USART_CR1_RXNEIE) && (USART3->ISR & USART_ISR_RXNE)){
        // Interrupt occured because of RX
        usart3_isr_rx(USART3->RDR);
    }
    if((USART3->CR1 & USART_CR1_TCIE) && (USART3->ISR & USART_ISR_TC)){
        // Interrupt occured because of TX
        USART3->ICR |= USART_ICR_TCCF;
        usart3_isr_tx();
    }
}

void usart1_default_tx_isr(void){

    //This exists to catch un-implemented ISR calls
}

void usart1_default_rx_isr(uint8_t byte){

    //This exists to catch un-implemented ISR calls
    USART_put(USART1,byte);
}

void usart2_default_tx_isr(void){

    //This exists to catch un-implemented ISR calls
}

void usart2_default_rx_isr(uint8_t byte){

    //This exists to catch un-implemented ISR calls
    USART_put(USART2,byte);
}

void usart3_default_tx_isr(void){

    //This exists to catch un-implemented ISR calls
}

void usart3_default_rx_isr(uint8_t byte){

    //This exists to catch un-implemented ISR calls
    USART_put(USART3,byte);
}

