#ifndef STM32F103CB_CLOCK_H_
#define STM32F103CB_CLOCK_H_

/**
    @addtogroup STM32F103CB 
* @{
*/

/**
    @addtogroup CLOCK
    @brief This module contains low level functions modifying clock parameters.
    @details 

    The STM32f1 has a maximum clock speed of 72MHz for external oscillator and 64MHz for intenal.

    By default, on startup, the clock is set to internal oscillator with 8MHz frequency.

    @note Clock switching needs to be done before any extra peripheral clocks are used!

    @author Stuart Ianna
    @version 0.1
    @date June 2018
    @copyright GNU GPLv3
    @warning None
    @bug Switching between all clock options doesn't work well.
        - Switch from internal ocsillator 64Mhz to a lower speed doesn't work
        
    @todo Add option for setting clock back to default option.

    @par Compilers
    - arm-none-eabi-gcc (15:4.9.3+svn231177-1) 4.9.3 20150529 (prerelease)

    @par Example Usage
    @include example_clock.c

 * @{
 */

/*! @file stm32f103cb_clock.h
    @brief Header file for stm32f103 CLOCK 
*/

#include "stm32f103cb_gpio.h"
#include "stm32f103cb_debug.h"
#include <libopencm3/stm32/rcc.h>
#include <stdint.h>

/*! 
    @brief Clock configuration available for MCU

    @details This doesn't represent all clock options available, further options can be utalised
    with libopencm3 firmware
*/
typedef enum{

    CLOCK_IN_OUT_64,        //!<Internal oscillator, multiplied to 64MHz final
    CLOCK_IN_OUT_48,        //!<Internal oscillator, multiplied to 48MHz final
    CLOCK_IN_OUT_24,        //!<Internal oscillator, multiplied to 24MHz final
    CLOCK_EX_8_OUT_24,      //!<External 8MHz oscillator, multiplied to 24MHz final
    CLOCK_EX_8_OUT_72,      //!<External 8MHz oscillator, multiplied to 72MHz final

}clock_option;


/*! 
    @brief Set the final clock speed of the device
    @details If this function isn't used, the default internal oscillator, 8Mhz

    @param option The clock speed option to use defined by #clock_option
    @return Error number as defined in #mcu_error, #E_CLOCK_NOSPEED if clock option doesn't exist.
*/
mcu_error CLOCK_setSpeed(clock_option option);
/*! 
    @brief Get the current clock speed of the device
    @details 

    @return The current speed of the ahp1 bus. (Clock frequency)
*/
uint32_t CLOCK_getSpeed(void);
/*! 
    @brief Get the current clock speed of the alternate bus
    @details 

    @return The current speed of the ahp2 bus. (Usuall Clock frequency/2)
*/
uint32_t CLOCK_getSpeedAlternate(void);
/**@}*/
/**@}*/
#endif
