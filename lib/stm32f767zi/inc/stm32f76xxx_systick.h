#ifndef LIB_STM32F76XXX_SYSTICK_H
#define LIB_STM32F76XXX_SYSTICK_H

/**
  @addtogroup STM32F76XXX
  @{
*/

/**
    @addtogroup SYSTICK
    @brief This module contains low level functions for systick usage.
    @details

    @author Stuart Ianna
    @date November 2019
    @warning None
    @bug
    @todo

    @par Example with start / stop and timeout modification.
    @include example_systick.cpp

    @par Example usage with multiple handlers
    @include example_systick_multiple_handlers.cpp

 * @{
 */

#include "stm32f76xxx_core.h"

/*!
    @brief Sets up systick and handler, the timer is running once this function returns.
    @details This function checks if the timeout value specified is possible, if not
		systick will not be enabled.
    @param timeout The desired timeout in microseconds
    @param handler The function to be called on interrupt event
    @return ERROR if the timeout is too short (clock frequency too low) or too long (clock frequency too high / timeout too long)
*/
uint8_t SYSTICK_setup(uint32_t timeout, void (*handler)(void));
/*!
    @brief Change the systick timeout value
    @details This function checks if the timeout value specified is possible, if not
		the timeout value is not changed an ERROR is returned.
    @param timeout The desired timeout in microseconds
    @return ERROR if the timeout is too short (clock frequency too low) or too long (clock frequency too high / timeout too long)
*/
uint8_t SYSTICK_timeout(uint32_t timeout);
/*!
    @brief Sets the handler called on interrupt event
    @param handler The function to be called on interrupt event
    @return none
*/
void SYSTICK_handler(void (*handler)(void));
/*!
    @brief Get the current clock value.
    @return The current value of the clock.
*/
uint32_t SYSTICK_count(void);
/*!
    @brief Stop the systick counter
    @return The current value of the clock.
*/
void SYSTICK_stop(void);
/*!
    @brief Start the systick counter
    Systick counter must have been set up with SYSTICK_setup() before this is used.
    @return The current value of the clock.
*/
void SYSTICK_start(void);
/**@}*/
/**@}*/

#endif
