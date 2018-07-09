#ifndef STM32F103C8_CORE_H
#define STM32F103C8_CORE_H

/**
    @addtogroup STM32F103
* @{
*/

/**
    @addtogroup CORE
    @brief Generic header file to include all modules
    @details This file can be included in applications to include all peripheral modules for the MCU.

    This file additionally contains useful fucntions specific to the stm32f1x3

    @author Stuart Ianna
    @version 1.0
    @date May 2018
    @copyright GNU GPLv3
    @warning None
    @bug None
    @todo

    @par Verified Compilers
    - arm-none-eabi-gcc (15:4.9.3+svn231177-1) 4.9.3 20150529 (prerelease)

 * @{
 */

/*! @file stm32f1x3_core.h
    @brief Generic header file for all modules
*/

#include "stm32f1x3_gpio.h"
#include "stm32f1x3_usart.h"
#include "stm32f1x3_i2c.h"
#include "stm32f1x3_clock.h"
#include "stm32f1x3_systick.h"
#include "stm32f1x3_timer.h"
#include "stm32f1x3_adc.h"
#include "stm32f1x3_iwdg.h"
#include "stm32f1x3_flash.h"
#include "stm32f1x3_usb.h"
#include "stm32f1x3_rtc.h"
#include "stm32f1x3_power.h"
#include "stm32f1x3_spi.h"
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
#endif
