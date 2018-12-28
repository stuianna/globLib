/*!
    @file at24c256.c
    @brief Source file for at24c256.h
    @author Stuart Ianna
    @version 0.1
    @date December 2018
    @copyright GNU GPLv3
    @warning None
    @bug 

    @details

    @par Verified Compilers
    - arm-none-eabi-gcc (15:4.9.3+svn231177-1) 4.9.3 20150529 (prerelease)
*/

#include "at24c256.h"

//This is not ideal
extern uint8_t I2C_start(uint8_t i2c_address);
extern uint8_t I2C_address(uint8_t i2c_address, uint8_t address, uint8_t operation);
extern uint8_t I2C_write(uint8_t i2c_address, uint8_t byte);
extern uint8_t I2C_read(uint8_t i2c_address);
extern uint8_t I2C_repeatRead(uint8_t i2c_address);
extern uint8_t I2C_stop(uint8_t i2c_address);

//This is less ideal
#define I2C_WRITE 0
#define I2C_READ 1

void AT24C256_setup(AT24C256 *target, uint8_t i2c_num, uint8_t address){

    target->bus = i2c_num;
    target->address = address;
}

void AT24C256_writeByte(AT24C256 *target, uint16_t address, uint8_t byte){

    I2C_start(target->bus);
    I2C_address(target->bus,target->address,I2C_WRITE);
    I2C_write(target->bus,(uint8_t)(address >> 8));
    I2C_write(target->bus,(uint8_t)(address));
    I2C_write(target->bus,(uint8_t)(byte));
    I2C_stop(target->bus);
}

uint8_t AT24C256_readByte(AT24C256 *target, uint16_t address){

    uint8_t byte;
    I2C_start(target->bus);
    I2C_address(target->bus,target->address,I2C_WRITE);
    I2C_write(target->bus,(uint8_t)(address >> 8));
    I2C_write(target->bus,(uint8_t)(address));
    I2C_start(target->bus);
    I2C_address(target->bus,target->address,I2C_READ);
    I2C_stop(target->bus);
    byte =  I2C_read(target->bus);
    return byte;
}
