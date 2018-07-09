#ifndef STM32F103C8_POWER_H_
#define STM32F103C8_POWER_H_

/**
    @addtogroup STM32F103
* @{
*/

/**
    @addtogroup POWER
    @brief This module contains low level functions for power configuration.
    @details 

    There are four power modes that the MCU can operate in. Each proceeding mode
    has a lower power consumption but higher restart time.
        - Normal,  This is the default state, every works as usual
        - Sleep, The clock is stalled, all NVIC functions remain, pin setings
        held in current state any ISR routine will fire and execute, RAM contents
        preserved. This mode can be disabled by using POWER_noSleep() in any ISR.
        - Stop, The clock is stalled, all perihperal clock stopped (not RTC). Only
        EXTI (external pin) interrupts enabled. RAM contents preserved, program
       counter preserved. When this mode is exited all peripherals need to be 
        set up again. 
        - Standby, All clocks and perihperals stopped, external pins set as
        floating, RAM contents lost, program counter lost. System can only be woken
        by RTC alarm or rising edge on PA1. On wakeup the system is as from reset
        state.


    This module also contains the hard fault handler. In its implementation, the
    handler determines if the bootloader is being used (Data register 1 of the 
    backup domain = 1) and resets to boot mode.

    If the bootloader is not being used, the system simply stalls.


    @author Stuart Ianna
    @version 1.0
    @date July 2018
    @copyright GNU GPLv3
    @warning None
    @bug 
    @todo

    @par Verified Compilers
    - arm-none-eabi-gcc (15:4.9.3+svn231177-1) 4.9.3 20150529 (prerelease)

    @par Example Usage

    This program works over USART_1. It provides four options which can be selected
    through terminal.
      - 1 But the system in sleep mode, wake up in 3 seconds using systick ISR
      - 2 Put the system in stop mode, wake up with rising edge on PA0
      - 3 Put the system in standby mode, wake up in 3 seconds using RTC alarm
      - 4 Reset the sytem.

    @include example_power.c

 * @{
 */

/*! @file stm32f1x3_power.h
    @brief Header file for stm32f103 Power
*/

#include "stm32f1x3_gpio.h"
#include "stm32f1x3_debug.h"
#include "stm32f1x3_clock.h"
#include "stm32f1x3_usb.h"
#include <libopencm3/stm32/f1/pwr.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/stm32/f1/bkp.h>
#include <libopencmsis/core_cm3.h>
#include <stdint.h>


/*! 
    @brief Resets the system
*/
void POWER_reset(void);
/*! 
    @brief Put the system in sleep mode.
    @details ISR are still enabled. This mode can be exited by calling 
    Power_noSleep() inside an ISR.
*/
void POWER_sleep(void);
/*! 
    @brief Disable sleep mode.
    @details This should be called inside an ISR.
*/
void POWER_noSleep(void);
/*! 
    @brief Put the device into standby mode.
    @details The RTC must be enabled with an alarm event to exit from standby
    mode.
*/
void POWER_standby(void);
/*! 
    @brief Check if the system is restarting from standby mode.
    @details This should always be used if standby mode is used. This resets
    the standby wake flags.
    mode.
*/
uint8_t POWER_fromStandby(void);
/*! 
    @brief Put the device into stop mode
    @details The device needs to be woken with an EXTI interrupt or RTC alarm
    event. All clocks need to be reinitialized after stop mode.
*/
void POWER_stop(void);
/*! 
    @brief Check if MCU was in stop mode and reset flags.
    @details This should always be used if stop mode is used.
*/
uint8_t POWER_fromStop(void);

/**@}*/
#endif
