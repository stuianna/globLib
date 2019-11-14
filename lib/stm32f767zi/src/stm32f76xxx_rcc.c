#include "stm32f76xxx_rcc.h"

bool clockEnable(RCC_Peripheral peripheral){

    switch(peripheral){
        case RCC_AHB1ENR_GPIOAEN:
        case RCC_AHB1ENR_GPIOBEN:
        case RCC_AHB1ENR_GPIOCEN:
        case RCC_AHB1ENR_GPIODEN:
        case RCC_AHB1ENR_GPIOEEN:
        case RCC_AHB1ENR_GPIOFEN:
        case RCC_AHB1ENR_GPIOGEN:
        case RCC_AHB1ENR_GPIOHEN:
        case RCC_AHB1ENR_GPIOIEN:
        case RCC_AHB1ENR_GPIOJEN:
        case RCC_AHB1ENR_GPIOKEN:
            SET_BIT(RCC->AHB1ENR,peripheral);
            break;

        case RCC_APB1ENR_USART2EN:
        case RCC_APB1ENR_USART3EN:
            SET_BIT(RCC->APB1ENR,peripheral);
            break;

        case RCC_APB2ENR_SYSCFGEN:
            SET_BIT(RCC->APB2ENR,peripheral);
            break;
        default:
            break;
    }

    switch(peripheral){
        case RCC_APB2ENR_USART1EN:
            SET_BIT(RCC->APB2ENR,peripheral);
            break;
        default:
            return ERROR;
    }
    return SUCCESS;
}

bool clockDisable(RCC_Peripheral peripheral){

    switch(peripheral){
        case RCC_AHB1ENR_GPIOAEN:
        case RCC_AHB1ENR_GPIOBEN:
        case RCC_AHB1ENR_GPIOCEN:
        case RCC_AHB1ENR_GPIODEN:
        case RCC_AHB1ENR_GPIOEEN:
        case RCC_AHB1ENR_GPIOFEN:
        case RCC_AHB1ENR_GPIOGEN:
        case RCC_AHB1ENR_GPIOHEN:
        case RCC_AHB1ENR_GPIOIEN:
        case RCC_AHB1ENR_GPIOJEN:
        case RCC_AHB1ENR_GPIOKEN:
            CLEAR_BIT(RCC->AHB1ENR,peripheral);
            break;

        case RCC_APB1ENR_USART2EN:
        case RCC_APB1ENR_USART3EN:
            CLEAR_BIT(RCC->APB1ENR,peripheral);
            break;

        case RCC_APB2ENR_SYSCFGEN:
            CLEAR_BIT(RCC->APB2ENR,peripheral);
            break;
        default:
            break;
    }

    switch(peripheral){
        case RCC_APB2ENR_USART1EN:
            CLEAR_BIT(RCC->APB2ENR,peripheral);
            break;
        default:
            return ERROR;
    }
    return SUCCESS;

}
