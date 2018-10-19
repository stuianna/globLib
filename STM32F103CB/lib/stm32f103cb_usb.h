#ifndef STM32F103CB_USB_H_
#define STM32F103CB_USB_H_

/**
    @addtogroup STM32F103CB 
* @{
*/

/**
    @addtogroup USB
    @brief This module contains low level functions for USB interaction.
    @details The USB stack is taken from a modified version of OPENCM3 lib.

    The USB module has a maximum data rate of 64kb / second. 

    When using the USB, USB_update must be called at at least 4KHz. Higher
    frequencies can be used, however this just causes unneccesary CPU usage.

    The clock frequency for the MCU must be either internal 48MHz or external 
    72MHz for the USB to function.

    @author Stuart Ianna
    @version 0.1
    @date June 2018
    @copyright GNU GPLv3
    @warning Using the USB requires the USB device to be reset in the host OS
    every time the MCU is flashed. This is handled automatically in recipe 
    "make flash". This requires ROOT access. OR the the the usb can be disconnected /
    reconnected.
    @bug When using the USB, the reset button on the MCU doesn't reset the USB
    peripheral. This means the only way to reset is to cycle power on the device.
    @todo

    @par Compilers
    - arm-none-eabi-gcc (15:4.9.3+svn231177-1) 4.9.3 20150529 (prerelease)

    @par Minimum working example
    @include example_usb.c

 * @{
 */

/*! @file stm32f103cb_usb.h
    @brief Header file for stm32f103cb USB
*/

#include <stdint.h>
#include "stm32f103cb_gpio.h"
#include "stm32f103cb_debug.h"
#include "stm32f103cb_power.h"
#include <libopencm3/usb/usbd.h>
#include <libopencm3/usb/cdc.h>
#include <libopencm3/usb/cdc.h>
#include <libopencm3/stm32/f1/bkp.h>
#include <libopencm3/stm32/f1/pwr.h>

/*! 
    @brief Initialize the USB device.
    @details This must be called before any other API feature is used.

    The clock must be > 48Mhz
    This function has a internal delay for the USB to setup (approx 0.3s);
*/
void USB_setup(void);
/*! 
    @brief Update the USB peripheral
    @details This must be call at at least 4kHz.

    This function handles control requests from the host and sends any
    data waiting in the internal software buffer.
*/
void USB_update(void);
/*! 
    @brief Send a single byte on USB
    @details This function actually loads the byte into the internal software
    buffer. The byte is send internally in USB_update().

    @param byte The byte to send (add to buffer).
*/
void USB_put(uint8_t byte);
/*! 
    @brief Set the function called when data is received on the USB
    @details 

    @param data_available Funtion pointer to the function to be called
*/
void USB_setGet(void (*data_available)(uint8_t));
/*! 
    @brief Dissconnect the USB device from the HOST
    @details This is also called internally at the start of USB_setup().
    This allows a software reset to register with the host and the USB to be
    reconfigured.
*/
void USB_disconnect(void);
/**@}*/
/**@}*/
#endif
