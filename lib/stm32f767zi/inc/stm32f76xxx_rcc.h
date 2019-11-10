#ifndef LIB_STM32F76XXX_RCC_H
#define LIB_STM32F76XXX_RCC_H

#include "stm32f76xxx_core.h"

/**
  @addtogroup STM32F76XXX
  @{
*/

/*! 
    @addtogroup RCC
    @brief Module for reset and clock control (RCC) configuration. Applicable for ARM based MCU.
    @author Stuart Ianna
    @date November 2019
    @warning none
    @todo
  @{
*/

/*! 
    @brief RCC peripheral clocks.
    @details This abstraction allows for common naming and a single function to be
    used to enable and disable clocks.
*/
typedef enum{

    RCC_GPIOA       = RCC_AHB1ENR_GPIOAEN,          //!<GPIOA Clock
    RCC_GPIOB       = RCC_AHB1ENR_GPIOBEN,          //!<GPIOB Clock
    RCC_GPIOC       = RCC_AHB1ENR_GPIOCEN,          //!<GPIOC Clock
    RCC_GPIOD       = RCC_AHB1ENR_GPIODEN,          //!<GPIOD Clock
    RCC_GPIOE       = RCC_AHB1ENR_GPIOEEN,          //!<GPIOE Clock
    RCC_GPIOF       = RCC_AHB1ENR_GPIOFEN,          //!<GPIOF Clock
    RCC_GPIOG       = RCC_AHB1ENR_GPIOGEN,          //!<GPIOG Clock
    RCC_GPIOH       = RCC_AHB1ENR_GPIOHEN,          //!<GPIOH Clock
    RCC_GPIOI       = RCC_AHB1ENR_GPIOIEN,          //!<GPIOI Clock
    RCC_GPIOJ       = RCC_AHB1ENR_GPIOJEN,          //!<GPIOJ Clock
    RCC_GPIOK       = RCC_AHB1ENR_GPIOKEN,          //!<GPIOK Clock
    RCC_SYSCFG      = RCC_APB2ENR_SYSCFGEN,         //!<System configuration clock.
    RCC_USART3      = RCC_APB1ENR_USART3EN,         //!<USART 3 clock
}RCC_Peripheral;

/*! 
    @brief Enables a given peripheral clock.
    @param peripheral The clock to enable, defined by #RCC_Peripherals
    @return SUCCESS if enable, FAIL if not.
*/
bool clockEnable(RCC_Peripheral peripheral);
/*! 
    @brief Disables a given peripheral clock.
    @param peripheral The clock to disable, defined by #RCC_Peripherals
    @return SUCCESS if enable, FAIL if not.
*/
bool clockDisable(RCC_Peripheral peripheral);

/**@}*/
/**@}*/

#endif
