#include "stm32f76xxx_gpio.h"

static void (*pin0_handler)(void)   = 0;
static void (*pin1_handler)(void)   = 0;
static void (*pin2_handler)(void)   = 0;
static void (*pin3_handler)(void)   = 0;
static void (*pin4_handler)(void)   = 0;
static void (*pin5_handler)(void)   = 0;
static void (*pin6_handler)(void)   = 0;
static void (*pin7_handler)(void)   = 0;
static void (*pin8_handler)(void)   = 0;
static void (*pin9_handler)(void)   = 0;
static void (*pin10_handler)(void)  = 0;
static void (*pin11_handler)(void)  = 0;
static void (*pin12_handler)(void)  = 0;
static void (*pin13_handler)(void)  = 0;
static void (*pin14_handler)(void)  = 0;
static void (*pin15_handler)(void)  = 0;

typedef enum{
    PORTA_EXTICR = 0x00,
    PORTB_EXTICR = 0x01,
    PORTC_EXTICR = 0x02,
    PORTD_EXTICR = 0x03,
    PORTE_EXTICR = 0x04,
    PORTF_EXTICR = 0x05,
    PORTG_EXTICR = 0x06,
    PORTH_EXTICR = 0x07,
    PORTI_EXTICR = 0x08,
    PORTJ_EXTICR = 0x09,
    PORTK_EXTICR = 0x0A,
}GPIO_Exti_Port_Mask;

bool GPIO_setEXTIPortMask(uint8_t pin, uint8_t mask, void (*handle)(void));
bool GPIO_setAlternateFunction(GPIO_TypeDef *port ,uint8_t  pin, uint8_t number);

bool GPIO_setHandler(uint8_t num, void (*handler)(void)){

    switch(num){

        case 0: pin0_handler = handler;  break;
        case 1: pin1_handler = handler;  break;
        case 2: pin2_handler = handler;  break;
        case 3: pin3_handler = handler;  break;
        case 4: pin4_handler = handler;  break;
        case 5: pin5_handler = handler;  break;
        case 6: pin6_handler = handler;  break;
        case 7: pin7_handler = handler;  break;
        case 8: pin8_handler = handler;  break;
        case 9: pin9_handler = handler;  break;
        case 10: pin10_handler = handler;  break;
        case 11: pin11_handler = handler;  break;
        case 12: pin12_handler = handler;  break;
        case 13: pin13_handler = handler;  break;
        case 14: pin14_handler = handler;  break;
        case 15: pin15_handler = handler;  break;
        default: return ERROR;
    }
    return SUCCESS;
}

bool GPIO_pinSetup(GPIO_Mode mode, GPIO_TypeDef *port, uint8_t pin){


    if(port == GPIOA){
        RCC_clockEnable(RCC_GPIOA);
        switch(mode){
            case GPIO_UART_TX:  GPIO_setAlternateFunction(port,pin,7);   break;
            case GPIO_UART_RX:  GPIO_setAlternateFunction(port,pin,7);   break;
            case GPIO_DI:       port->MODER |= (GPIO_DI << pin * 2);   break;
            case GPIO_DO:       port->MODER |= (GPIO_DO << pin * 2);   break;
            case GPIO_ADC:      port->MODER |= (GPIO_ADC << pin * 2);   break;
            default:            port->MODER |= (GPIO_ADC << pin * 2);   break;
        }
    }
    else if(port == GPIOB){
        RCC_clockEnable(RCC_GPIOB);
        switch(mode){
            case GPIO_UART_TX:
                if(pin == 14){
                    GPIO_setAlternateFunction(port,pin,4);
                }
                else{
                    GPIO_setAlternateFunction(port,pin,7);
                }
                break;
            case GPIO_UART_RX:
                if(pin == 15){
                    GPIO_setAlternateFunction(port,pin,4);
                }
                else{
                    GPIO_setAlternateFunction(port,pin,7);
                }
                break;
            case GPIO_DI:       port->MODER |= (GPIO_DI << pin * 2);   break;
            case GPIO_DO:       port->MODER |= (GPIO_DO << pin * 2);   break;
            case GPIO_ADC:      port->MODER |= (GPIO_ADC << pin * 2);   break;
            default:            port->MODER |= (GPIO_ADC << pin * 2);   break;
        }
    }
    else if(port == GPIOC){
        RCC_clockEnable(RCC_GPIOC);
        port->MODER |= (mode << pin*2);
    }
    else if(port == GPIOD){
        RCC_clockEnable(RCC_GPIOD);
        switch(mode){
            case GPIO_UART_TX:  GPIO_setAlternateFunction(port,pin,7);   break;
            case GPIO_UART_RX:  GPIO_setAlternateFunction(port,pin,7);   break;
            case GPIO_DI:       port->MODER |= (GPIO_DI << pin * 2);   break;
            case GPIO_DO:       port->MODER |= (GPIO_DO << pin * 2);   break;
            case GPIO_ADC:      port->MODER |= (GPIO_ADC << pin * 2);   break;
            default:            port->MODER |= (GPIO_ADC << pin * 2);   break;
        }
    }
    else if(port == GPIOE){
        RCC_clockEnable(RCC_GPIOE);
        port->MODER |= (mode << pin*2);
    }
    else if(port == GPIOF){
        RCC_clockEnable(RCC_GPIOF);
        port->MODER |= (mode << pin*2);
    }
    else if(port == GPIOG){
        RCC_clockEnable(RCC_GPIOG);
        port->MODER |= (mode << pin*2);
    }
    else if(port == GPIOH){
        RCC_clockEnable(RCC_GPIOH);
        port->MODER |= (mode << pin*2);
    }
    else if(port == GPIOI){
        RCC_clockEnable(RCC_GPIOI);
        port->MODER |= (mode << pin*2);
    }
    else if(port == GPIOJ){
        RCC_clockEnable(RCC_GPIOJ);
        port->MODER |= (mode << pin*2);
    }
    else if(port == GPIOK){
        RCC_clockEnable(RCC_GPIOK);
        port->MODER |= (mode << pin*2);
    }
    else{
        return ERROR;
    }


    return SUCCESS;
}

bool GPIO_setAlternateFunction(GPIO_TypeDef *port ,uint8_t  pin, uint8_t number){

    port->MODER |= (0x2 << pin * 2);
    port->AFR[pin >> 3] &= ~(0xF << ( (pin - (pin & 0x8)) * 4));
    port->AFR[pin >> 3] |= number << ( (pin - (pin & 0x8)) * 4);
    return SUCCESS;
}

void GPIO_pinHigh(GPIO_TypeDef *port, uint8_t pin){

    SET_BIT(port->BSRR,0x1<<pin);
    return;
}

void GPIO_pinLow(GPIO_TypeDef *port, uint8_t pin){

    SET_BIT(port->BSRR,0x1<<(pin+16));
    return;
}

void GPIO_pinWrite(GPIO_TypeDef *port, uint8_t pin, bool state){

    if(state){
        GPIO_pinHigh(port,pin);
    }
    else{
        GPIO_pinLow(port,pin);
    }
    return;
}

void GPIO_pinToggle(GPIO_TypeDef *port, uint8_t pin){

    GPIO_pinWrite(port,pin,!GPIO_pinRead(port,pin));
    return;
}

bool GPIO_pinRead(GPIO_TypeDef *port, uint8_t pin){

    if(READ_REG(port->IDR) & (0x1 << pin)){
        return true;
    }
    else{
        return false;
    }
    return false;
}

bool GPIO_speed(GPIO_TypeDef *port, uint8_t pin, GPIO_Speed speed){

    if(pin > 16){
        return ERROR;
    }
    CLEAR_BIT(port->OSPEEDR, 0x3 << pin*2 );
    SET_BIT(port->OSPEEDR, speed << pin*2 );
    return SUCCESS;
}

bool GPIO_oType(GPIO_TypeDef *port, uint8_t pin, GPIO_Output_Type type){

    if(pin > 16){
        return ERROR;
    }
    CLEAR_BIT(port->OTYPER, 0x3 << pin*2 );
    SET_BIT(port->OTYPER, type << pin*2 );
    return SUCCESS;
}

bool GPIO_pupd(GPIO_TypeDef *port, uint8_t pin, GPIO_Pupd type){

    if(pin > 16){
        return ERROR;
    }
    CLEAR_BIT(port->PUPDR, 0x3 << pin*2 );
    SET_BIT(port->PUPDR, type << pin*2 );
    return SUCCESS;
}

bool GPIO_ISREnable(GPIO_TypeDef *port, uint8_t pin, GPIO_Isr trigger, void (*handle)(void)){

    RCC_clockEnable(RCC_SYSCFG);

    // Check if the pin is available
    if((EXTI->IMR & pin)){
        return ERROR;
    }

    GPIO_pinSetup(GPIO_DI,port,pin);

    //Configure the EXTI line to be mapped to that port
    if(port == GPIOA){
        GPIO_setEXTIPortMask(pin, PORTA_EXTICR, handle);
    }
    else if(port == GPIOB){
        GPIO_setEXTIPortMask(pin, PORTB_EXTICR, handle);
    }
    else if(port == GPIOC){
        GPIO_setEXTIPortMask(pin, PORTC_EXTICR, handle);
    }
    else if(port == GPIOD){
        GPIO_setEXTIPortMask(pin, PORTD_EXTICR, handle);
    }
    else if(port == GPIOE){
        GPIO_setEXTIPortMask(pin, PORTE_EXTICR, handle);
    }
    else if(port == GPIOF){
        GPIO_setEXTIPortMask(pin, PORTF_EXTICR, handle);
    }
    else if(port == GPIOG){
        GPIO_setEXTIPortMask(pin, PORTG_EXTICR, handle);
    }
    else if(port == GPIOH){
        GPIO_setEXTIPortMask(pin, PORTH_EXTICR, handle);
    }
    else if(port == GPIOI){
        GPIO_setEXTIPortMask(pin, PORTI_EXTICR, handle);
    }
    else if(port == GPIOJ){
        GPIO_setEXTIPortMask(pin, PORTJ_EXTICR, handle);
    }
    else if(port == GPIOK){
        GPIO_setEXTIPortMask(pin, PORTK_EXTICR, handle);
    }
    else{
        return ERROR;
    }

    // Enable interrupt mask
    EXTI->IMR |= (0x1 << pin);

    //Set rising or falling trigger
    switch(trigger){
        case GPIO_RISING:
            EXTI->RTSR |= (0x1 << pin);
            EXTI->FTSR &= ~(0x1 << pin);
            break;
        case GPIO_FALLING:
            EXTI->RTSR &= ~(0x1 << pin);
            EXTI->FTSR |= (0x1 << pin);
            break;
        case GPIO_BOTH:
            EXTI->RTSR |= (0x1 << pin);
            EXTI->FTSR |= (0x1 << pin);
            break;
        default:
            return ERROR;
            break;
    }

    if(pin == 0){
        NVIC_EnableIRQ(EXTI0_IRQn);
    }
    else if(pin == 1){
        NVIC_EnableIRQ(EXTI1_IRQn);
    }
    else if(pin == 2){
        NVIC_EnableIRQ(EXTI2_IRQn);
    }
    else if(pin == 3){
        NVIC_EnableIRQ(EXTI3_IRQn);
    }
    else if(pin == 4){
        NVIC_EnableIRQ(EXTI4_IRQn);
    }
    else if(pin <= 9){
        NVIC_EnableIRQ(EXTI9_5_IRQn);
    }
    else if(pin <= 15){
        NVIC_EnableIRQ(EXTI15_10_IRQn);
    }
    else{
        return ERROR;
    }
    return SUCCESS;
}

bool GPIO_ISRDisable(GPIO_TypeDef *port,uint8_t  pin){

    (void)port;
    if(pin == 0){
        NVIC_DisableIRQ(EXTI0_IRQn);
    }
    else if(pin == 1){
        NVIC_DisableIRQ(EXTI1_IRQn);
    }
    else if(pin == 2){
        NVIC_DisableIRQ(EXTI2_IRQn);
    }
    else if(pin == 3){
        NVIC_DisableIRQ(EXTI3_IRQn);
    }
    else if(pin == 4){
        NVIC_DisableIRQ(EXTI4_IRQn);
    }
    else if(pin <= 9){
        NVIC_DisableIRQ(EXTI9_5_IRQn);
    }
    else if(pin <= 15){
        NVIC_DisableIRQ(EXTI15_10_IRQn);
    }
    else{
        return ERROR;
    }
    return SUCCESS;

}

bool GPIO_setEXTIPortMask(uint8_t pin, uint8_t mask, void (*handle)(void)){

    switch(pin){
        case 0:
            SYSCFG->EXTICR[0] &= ~(0xF);
            SYSCFG->EXTICR[0] |= (mask);
            pin0_handler = handle;
            break;
        case 1:
            SYSCFG->EXTICR[0] &= ~(0xF<<4);
            SYSCFG->EXTICR[0] |= (mask << 0x4);
            pin1_handler = handle;
            break;
        case 2:
            SYSCFG->EXTICR[0] &= ~(0xF<<8);
            SYSCFG->EXTICR[0] |= (mask << 0x8);
            pin2_handler = handle;
            break;
        case 3:
            SYSCFG->EXTICR[0] &= ~(0xF<<12);
            SYSCFG->EXTICR[0] |= (mask << 0xC);
            pin3_handler = handle;
            break;
        case 4:
            SYSCFG->EXTICR[1] &= ~(0xF);
            SYSCFG->EXTICR[1] |= (mask);
            pin4_handler = handle;
            break;
        case 5:
            SYSCFG->EXTICR[1] &= ~(0xF<<4);
            SYSCFG->EXTICR[1] |= (mask << 0x4);
            pin5_handler = handle;
            break;
        case 6:
            SYSCFG->EXTICR[1] &= ~(0xF<<8);
            SYSCFG->EXTICR[1] |= (mask << 0x8);
            pin6_handler = handle;
            break;
        case 7:
            SYSCFG->EXTICR[1] &= ~(0xF<<12);
            SYSCFG->EXTICR[1] |= (mask << 0xC);
            pin7_handler = handle;
            break;
        case 8:
            SYSCFG->EXTICR[2] &= ~(0xF);
            SYSCFG->EXTICR[2] |= (mask);
            pin8_handler = handle;
            break;
        case 9:
            SYSCFG->EXTICR[2] &= ~(0xF<<4);
            SYSCFG->EXTICR[2] |= (mask << 0x4);
            pin9_handler = handle;
            break;
        case 10:
            SYSCFG->EXTICR[2] &= ~(0xF<<8);
            SYSCFG->EXTICR[2] |= (mask << 0x8);
            pin10_handler = handle;
            break;
        case 11:
            SYSCFG->EXTICR[2] &= ~(0xF<<12);
            SYSCFG->EXTICR[2] |= (mask << 0xC);
            pin11_handler = handle;
            break;
        case 12:
            SYSCFG->EXTICR[3] &= ~(0xF);
            SYSCFG->EXTICR[3] |= (mask);
            pin12_handler = handle;
            break;
        case 13:
            SYSCFG->EXTICR[3] &= ~(0xF<<4);
            SYSCFG->EXTICR[3] |= (mask << 0x4);
            pin13_handler = handle;
            break;
        case 14:
            SYSCFG->EXTICR[3] &= ~(0xF<<8);
            SYSCFG->EXTICR[3] |= (mask << 0x8);
            pin14_handler = handle;
            break;
        case 15:
            SYSCFG->EXTICR[3] &= ~(0xF<<12);
            SYSCFG->EXTICR[3] |= (mask << 0xC);
            pin15_handler = handle;
            break;
        default:
            return ERROR;
            break;
    }
    return SUCCESS;
}

void EXTI0_IRQHandler(void){

    EXTI->PR = EXTI_PR_PR0;
    pin0_handler();
}

void EXTI1_IRQHandler(void){

    EXTI->PR = EXTI_PR_PR1;
    pin1_handler();
}

void EXTI2_IRQHandler(void){

    EXTI->PR = EXTI_PR_PR2;
    pin2_handler();
}

void EXTI3_IRQHandler(void){

    EXTI->PR = EXTI_PR_PR3;
    pin3_handler();
}


void EXTI4_IRQHandler(void){

    EXTI->PR = EXTI_PR_PR4;
    pin4_handler();
}

void EXTI9_5_IRQHandler(void){

    if(EXTI->PR & EXTI_PR_PR5){
        EXTI->PR = EXTI_PR_PR5;
        pin5_handler();
    }
    if(EXTI->PR & EXTI_PR_PR6){
        EXTI->PR = EXTI_PR_PR6;
        pin6_handler();
    }
    if(EXTI->PR & EXTI_PR_PR7){
        EXTI->PR = EXTI_PR_PR7;
        pin7_handler();
    }
    if(EXTI->PR & EXTI_PR_PR8){
        EXTI->PR = EXTI_PR_PR8;
        pin8_handler();
    }
    if(EXTI->PR & EXTI_PR_PR9){
        EXTI->PR = EXTI_PR_PR9;
        pin9_handler();
    }
}

void EXTI15_10_IRQHandler(void){

    if(EXTI->PR & EXTI_PR_PR10){
        EXTI->PR = EXTI_PR_PR10;
        pin10_handler();
    }
    if(EXTI->PR & EXTI_PR_PR11){
        EXTI->PR = EXTI_PR_PR11;
        pin11_handler();
    }
    if(EXTI->PR & EXTI_PR_PR12){
        EXTI->PR = EXTI_PR_PR12;
        pin12_handler();
    }
    if(EXTI->PR & EXTI_PR_PR13){
        EXTI->PR = EXTI_PR_PR13;
        pin13_handler();
    }
    if(EXTI->PR & EXTI_PR_PR14){
        EXTI->PR = EXTI_PR_PR14;
        pin14_handler();
    }
    if(EXTI->PR & EXTI_PR_PR15){
        EXTI->PR = EXTI_PR_PR15;
        pin15_handler();
    }
}
