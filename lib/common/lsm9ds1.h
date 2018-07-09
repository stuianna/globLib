/**
    @addtogroup COMMON
* @{
*/

/**
    @addtogroup LSM9DS1
    @brief Routines for interaction with LSM9DS1 9D0F IMU
    @details 

    The LSM9DS1 operates with either SPI or I2C bus. This library curretly
    only supports SPI.

    The device requires two active low chip select pins, one for the 
    magnetometer and the other for the combined gyro and accelerometer.
    These must be configured externally. (see example)

    The maximum updates for each component are
        - Magnetometer 150Hz
        - Accelerometer and gyroscope 950Hz

    LSM9DS1_accReady(), LSM9DS1_gyrReady(), LSM9DS1_magReady() can be used
    to check if data is available when sampling near these frequencies.

    @note This library requires external functions to be declared.
        - For SPI usage
            - SPI_read()
            - SPI_repeatRead()
            - SPI_write()
            - delayms()

    @note Reading from the device using SPI bus has special bit setting 
    requirments. These are handled internally in LSM9DS1_writeByte() and
    LSM9DS1_readByte()

    @author Stuart Ianna
    @version 1.0
    @date June 2018
    @copyright GNU GPLv3
    @warning None
    @bug None
    @todo Add I2C function

    @par Verified Compilers
    - arm-none-eabi-gcc (15:4.9.3+svn231177-1) 4.9.3 20150529 (prerelease)

    @par Example - Typical usage.
    @include example_lsm9ds1.c

 * @{
 */

/*! @file lsm9ds1.h
    @brief Header file for STMicroelectronics LSM9DS1 9DOF IMU.
*/
#ifndef LSM9DS1_H_
#define LSM9DS1_H_
#include <stdio.h>

#define LSM9DS1_DEFAULT_MAG_SCALE LSM9DS1_MAG_8     //!<Default mag range set by LSM9DS1_setup()
#define LSM9DS1_DEFAULT_GYR_SCALE LSM9DS1_GYR_500   //!<Default mag range set by LSM9DS1_setup()
#define LSM9DS1_DEFAULT_ACC_SCALE LSM9DS1_ACC_4     //!<Default mag range set by LSM9DS1_setup()
#define LSM9DS1_DATA_RANGE 32767.0f                 //!<Data range for each axis.
#define LSM9DS1_GX 0                                //!<Used by read and write function to select gyro/accel
#define LSM9DS1_M 1                                 //!<Used by read/write functions to select mag

//Accel/Gyro Registers
#define LSM9DS1_ACT_THS                 0x04
#define LSM9DS1_ACT_DUR                 0x05
#define LSM9DS1_INT_GEN_CFG_XL          0x06
#define LSM9DS1_INT_GEN_THS_X_XL        0x07
#define LSM9DS1_INT_GEN_THS_Y_XL        0x08
#define LSM9DS1_INT_GEN_THS_Z_XL        0x09
#define LSM9DS1_INT_GEN_DUR_XL          0x0A
#define LSM9DS1_REFERENCE_G             0x0B
#define LSM9DS1_INT1_CTRL               0x0C
#define LSM9DS1_INT2_CTRL               0x0D
#define LSM9DS1_WHO_AM_I_XG             0x0F
#define LSM9DS1_CTRL_REG1_G             0x10
#define LSM9DS1_CTRL_REG2_G             0x11
#define LSM9DS1_CTRL_REG3_G             0x12
#define LSM9DS1_ORIENT_CFG_G            0x13
#define LSM9DS1_INT_GEN_SRC_G           0x14
#define LSM9DS1_OUT_TEMP_L              0x15
#define LSM9DS1_OUT_TEMP_H              0x16
#define LSM9DS1_STATUS_REG_0            0x17
#define LSM9DS1_OUT_X_L_G               0x18
#define LSM9DS1_OUT_X_H_G               0x19
#define LSM9DS1_OUT_Y_L_G               0x1A
#define LSM9DS1_OUT_Y_H_G               0x1B
#define LSM9DS1_OUT_Z_L_G               0x1C
#define LSM9DS1_OUT_Z_H_G               0x1D
#define LSM9DS1_CTRL_REG4               0x1E
#define LSM9DS1_CTRL_REG5_XL            0x1F
#define LSM9DS1_CTRL_REG6_XL            0x20
#define LSM9DS1_CTRL_REG7_XL            0x21
#define LSM9DS1_CTRL_REG8               0x22
#define LSM9DS1_CTRL_REG9               0x23
#define LSM9DS1_CTRL_REG10              0x24
#define LSM9DS1_INT_GEN_SRC_XL          0x26
#define LSM9DS1_STATUS_REG_1            0x27
#define LSM9DS1_OUT_X_L_XL              0x28
#define LSM9DS1_OUT_X_H_XL              0x29
#define LSM9DS1_OUT_Y_L_XL              0x2A
#define LSM9DS1_OUT_Y_H_XL              0x2B
#define LSM9DS1_OUT_Z_L_XL              0x2C
#define LSM9DS1_OUT_Z_H_XL              0x2D
#define LSM9DS1_FIFO_CTRL               0x2E
#define LSM9DS1_FIFO_SRC                0x2F
#define LSM9DS1_INT_GEN_CFG_G           0x30
#define LSM9DS1_INT_GEN_THS_XH_G        0x31
#define LSM9DS1_INT_GEN_THS_XL_G        0x32
#define LSM9DS1_INT_GEN_THS_YH_G        0x33
#define LSM9DS1_INT_GEN_THS_YL_G        0x34
#define LSM9DS1_INT_GEN_THS_ZH_G        0x35
#define LSM9DS1_INT_GEN_THS_ZL_G        0x36
#define LSM9DS1_INT_GEN_DUR_G           0x37

//Magneto Registers
#define LSM9DS1_OFFSET_X_REG_L_M        0x05
#define LSM9DS1_OFFSET_X_REG_H_M        0x06
#define LSM9DS1_OFFSET_Y_REG_L_M        0x07
#define LSM9DS1_OFFSET_Y_REG_H_M        0x08
#define LSM9DS1_OFFSET_Z_REG_L_M        0x09
#define LSM9DS1_OFFSET_Z_REG_H_M        0x0A
#define LSM9DS1_WHO_AM_I_M              0x0F
#define LSM9DS1_CTRL_REG1_M             0x20
#define LSM9DS1_CTRL_REG2_M             0x21
#define LSM9DS1_CTRL_REG3_M             0x22
#define LSM9DS1_CTRL_REG4_M             0x23
#define LSM9DS1_CTRL_REG5_M             0x24
#define LSM9DS1_STATUS_REG_M            0x27
#define LSM9DS1_OUT_X_L_M               0x28
#define LSM9DS1_OUT_X_H_M               0x29
#define LSM9DS1_OUT_Y_L_M               0x2A
#define LSM9DS1_OUT_Y_H_M               0x2B
#define LSM9DS1_OUT_Z_L_M               0x2C
#define LSM9DS1_OUT_Z_H_M               0x2D
#define LSM9DS1_INT_CFG_M               0x30
#define LSM9DS1_INT_SRC_M               0x31
#define LSM9DS1_INT_THS_L_M             0x32
#define LSM9DS1_INT_THS_H_M             0x33

/*! 
    @brief Data structure containing variables specific to individual 
    slave devices
    @details These values are set by LSM9DS1_setup() and do not need to 
    be changed manually.
*/
typedef struct{

    uint8_t port;       //!<The port the device is connected to
    uint32_t gx_pin;    //!<The pin used for gyro/accel SPI chip select
    uint16_t gx_port;   //!<The port used for gyro/accel SPI chip select
    uint32_t m_pin;     //!<The pin used for mag SPI chip select
    uint16_t m_port;    //!<The port used for mag SPI chip select
    uint8_t cs_pol;     //!<The polary of the SPI chip select line
}LSM9DS1;

/*! 
    @brief Available magnetometer ranges
*/
typedef enum{

    LSM9DS1_MAG_4 = 4,  //!<+-4 Gauss range
    LSM9DS1_MAG_8 = 8,  //!<+-8 Gauss range
    LSM9DS1_MAG_12 = 12,//!<+-12 Gauss range
    LSM9DS1_MAG_16 = 16,//!<+-16 Gauss range

}lsm9ds1_magScale;

/*! 
    @brief Available accelerometer ranges
*/
typedef enum{

    LSM9DS1_ACC_2 = 2,  //!<+-2G range
    LSM9DS1_ACC_4 = 4,  //!<+-4G range
    LSM9DS1_ACC_8 = 8,  //!<+-8G range
    LSM9DS1_ACC_16 = 16,//!<+-16G range

}lsm9ds1_accScale;

/*! 
    @brief Available gyroscope ranges
*/
typedef enum{

    LSM9DS1_GYR_245 = 245,  //!<+-245 degrees / second range
    LSM9DS1_GYR_500 = 500,  //!<+-500 degrees / second range
    LSM9DS1_GYR_2000 = 2000,//!<+-2000 degrees / second range

}lsm9ds1_gyrScale;

/*! 
    @brief Initalizes the slave device and populates data structure variable
    @note The relavent SPI port and GPIO pins must be setup before this is called
    @param target Pointer to the LSM9DS1 data structure
    @param spiPort The number of the peripheral port connected, typically SPI_1 or SPI_2
    @param gx_port The GPIO port which the gyro/accel chip select pin is connected
    @param gx_pin The GPIO pin which the gyro/accel chip select pin is connected
    @param m_port The GPIO port which the magneto chip select pin is connected
    @param m_pin The GPIO pin which the magneto chip select pin is connected
    @param cs_pol The polarity of the chip select pin. This must be active low (SPI_CS_LOW)
*/
void LSM9DS1_setup(LSM9DS1 *target, uint8_t spiPort,uint16_t gx_port, 
        uint32_t gx_pin, uint16_t m_port, uint32_t m_pin, uint8_t cs_pol);
/*! 
    @brief Check if the magnetometer has new data available
    @warning The device will return rubish if new data isn't ready.
    @param target Pointer to the LSM9DS1 data structure
    @return 
        - 0 if data is not ready
        - non-zero if data is ready
*/
uint8_t LSM9DS1_magReady(LSM9DS1 *target);
/*! 
    @brief Get the latest magnometer readings
    @warning data must be of size 3 to avoid memory fault.
    This function can be called at a maximum of 150Hz
    @param target Pointer to the LSM9DS1 data structure
    @param data Container for the new data. Must be of size 3.
    @return New readings are retured in data.
*/
void LSM9DS1_getMag(LSM9DS1 *target,int16_t *data);
/*! 
    @brief Check if the gyroscope has new data available
    @warning The device will return rubish if new data isn't ready.
    @param target Pointer to the LSM9DS1 data structure
    @return 
        - 0 if data is not ready
        - non-zero if data is ready
*/
uint8_t LSM9DS1_gyrReady(LSM9DS1 *target);
/*! 
    @brief Get the latest gyroscope readings
    @warning data must be of size 3 to avoid memory fault
    This function can be called at a maximum of 950Hz
    @param target Pointer to the LSM9DS1 data structure
    @param data Container for the new data. Must be of size 3.
    @return New readings are retured in data.
*/
void LSM9DS1_getGyr(LSM9DS1 *target,int16_t *data);
/*! 
    @brief Check if the accelerometer has new data available
    @warning The device will return rubish if new data isn't ready.
    @param target Pointer to the LSM9DS1 data structure
    @return 
        - 0 if data is not ready
        - non-zero if data is ready
*/
uint8_t LSM9DS1_accReady(LSM9DS1 *target);
/*! 
    @brief Get the latest accelerometer readings
    @warning data must be of size 3 to avoid memory fault
    This function can be called at a maximum of 950Hz
    @param target Pointer to the LSM9DS1 data structure
    @param data Container for the new data. Must be of size 3.
    @return New readings are retured in data.
*/
void LSM9DS1_getAcc(LSM9DS1 *target,int16_t *data);
/*! 
    @brief Check if the temperature module has new data available
    @warning The device will return rubish if new data isn't ready.
    @param target Pointer to the LSM9DS1 data structure
    @return 
        - 0 if data is not ready
        - non-zero if data is ready
*/
uint8_t LSM9DS1_tempReady(LSM9DS1 *target);
/*! 
    @brief Get the latest accelerometer readings
    @note Update frequency for this module is not specified,
    tested ok at 150Hz
    @param target Pointer to the LSM9DS1 data structure
    @return The temperature converted to degrees celcius
*/
float LSM9DS1_getTemp(LSM9DS1 *target);
/*! 
    @brief Read the value for the gyro/accel who am I register.
    @details This function can be used to test the device connection.
    @param target Pointer to the LSM9DS1 data structure
    @return A correct reading will be 104 (decimal)
*/
uint8_t LSM9DS1_gxWAI(LSM9DS1 *target);
/*! 
    @brief Read the value for the magnetometer who am I register.
    @details This function can be used to test the device connection.
    @param target Pointer to the LSM9DS1 data structure
    @return A correct reading will be 61 (decimal) (0x3D hex!)
*/
uint8_t LSM9DS1_mWAI(LSM9DS1 *target);
/*! 
    @brief Set the scale of the magnetometer
    @param target Pointer to the LSM9DS1 data structure
    @param scale Magnometer scale defined by #lsm9ds1_magScale
*/
void LSM9DS1_setMagScale(LSM9DS1 *target, lsm9ds1_magScale scale); 
/*! 
    @brief Set the scale of the accelerometer
    @param target Pointer to the LSM9DS1 data structure
    @param scale Accelerometer scale defined by #lsm9ds1_accScale
*/
void LSM9DS1_setAccScale(LSM9DS1 *target, lsm9ds1_accScale scale); 
/*! 
    @brief Set the scale of the gyroscope
    @param target Pointer to the LSM9DS1 data structure
    @param scale Gyroscope scale defined by #lsm9ds1_gyrScale
*/
void LSM9DS1_setGyrScale(LSM9DS1 *target, lsm9ds1_gyrScale scale); 
/*! 
    @brief Get the scale of the magnetometer
    @param target Pointer to the LSM9DS1 data structure
    @return Integer value representing the scale
*/
uint8_t LSM9DS1_getMagScale(LSM9DS1 *target); 
/*! 
    @brief Get the scale of the accelerometer
    @param target Pointer to the LSM9DS1 data structure
    @return Integer value representing the scale
*/
uint8_t LSM9DS1_getAccScale(LSM9DS1 *target); 
/*! 
    @brief Get the scale of the gyroscope
    @param target Pointer to the LSM9DS1 data structure
    @return Integer value representing the scale
*/
uint16_t LSM9DS1_getGyrScale(LSM9DS1 *target); 
/*! 
    @brief Write a single byte to the device's register
    @param target Pointer to the LSM9DS1 data structure
    @param component LSM9DS1_GX or LSM9DS1_M depending a on the register address.
    @param address The device address to write to
    @param data The data to write
*/
void LSM9DS1_writeByte(LSM9DS1 *target, uint8_t component, uint8_t address, uint8_t data);
/*! 
    @brief Read a single byte from the device's register
    @param target Pointer to the LSM9DS1 data structure
    @param component LSM9DS1_GX or LSM9DS1_M depending a on the register address.
    @param address The device address to read from 
    @return The byte of data located at that address
    @note 16-bit data type is return for compatability with SPI peripheral library.
*/
uint16_t LSM9DS1_readByte(LSM9DS1 *target, uint8_t component,uint8_t address);

#endif

