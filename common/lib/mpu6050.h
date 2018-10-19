/**
    @addtogroup COMMON
* @{
*/

/**
    @addtogroup MPU6050
    @brief Routines for interaction with MPU6050 IMU
    @details 

    The MPU6060 operates on the I2C bus. This module provides routines for

    @note This library requires external functions to be declared.
        - I2C_start()
        - I2C_address()
        - I2C_write()
        - I2C_read()
        - I2C_repeatRead()
        - I2C_stop()


    @author Stuart Ianna
    @version 1.0
    @date June 2018
    @copyright GNU GPLv3
    @warning None
    @bug None
    @todo
        

    @par Verified Compilers
    - arm-none-eabi-gcc (15:4.9.3+svn231177-1) 4.9.3 20150529 (prerelease)

    @par Example - Typical usage.
    @include example_mpu6050.c

 * @{
 */

/*! @file mpu6050.h
    @brief Header file for Invensence IMU MPU6050.
*/
#ifndef MPU6050_H_
#define MPU6050_H_
#include <stdio.h>

/*! 
    @brief Standard slave address for the module (ADD -> GND).
*/
#define MPU_DEVICE_ADDRESS 0x68
/*! 
    @brief Altenate slave address for the module (ADD -> VCC).
*/
#define MPU_DEVICE_ADDRESS_ALTERNATE 0x69
/*! 
    @brief The decimal rangle of the ADC (15-bit <=> 32767).
*/
#define MPU_DATA_RANGE 32767.0f

/*! 
    @brief Data structure containing variables specific to individual slave devices
*/
typedef struct{

    /*! 
        @brief The port the device is connected to.
    */
    uint8_t bus;
    /*! 
        @brief The address of the slave device
    */
    uint8_t slaveAddress;
    /*! 
        @brief The accelerometer sesitivity, set during MPU6050_setup() by reading slave data register
    */
    uint8_t accelSen;
    /*! 
        @brief The gyroscope sesitivity, set during MPU6050_setup() by reading slave data register
    */
    uint16_t gyroSen;
}MPU6050;

/*! 
    @brief Different options for getting data with MPU6050_raw() and MPU6050_calc()
*/
typedef enum{

    MPU_ACCX,       //!<Acceleration data x-axis
    MPU_ACCY,       //!<Acceleration data y-axis
    MPU_ACCZ,       //!<Acceleration data z-axis
    MPU_TEMP,       //!<Temperature data
    MPU_GYRX,       //!<Gyrometer data x-axis
    MPU_GYRY,       //!<Gyrometer data y-axis
    MPU_GYRZ,       //!<Gyrometer data z-axis
    MPU_ACC,        //!<All raw accelerometer data
    MPU_GYR,        //!<All raw gyro data
    MPU_IMU,        //!<All gyro and all accelermeter
    MPU_ALL,        //!<All raw data

}mpu_data;

typedef enum{

    MPU_ACC_2G   =  0x00,     //!< 2G range for accelerometer
    MPU_ACC_4G   =  0x08,     //!< 4G range for accelerometer
    MPU_ACC_8G   =  0x10,     //!< 8G range for accelerometer
    MPU_ACC_16G  =  0x18,     //!< 16G range for accelerometer

}mpu_acc;

typedef enum{

    MPU_GYR_250   = 0x00,   //!< 250 degree range for gyroscope
    MPU_GYR_500   = 0x08,   //!< 500 degree range for gyroscope
    MPU_GYR_1000  = 0x10,   //!< 1000 degree range for gyroscope
    MPU_GYR_2000  = 0x18,   //!< 2000 degree range for gyroscope

}mpu_gyr;


//Device register addresses
#define MPU6050_ADD_ACCX 0x3B
#define MPU6050_ADD_ACCY 0x3D
#define MPU6050_ADD_ACCZ 0x3F
#define MPU6050_ADD_TEMP 0x41
#define MPU6050_ADD_GYRX 0x43
#define MPU6050_ADD_GYRY 0x45
#define MPU6050_ADD_GYRZ 0x47
#define MPU6050_ADD_WAI  0x75
#define MPU6050_ADD_PWR_MGMT_1  0x6B
#define MPU6050_ADD_ACCEL_CONFIG  0x1C
#define MPU6050_ADD_GYRO_CONFIG  0x1B

/*! 
    @brief Initalizes the slave device and populates data structure variable
    @note The relavent I2C port must be setup before this is called

    @param target Pointer to the MPU6050 data structure
    @param i2c_number The number of the peripheral port connected, typically I2C_1 or I2C_2
    @param address The slave address of the device MPU_DEVICE_ADDRESS or MPU_DEVICE_ADDRESS_ALTERNATE
    @return 0 if setup was ok, 1 if error occured.
*/
uint8_t MPU6050_setup(MPU6050 *target, uint8_t i2c_number, uint8_t address);
/*! 
    @brief Writes a single byte to a given address
    @details 

    This function can be used to modify individual registers of the device.

    @param target the target slave device
    @param address The target register address of the slave
    @param data The byte of data to write
    @return none
*/
void MPU6050_writeByte(MPU6050 *target, uint8_t address, uint8_t data);
/*! 
    @brief Read a single byte from a given address
    @details 

    This function can be used to read individual values of the device registers

    @param target The target slave device
    @param address The target register address of the slave
    @return The data at that address.
*/
uint8_t MPU6050_readByte(MPU6050 *target, uint8_t address);
/*! 
    @brief Read a half word from a given address
    @details 


    @param target The target slave device
    @param address The target register address for the first register target
    @return The data at that address.
*/
uint16_t MPU6050_readHalf(MPU6050 *target, uint8_t address);
/*! 
    @brief Get raw data values (16-bit each)
    @details 

    @param target The target slave device
    @param what The type of data to get 
    @param container 
    @return none
*/
void MPU6050_raw(MPU6050 *target, mpu_data what, int16_t *container);
/*! 
    @brief Get calculated data values 
    @details 

    This function calculates G-force and degrees per second using the sensitivity
    values defined in data structure MPU6050.

    @param target The target slave device
    @param what The type of data to get 
    @param container 
    @return none
*/
void MPU6050_calc(MPU6050 *target, mpu_data what, float *container);
/*! 
    @brief Set acceleration range
    @details 

    @param target The target slave device
    @param range The range to set 
    @return none
*/
void MPU6050_setAcc(MPU6050 *target, mpu_acc range);
/*! 
    @brief Set gyroscope range
    @details 

    @param target The target slave device
    @param range The range to set 
    @return none
*/
void MPU6050_setGyr(MPU6050 *target, mpu_gyr range);
/*! 
    @brief Get the acceleration range
    @details 

    @param target The target slave device
    @return The range for the accelerometer
*/
uint8_t MPU6050_getAcc(MPU6050 *target);
/*! 
    @brief Get the gyroscope range
    @details 

    @param target The target slave device
    @return The range for the gyroscope
*/
uint16_t MPU6050_getGyr(MPU6050 *target);
/**@}*/
/**@}*/
#endif

