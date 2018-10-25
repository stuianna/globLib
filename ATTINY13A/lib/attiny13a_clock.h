#ifndef ATTINY13A_CLOCK_H_
#define ATTINY13A_CLOCK_H_

/**
    @addtogroup ATTINY13A 
* @{
*/

/**
    @addtogroup CLOCK
    @brief This module contains low level functions modifying clock parameters.
    @details 

    There are four options for clock speeds on the ATTiny13A using the internal RC oscillator. This
    speed is set by CLOCK parameter in the ** Makefile **. Although it is possible to adjust the
    clock speed using software. This module doesn't include this function.

    The four available clock speeds are
    - 9600000 (9.6MHz)
    - 4800000 (4.8MHz)
    - 1200000 (1.2MHz)
    - 600000  (0.6MHz)

    Note that lower clock speeds result in lower power consumption.

    Clock speeds have effects on the following modules:
    - TIMERS

    Their documentation details the effects.
    

    @author Stuart Ianna
    @version 0.1
    @date October 2018
    @copyright GNU GPLv3
    @warning Clock speed needs to be set in the Makefile.
    @bug 
        

    @par Compilers
    - avr-gcc 4.9.2

    @par Example Usage
    @include example_clock.c

 * @{
 */

/*! @file attiny13a_clock.h
    @brief Header file for attiny13a CLOCK 
*/

#include "attiny13a_gpio.h"
#include "attiny13a_debug.h"

#define CLOCK_BASE 9.6e6            //!<Maximum clock speed for internal RC oscillator
#define CLOCK_DIV_64 F_CPU/64       //!<Clock speed divided by 64
#define CLOCK_DIV_1024 F_CPU/1024   //!<Clock speed divided by 1024

/*! 
    @brief Get the current clock speed of the device
    @details 

    @return The current speed of the CPU bus in hertz. (Clock frequency)
*/
uint32_t CLOCK_getSpeed(void);
/*! 
    @brief Get the current clock divisor.
    @details This can be used to return the clock divisor for division
    in the main application.

    A sample usage of this application is in delayms() of attiny13a_core.h

    @return The current speed of the CPU clock bus. 

    - CPU = 9.6MHz - Divisor = 1
    - CPU = 4.8MHz - Divisor = 2
    - CPU = 1.2MHz - Divisor = 8
    - CPU = 0.6MHz - Divisor = 16 
*/
uint8_t CLOCK_getSpeedAlternate(void);
/**@}*/
/**@}*/
#endif
