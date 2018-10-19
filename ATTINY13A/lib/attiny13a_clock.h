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

    The ATTINY13A has a maximum clock speed of 9.6MHz for the internal RC oscillator.
    By default, on startup, the clock is set to internal oscillator with 9.6MHz frequency.

    @note Clock switching needs to be done before any extra peripheral clocks are used!



    @author Stuart Ianna
    @version 0.1
    @date October 2018
    @copyright GNU GPLv3
    @warning None
    @bug Switching between all clock options doesn't work well.
        - Switch from internal ocsillator 64Mhz to a lower speed doesn't work
        
    @todo Add option for setting clock back to default option.

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

/*! 
    @brief Clock configuration available for MCU

    @details In theory the clock speed can be reduced further by increaing
    the division of the CLKPR register (see source code). Although with the
    USBASP, frequencies lower than 600kHz will case the programmer to no longer
    be able to comunicate with the MCU and no more firmware can be uploaded.


    Should the MCU become locked under these conditions, the device can be powered
    on with the reset pin held low, causing no application code to run. The device
    can then be flashed with a higher clock speed to be unlocked.
*/
typedef enum{

    CLOCK_IN_9_6M,        //!<Internal oscillator, divided to 9.6MHz final
    CLOCK_IN_4_8M,        //!<Internal oscillator, divided to 4.8MHz final
    CLOCK_IN_2_4M,        //!<Internal oscillator, divided to 2.4MHz final
    CLOCK_IN_1_2M,        //!<Internal oscillator, divided to 1.2MHz final
    CLOCK_IN_600K,        //!<Internal oscillator, divided to 600kHz final
}clock_option;


/*! 
    @brief Set the final clock speed of the device
    @details If this function isn't used, the default speed of 9.6MHz
    will be used.

    @param option The clock speed option to use defined by #clock_option
    @return Error number as defined in #mcu_error, #E_CLOCK_NOSPEED if clock option doesn't exist.
*/
mcu_error CLOCK_setSpeed(clock_option option);
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

    - CLOCK_IN_9_6M - Divisor = 0
    - CLOCK_IN_4_8M - Divisor = 1
    - CLOCK_IN_2_4M - Divisor = 2
    - CLOCK_IN_1_2M - Divisor = 3
    - CLOCK_IN_600K - Divisor = 4
*/
uint8_t CLOCK_getSpeedAlternate(void);
/**@}*/
/**@}*/
#endif
