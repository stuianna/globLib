#ifndef STM32F103C8_SYSTICK_H_
#define STM32F103C8_SYSTICK_H_

/**
    @addtogroup STM32F103
* @{
*/

/**
    @addtogroup SYSTICK
    @brief This module contains low level functions for systick usage.
    @details 


    @author Stuart Ianna
    @version 1.0
    @date June 2018
    @copyright GNU GPLv3
    @warning None
    @bug 
    @todo

    @par Verified Compilers
    - arm-none-eabi-gcc (15:4.9.3+svn231177-1) 4.9.3 20150529 (prerelease)

    @par Example Usage
    @include example_systick.c

 * @{
 */

/*! @file stm32f1x3_systick.h
    @brief Header file for stm32f103 SYSTICK
*/

#include "stm32f1x3_gpio.h"
#include "stm32f1x3_debug.h"
#include "stm32f1x3_clock.h"
#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/nvic.h>
#include <stdint.h>

/*! 
    @brief This is a dummy define so that USB_coms() can use systick as a timer
    option.
*/
#define SYSTICK 27

/*! 
    @brief Sets up systick and handler
    @details This function checks if the timeout value specified is possible, defaults to ..... if not.

    @param timeout The desired timeout in microseconds
    @param handler The function to be called on interrupt event
    @return Error code define by mcu_error, SYSTICK_NOERROR if no error, SYSTICK_TOSHORT if timeout value is too small
*/
mcu_error SYSTICK_setup(uint32_t timeout, void (*handler)(void));
/*! 
    @brief Change the systick timeout value
    @details This function checks if the timeout value specified is possible, defaults to ..... if not.

    @param timeout The desired timeout in microseconds
    @return Error code define by mcu_error, SYSTICK_NOERROR if no error, SYSTICK_TOOLONG if timeout value is too small
*/
mcu_error SYSTICK_timeout(uint32_t timeout);
/*! 
    @brief Sets the handler called on interrupt event
    @details 

    @param handler The function to be called on interrupt event
    @return none
*/
void SYSTICK_handler(void (*handler)(void));
/*! 
    @brief Get the current clock value
    @details 

    @return The current value of the clock.
*/
uint32_t SYSTICK_count(void);
/*! 
    @brief Stop the systick counter
    @details 

    @return The current value of the clock.
*/
void SYSTICK_stop(void);
/*! 
    @brief Start the systick counter
    @details 

    Systick counter must have been set up with SYSTICK_setup() before this is used.
    @return The current value of the clock.
*/
void SYSTICK_start(void);
/**@}*/
#endif
