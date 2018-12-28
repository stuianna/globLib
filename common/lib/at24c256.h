/**
    @addtogroup COMMON
* @{
*/

/**
    @addtogroup AT24C256
    @brief Routines for interaction with Atmel EEPROM AT24C256
    @details 

    The AT24C256 operates on the I2C bus. This module provides routines for

    @note This library requires external functions to be declared.
        - I2C_start()
        - I2C_address()
        - I2C_write()
        - I2C_read()
        - I2C_repeatRead()
        - I2C_stop()


    @author Stuart Ianna
    @version 0.1
    @date December 2018
    @copyright GNU GPLv3
    @warning This module is incomplete
    @bug None
    @todo
        

    @par Verified Compilers
    - arm-none-eabi-gcc (15:4.9.3+svn231177-1) 4.9.3 20150529 (prerelease)

    @par Example - Typical usage.
    @include example_at24c256.c

 * @{
 */

/*! @file at24c256.h
    @brief Header file for Atmel EEPROM AT24C256
*/
#ifndef AT24C256_H_
#define AT24C256_H_
#include <stdio.h>

/*! 
    @brief Standard slave address for the module
*/
#define AT24C256_DEVICE_ADDRESS 0xA0
#define AT24C256_DEVICE_ADDRESS_ALT_1 0xA2
#define AT24C256_DEVICE_ADDRESS_ALT_2 0xA4
#define AT24C256_DEVICE_ADDRESS_ALT_3 0xA6

#define AT24C256_PAGE_SIZE 64
#define AT24C256_PAGE_NUMBER 512
#define AT24C256_BYTE_NUMBER AT24C256_PAGE_SIZE*AT24C256_PAGE_NUMBER


typedef struct{

    /*! 
        @brief The I2C bus which the device is connected to
    */
    uint8_t bus;
    uint8_t address;

}AT24C256;


/*! 
    @brief Initalizes the slave device and populates data structure variable
    @note The relavent I2C port must be setup before this is called

    @param target Pointer to the MPU6050 data structure
    @param i2c_number The number of the peripheral port connected, typically I2C_1 or I2C_2
    @param address The slave address of the device MPU_DEVICE_ADDRESS or MPU_DEVICE_ADDRESS_ALTERNATE
    @return 0 if setup was ok, 1 if error occured.
*/
void AT24C256_setup(AT24C256 *target, uint8_t i2c_number, uint8_t address);

void AT24C256_writeByte(AT24C256 *target, uint16_t address, uint8_t byte);

uint8_t AT24C256_readByte(AT24C256 *target, uint16_t address);

/**@}*/
/**@}*/
#endif

