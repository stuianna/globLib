#ifndef ATTINY13A_CORE_H
#define ATTINY13A_CORE_H

/**
    @addtogroup ATTINY13A
* @{
*/

/**
    @addtogroup CORE
    @brief Generic header file to include all modules
    @details This file can be included in applications to include all peripheral modules for the MCU.

    This file additionally contains useful fucntions specific to the attiny13a.

    @author Stuart Ianna
    @version 0.1
    @date October 2018
    @copyright GNU GPLv3
    @warning None
    @bug None
    @todo

    @par Verified Compilers
    - avr-gcc 4.9.2

 * @{
 */

/*! @file attiny13a_core.h
    @brief Generic header file for all modules
*/

#include "attiny13a_gpio.h"
#include "attiny13a_debug.h"
#include "attiny13a_clock.h"

/*! 
    @brief Millisecond blocking delay.
    @param delay The required delay
    @warning This function has a maximum delay of approximately 65 seconds.
*/
void delayms(uint16_t delay);

/**@}*/
/**@}*/
#endif
