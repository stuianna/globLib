#ifndef STM32F103CB_IWDG_H_
#define STM32F103CB_IWDG_H_

/**
    @addtogroup STM32F103CB 
* @{
*/

/**
    @addtogroup IWDG
    @brief This module contains low level functions for watchdog configuration.
    @details 

    The stm32f103cb independant watchdog timer can be configured with a timeout of up to 26 seconds.
    To avoid a forced reset the timer should be reloaded with IWDG_feed() before the timeout time has
    elapsed.

    IWDG_resetCheck() can be used to check if a reset has occured from a watchdog timeout.

    @author Stuart Ianna
    @version 0.1
    @date June 2018
    @copyright GNU GPLv3
    @warning None
    @bug None
    @todo

    @par Verified Compilers
    - arm-none-eabi-gcc (15:4.9.3+svn231177-1) 4.9.3 20150529 (prerelease)

    @par Working example
    This program's loop time increasing by a factor of 10 each itteration. The increasing time
    causes a forced reset by the watchdog timer.
    @include example_iwdg.c

 * @{
 */

/*! @file stm32f103cb_iwdg.h
    @brief Header file for stm32f103cb independant watchdog timer 
*/

#include "stm32f103cb_gpio.h"
#include "stm32f103cb_debug.h"
#include <libopencm3/stm32/iwdg.h>
#include <stdint.h>

/*! 
    @brief Initialize the independant watchdog timer.
    @details This must be called before any other  feature is used.

    @param timeout The timeout period in milliseconds
    @return Error number as defined in #mcu_error, E_IWDG_NOERROR if no error, E_IWDG_PERIOD if period
    is too long.
*/
mcu_error IWDG_setup(uint32_t timeout);
/*! 
    @brief Check if reset occured from watchdog timeout
    @details 

    @return 
    - 1 if reset occured from watchdog 
    - 0 from other reset type.
*/
bool IWDG_resetCheck(void);
/*! 
    @brief Reload the watchdog timer
    @details This should be called at least as often as the watchdog timeout value to avoid forced reset.
*/
void IWDG_feed(void);
/**@}*/
/**@}*/

#endif
