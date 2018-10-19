#ifndef STM32F103CB_I2C_H_
#define STM32F103CB_I2C_H_
/**
    @addtogroup STM32F103CB 
* @{
*/

/**
    @addtogroup I2C
    @brief This module contains low level functions for I2C interaction.
    @details The I2C peripherals are mapped to the following pins for the ports

    - I2C1:
             + SCL - PB6
             + SDA - PB7
    - I2C2:
             + SCL - PB10
             + SDA - PB11

    PULLUP RESISTORS (AROUND 4.7k) ARE NEEDED ON THESE LINES

    I2C_setup() sets up a peripheral port with the following:

    - Fast mode
    - Slave address defined
    - 7-bit address mode

    @author Stuart Ianna
    @version 0.1
    @date June 2018
    @copyright GNU GPLv3
    @warning None
    @bug None
    @todo

    @par Compilers
    - arm-none-eabi-gcc (15:4.9.3+svn231177-1) 4.9.3 20150529 (prerelease)

    @par Minimum working example (blocking)
    @include example_i2c.c

 * @{
 */

/*! @file stm32f103cb_i2c.h
    @brief Header file for stm32f103cb I2C
*/

#include "stm32f103cb_gpio.h"
#include "stm32f103cb_debug.h"
#include <libopencm3/stm32/i2c.h>
#include <stdint.h>

/*! 
    @brief The number of loop cycles to wait when polling peripheral registers.
*/
#define I2C_TIMEOUT 500000

/*! 
    @brief The address of this device.
*/
#define I2C_MY_ADDRESS 0x32

/*! 
    @brief I2C ports available on the MCU.
    @details 
*/
typedef enum{

    I2C_1,          //!<First I2C port.
    I2C_2,          //!<Second I2C port.
}i2c_periph;


/*! 
    @brief Initialize I2C data structure and setup port.
    @details This must be called before any other feature is used.

    @param peripheral I2C peripheral to associate with the port.
    @return Error number as defined in #mcu_error, E_I2C_NOERROR if no error, E_I2C_PORT if port doesn't exist
*/
mcu_error I2C_setup(i2c_periph peripheral);
/*! 
    @brief Generate a start condition on the bus.
    @details 

    @param peripheral I2C peripheral to modify.
    @return Error number as defined in #mcu_error, E_I2C_NOERROR if no error, E_I2C_START if start wasn't generated.
*/
mcu_error I2C_start(i2c_periph peripheral);
/*! 
    @brief Generate a stop condition on the bus.
    @details 

    @param peripheral I2C peripheral to modify.
    @return Error number as defined in #mcu_error, E_I2C_NOERROR if no error, E_I2C_STOP if stop wasn't generated.
*/
mcu_error I2C_stop(i2c_periph peripheral);
/*! 
    @brief Send a seven bit address on the bus.
    @details 

    @param peripheral I2C peripheral to modify.
    @param address The slave address of the device
    @param operation read or write operation
    @return Error number as defined in #mcu_error, E_I2C_NOERROR if no error, E_I2C_ADD if address wasn't sent.
*/
mcu_error I2C_address(i2c_periph peripheral, uint8_t address, uint8_t operation);
/*! 
    @brief Send one byte on the bus.
    @details 

    @param peripheral I2C peripheral to modify.
    @param byte The byte to send.
    @return Error number as defined in #mcu_error, E_I2C_NOERROR if no error, E_I2C_WRITE if byte wasn't transmitted.
*/
mcu_error I2C_write(i2c_periph peripheral, uint8_t byte);
/*! 
    @brief Read one byte from the bus without acknowledge generation.
    @details This operation will stil return mcu_error if an error has occured. The generated error will also be streamed
    on the debug output stream.

    This is typically used at the end of a read chain, or when only a sigle byte is read.

    @param peripheral I2C peripheral to modify.
    @return Error number as defined in #mcu_error, E_I2C_NOERROR if no error, E_I2C_WRITE if byte wasn't transmitted.
*/
uint8_t I2C_read(i2c_periph peripheral);
/*! 
    @brief Read one byte from the bus with acknowledge generation.
    @details This operation will stil return mcu_error if an error has occured. The generated error will also be streamed
    on the debug output stream.

    This is typically used when another read operation is to follow.

    @param peripheral I2C peripheral to modify.
    @return Error number as defined in #mcu_error, E_I2C_NOERROR if no error, E_I2C_WRITE if byte wasn't transmitted.
*/
uint8_t I2C_repeatRead(i2c_periph peripheral);

/**@}*/
/**@}*/
#endif
