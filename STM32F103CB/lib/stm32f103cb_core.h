#ifndef STM32F103CB_CORE_H
#define STM32F103CB_CORE_H

/**
    @addtogroup STM32F103CB 
* @{
*/

/**
    @addtogroup CORE
    @brief Generic header file to include all modules
    @details This file can be included in applications to include all peripheral modules for the MCU.

    This file additionally contains useful fucntions specific to the stm32f103cb 

    @author Stuart Ianna
    @version 0.1
    @date May 2018
    @copyright GNU GPLv3
    @warning None
    @bug None
    @todo

    @par Compilers
    - arm-none-eabi-gcc (15:4.9.3+svn231177-1) 4.9.3 20150529 (prerelease)

 * @{
 */

/*! @file stm32f103cb_core.h
    @brief Generic header file for all modules
*/

#include "stm32f103cb_gpio.h"
#include "stm32f103cb_usart.h"
#include "stm32f103cb_i2c.h"
#include "stm32f103cb_clock.h"
#include "stm32f103cb_systick.h"
#include "stm32f103cb_timer.h"
#include "stm32f103cb_adc.h"
#include "stm32f103cb_iwdg.h"
#include "stm32f103cb_flash.h"
#include "stm32f103cb_usb.h"
#include "stm32f103cb_rtc.h"
#include "stm32f103cb_power.h"
#include "stm32f103cb_spi.h"
#include <string11.h>

/*! 
    @brief Set up USB "serial" communication. The passed timer is used to
    update the device.
    @details The following options are valid.
    
        - TIMER_1
        - TIMER_2
        - TIMER_3
        - TIMER_4
        - SYSTICK

    Currently this also sets the main clock speed to 72MHz (external crystal)

    @param timerNumber the timer to initial the USB device with
*/
void USB_coms(timer_main timerNumber);
/*! 
    @brief Millisecond blocking delay.
    @param delay The required delay
*/
void delayms(uint32_t delay);

/**@}*/
/**@}*/
#endif
