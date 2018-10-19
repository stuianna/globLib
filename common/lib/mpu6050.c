/*!
    @file mpu6050.c
    @brief Source file for mpu6050.h
    @author Stuart Ianna
    @version 1.0
    @date June 2018
    @copyright GNU GPLv3
    @warning None
    @bug 

    @details

    @par Verified Compilers
    - arm-none-eabi-gcc (15:4.9.3+svn231177-1) 4.9.3 20150529 (prerelease)
*/

#include "mpu6050.h"

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

uint8_t MPU6050_setup(MPU6050 *target, uint8_t i2c_num, uint8_t address){

    target->bus = i2c_num;
    target->slaveAddress = address;

    if(MPU6050_readByte(target,MPU6050_ADD_WAI) != address){

        return 1;
    }

    //Start up the device
    MPU6050_writeByte(target,MPU6050_ADD_PWR_MGMT_1,0x1); //0x68 = power register, 0x1 = xgyro clock source

    //Get the gyro sensitivity
    target->gyroSen = MPU6050_getGyr(target);

    //Get the accel sensitivity
    target->accelSen = MPU6050_getAcc(target);
    return 0;
}

void MPU6050_calc(MPU6050 *target, mpu_data what, float *container){

    int16_t tempRead;

    switch(what){

        case MPU_ACCX:

            tempRead = MPU6050_readHalf(target,MPU6050_ADD_ACCX);
            *container = (target->accelSen * tempRead)/MPU_DATA_RANGE;
            break;
        case MPU_ACCY:

            tempRead = MPU6050_readHalf(target,MPU6050_ADD_ACCY);
            *container = (target->accelSen * tempRead)/MPU_DATA_RANGE;
            break;
        case MPU_ACCZ:

            tempRead = MPU6050_readHalf(target,MPU6050_ADD_ACCZ);
            *container = (target->accelSen * tempRead)/MPU_DATA_RANGE;
            break;
        case MPU_GYRX:

            tempRead = MPU6050_readHalf(target,MPU6050_ADD_GYRX);
            *container = (target->gyroSen * tempRead)/MPU_DATA_RANGE;
            break;
        case MPU_GYRY:

            tempRead = MPU6050_readHalf(target,MPU6050_ADD_GYRY);
            *container = (target->gyroSen * tempRead)/MPU_DATA_RANGE;
            break;
        case MPU_GYRZ:

            tempRead = MPU6050_readHalf(target,MPU6050_ADD_GYRZ);
            *container = (target->gyroSen * tempRead)/MPU_DATA_RANGE;
            break;
        case MPU_TEMP:

            //Page 30 Datasheet, temperature measurement formula
            tempRead = MPU6050_readHalf(target,MPU6050_ADD_TEMP);
            *container = (tempRead/340.0f) + 36.53f;
            break;
        case MPU_ACC:

            I2C_start(target->bus);
            I2C_address(target->bus,target->slaveAddress,I2C_WRITE);
            I2C_write(target->bus,MPU6050_ADD_ACCX);
            I2C_start(target->bus);
            I2C_address(target->bus,target->slaveAddress,I2C_READ);

            //X-axis accel
            tempRead = (I2C_repeatRead(target->bus) << 8);
            tempRead |= (I2C_repeatRead(target->bus));
            *container++ = (target->accelSen * tempRead)/MPU_DATA_RANGE;

            //Y-axis accel
            tempRead = (I2C_repeatRead(target->bus) << 8);
            tempRead |= (I2C_repeatRead(target->bus));
            *container++ = (target->accelSen * tempRead)/MPU_DATA_RANGE;

            //Z-axis accel
            tempRead = (I2C_repeatRead(target->bus) << 8);
            I2C_stop(target->bus);
            tempRead |= (I2C_read(target->bus));
            *container = (target->accelSen * tempRead)/MPU_DATA_RANGE;

            break;
        case MPU_GYR:

            I2C_start(target->bus);
            I2C_address(target->bus,target->slaveAddress,I2C_WRITE);
            I2C_write(target->bus,MPU6050_ADD_GYRX);
            I2C_start(target->bus);
            I2C_address(target->bus,target->slaveAddress,I2C_READ);

            //X-axis gyro
            tempRead = (I2C_repeatRead(target->bus) << 8);
            tempRead |= (I2C_repeatRead(target->bus));
            *container++ = (target->gyroSen * tempRead)/MPU_DATA_RANGE;

            //Y-axis gyro
            tempRead = (I2C_repeatRead(target->bus) << 8);
            tempRead |= (I2C_repeatRead(target->bus));
            *container++ = (target->gyroSen * tempRead)/MPU_DATA_RANGE;

            //Z-axis gyro
            tempRead = (I2C_repeatRead(target->bus) << 8);
            I2C_stop(target->bus);
            tempRead |= (I2C_read(target->bus));
            *container = (target->gyroSen * tempRead)/MPU_DATA_RANGE;
            break;

        case MPU_ALL:

            I2C_start(target->bus);
            I2C_address(target->bus,target->slaveAddress,I2C_WRITE);
            I2C_write(target->bus,MPU6050_ADD_ACCX);
            I2C_start(target->bus);
            I2C_address(target->bus,target->slaveAddress,I2C_READ);

            //X-axis accel
            tempRead = (I2C_repeatRead(target->bus) << 8);
            tempRead |= (I2C_repeatRead(target->bus));
            *container++ = (target->accelSen * tempRead)/MPU_DATA_RANGE;

            //Y-axis accel
            tempRead = (I2C_repeatRead(target->bus) << 8);
            tempRead |= (I2C_repeatRead(target->bus));
            *container++ = (target->accelSen * tempRead)/MPU_DATA_RANGE;

            //Z-axis accel
            tempRead = (I2C_repeatRead(target->bus) << 8);
            tempRead |= (I2C_repeatRead(target->bus));
            *container++ = (target->accelSen * tempRead)/MPU_DATA_RANGE;

            //Temperature
            tempRead = (I2C_repeatRead(target->bus) << 8);
            tempRead |= (I2C_repeatRead(target->bus));
            *container++ = (tempRead/340.0f) + 36.53f;

            //X-axis gyro
            tempRead = (I2C_repeatRead(target->bus) << 8);
            tempRead |= (I2C_repeatRead(target->bus));
            *container++ = (target->gyroSen * tempRead)/MPU_DATA_RANGE;

            //Y-axis gyro
            tempRead = (I2C_repeatRead(target->bus) << 8);
            tempRead |= (I2C_repeatRead(target->bus));
            *container++ = (target->gyroSen * tempRead)/MPU_DATA_RANGE;

            //Z-axis gyro
            tempRead = (I2C_repeatRead(target->bus) << 8);
            I2C_stop(target->bus);
            tempRead |= (I2C_read(target->bus));
            *container = (target->gyroSen * tempRead)/MPU_DATA_RANGE;
            break;

        case MPU_IMU:

            I2C_start(target->bus);
            I2C_address(target->bus,target->slaveAddress,I2C_WRITE);
            I2C_write(target->bus,MPU6050_ADD_ACCX);
            I2C_start(target->bus);
            I2C_address(target->bus,target->slaveAddress,I2C_READ);

            //X-axis accel
            tempRead = (I2C_repeatRead(target->bus) << 8);
            tempRead |= (I2C_repeatRead(target->bus));
            *container++ = (target->accelSen * tempRead)/MPU_DATA_RANGE;

            //Y-axis accel
            tempRead = (I2C_repeatRead(target->bus) << 8);
            tempRead |= (I2C_repeatRead(target->bus));
            *container++ = (target->accelSen * tempRead)/MPU_DATA_RANGE;

            //Z-axis accel
            tempRead = (I2C_repeatRead(target->bus) << 8);
            tempRead |= (I2C_repeatRead(target->bus));
            *container++ = (target->accelSen * tempRead)/MPU_DATA_RANGE;

            //These two need to be read but not used, don't add to container
            I2C_repeatRead(target->bus);
            I2C_repeatRead(target->bus);

            //X-axis gyro
            tempRead = (I2C_repeatRead(target->bus) << 8);
            tempRead |= (I2C_repeatRead(target->bus));
            *container++ = (target->gyroSen * tempRead)/MPU_DATA_RANGE;

            //Y-axis gyro
            tempRead = (I2C_repeatRead(target->bus) << 8);
            tempRead |= (I2C_repeatRead(target->bus));
            *container++ = (target->gyroSen * tempRead)/MPU_DATA_RANGE;

            //Z-axis gyro
            tempRead = (I2C_repeatRead(target->bus) << 8);
            I2C_stop(target->bus);
            tempRead |= (I2C_read(target->bus));
            *container = (target->gyroSen * tempRead)/MPU_DATA_RANGE;
            break;

        default:
            break;
    }
    return;
}
void MPU6050_raw(MPU6050 *target, mpu_data what, int16_t *container){

    switch(what){

        case MPU_ACCX:

            *container = MPU6050_readHalf(target,MPU6050_ADD_ACCX);
            break;
        case MPU_ACCY:

            *container = MPU6050_readHalf(target,MPU6050_ADD_ACCY);
            break;
        case MPU_ACCZ:

            *container = MPU6050_readHalf(target,MPU6050_ADD_ACCZ);
            break;
        case MPU_GYRX:

            *container = MPU6050_readHalf(target,MPU6050_ADD_GYRX);
            break;
        case MPU_GYRY:

            *container = MPU6050_readHalf(target,MPU6050_ADD_GYRY);
            break;
        case MPU_GYRZ:

            *container = MPU6050_readHalf(target,MPU6050_ADD_GYRZ);
            break;
        case MPU_TEMP:

            *container = MPU6050_readHalf(target,MPU6050_ADD_TEMP);
            break;
        case MPU_ACC:

            I2C_start(target->bus);
            I2C_address(target->bus,target->slaveAddress,I2C_WRITE);
            I2C_write(target->bus,MPU6050_ADD_ACCX);
            I2C_start(target->bus);
            I2C_address(target->bus,target->slaveAddress,I2C_READ);
            *container = (I2C_repeatRead(target->bus) << 8);
            *container++ |= (I2C_repeatRead(target->bus));
            *container = (I2C_repeatRead(target->bus) << 8);
            *container++ |= (I2C_repeatRead(target->bus));
            *container = (I2C_repeatRead(target->bus) << 8);
            I2C_stop(target->bus);
            *container++ |= (I2C_read(target->bus));
            break;
        case MPU_GYR:

            I2C_start(target->bus);
            I2C_address(target->bus,target->slaveAddress,I2C_WRITE);
            I2C_write(target->bus,MPU6050_ADD_GYRX);
            I2C_start(target->bus);
            I2C_address(target->bus,target->slaveAddress,I2C_READ);
            *container = (I2C_repeatRead(target->bus) << 8);
            *container++ |= (I2C_repeatRead(target->bus));
            *container = (I2C_repeatRead(target->bus) << 8);
            *container++ |= (I2C_repeatRead(target->bus));
            *container = (I2C_repeatRead(target->bus) << 8);
            I2C_stop(target->bus);
            *container++ |= (I2C_read(target->bus));

            break;
        case MPU_ALL:

            I2C_start(target->bus);
            I2C_address(target->bus,target->slaveAddress,I2C_WRITE);
            I2C_write(target->bus,MPU6050_ADD_ACCX);
            I2C_start(target->bus);
            I2C_address(target->bus,target->slaveAddress,I2C_READ);
            *container = (I2C_repeatRead(target->bus) << 8);
            *container++ |= (I2C_repeatRead(target->bus));
            *container = (I2C_repeatRead(target->bus) << 8);
            *container++ |= (I2C_repeatRead(target->bus));
            *container = (I2C_repeatRead(target->bus) << 8);
            *container++ |= (I2C_repeatRead(target->bus));
            *container = (I2C_repeatRead(target->bus) << 8);
            *container++ |= (I2C_repeatRead(target->bus));
            *container = (I2C_repeatRead(target->bus) << 8);
            *container++ |= (I2C_repeatRead(target->bus));
            *container = (I2C_repeatRead(target->bus) << 8);
            *container++ |= (I2C_repeatRead(target->bus));
            *container = (I2C_repeatRead(target->bus) << 8);
            I2C_stop(target->bus);
            *container++ |= (I2C_read(target->bus));
            break;

        case MPU_IMU:

            I2C_start(target->bus);
            I2C_address(target->bus,target->slaveAddress,I2C_WRITE);
            I2C_write(target->bus,MPU6050_ADD_ACCX);
            I2C_start(target->bus);
            I2C_address(target->bus,target->slaveAddress,I2C_READ);
            *container = (I2C_repeatRead(target->bus) << 8);
            *container++ |= (I2C_repeatRead(target->bus));
            *container = (I2C_repeatRead(target->bus) << 8);
            *container++ |= (I2C_repeatRead(target->bus));
            *container = (I2C_repeatRead(target->bus) << 8);
            *container++ |= (I2C_repeatRead(target->bus));

            //These two need to be read but not used, simply don't increment
            I2C_repeatRead(target->bus);
            I2C_repeatRead(target->bus);

            *container = (I2C_repeatRead(target->bus) << 8);
            *container++ |= (I2C_repeatRead(target->bus));
            *container = (I2C_repeatRead(target->bus) << 8);
            *container++ |= (I2C_repeatRead(target->bus));
            *container = (I2C_repeatRead(target->bus) << 8);
            I2C_stop(target->bus);
            *container++ |= (I2C_read(target->bus));
            break;

        default:
            break;
    }
    return;
}

void MPU6050_writeByte(MPU6050 *target, uint8_t address, uint8_t data){
      
    I2C_start(target->bus);
    I2C_address(target->bus,target->slaveAddress,I2C_WRITE);
    I2C_write(target->bus,address);
    I2C_write(target->bus,data);
    I2C_stop(target->bus);
    return;
}

uint8_t MPU6050_readByte(MPU6050 *target, uint8_t address){

    I2C_start(target->bus);
    I2C_address(target->bus,target->slaveAddress,I2C_WRITE);
    I2C_write(target->bus,address);
    I2C_start(target->bus);
    I2C_address(target->bus,target->slaveAddress,I2C_READ);
    I2C_stop(target->bus);
    return I2C_read(target->bus);

}

uint16_t MPU6050_readHalf(MPU6050 *target, uint8_t address){

    uint16_t data;

    I2C_start(target->bus);
    I2C_address(target->bus,target->slaveAddress,I2C_WRITE);
    I2C_write(target->bus,address);
    I2C_start(target->bus);
    I2C_address(target->bus,target->slaveAddress,I2C_READ);
    data = (I2C_repeatRead(target->bus) << 8);
    I2C_stop(target->bus);
    return (data | I2C_read(target->bus));
}

void MPU6050_setAcc(MPU6050 *target, mpu_acc range){

    MPU6050_writeByte(target,MPU6050_ADD_ACCEL_CONFIG,range);

    switch (range){

        case MPU_ACC_2G:

            target->accelSen = 2;
            break;
        case MPU_ACC_4G:

            target->accelSen = 4;
            break;
        case MPU_ACC_8G:

            target->accelSen = 8;
            break;
        case MPU_ACC_16G:

            target->accelSen = 16;
            break;
        default: 

            target->accelSen = 2;
            break;
    }
    return;

}

void MPU6050_setGyr(MPU6050 *target, mpu_gyr range){

    MPU6050_writeByte(target,MPU6050_ADD_GYRO_CONFIG,range);

    switch (range){

        case MPU_GYR_250:

            target->gyroSen = 250;
            break;
        case MPU_GYR_500:

            target->gyroSen = 500;
            break;
        case MPU_GYR_1000:

            target->gyroSen = 1000;
            break;
        case MPU_GYR_2000:

            target->gyroSen = 2000;
            break;
        default: 

            target->gyroSen = 250;
            break;
    }
    return;
}

uint8_t MPU6050_getAcc(MPU6050 *target){

    uint8_t data = MPU6050_readByte(target,MPU6050_ADD_ACCEL_CONFIG);

    switch (data){

        case MPU_ACC_2G:

            return 2;
            break;
        case MPU_ACC_4G:

            return 4;
            break;
        case MPU_ACC_8G:

            return 8;
            break;
        case MPU_ACC_16G:

            return 16;
            break;
        default: 

            return 2;
            break;
    }
    return 2;
}

uint16_t MPU6050_getGyr(MPU6050 *target){

    uint8_t data = MPU6050_readByte(target,MPU6050_ADD_GYRO_CONFIG);

    switch (data){

        case MPU_GYR_250:

            return 250;
            break;
        case MPU_GYR_500:

            return 500;
            break;
        case MPU_GYR_1000:

            return 1000;
            break;
        case MPU_GYR_2000:

            return 2000;
            break;
        default: 

            return 250;
            break;
    }
    return 250;
}
