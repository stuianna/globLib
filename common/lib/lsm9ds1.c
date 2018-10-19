/*!
    @file lsm9ds1.c
    @brief Source file for lsm9ds1.h
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

#include "lsm9ds1.h"

//From SPI library

extern uint16_t SPI_read(uint8_t,uint16_t,uint32_t,uint8_t,uint16_t);
extern void SPI_repeatRead(uint8_t,uint16_t,uint32_t,uint8_t,uint16_t,uint16_t*,uint16_t);
extern void SPI_write(uint8_t,uint16_t,uint32_t,uint8_t,uint16_t,uint16_t);

//From Core library
extern void delayms(uint32_t);

void lsm9ds1_magSetup(LSM9DS1 *target);
void lsm9ds1_gyrSetup(LSM9DS1 *target);
void lsm9ds1_accSetup(LSM9DS1 *target);

void LSM9DS1_setup(LSM9DS1 *target,uint8_t spiPort,uint16_t gx_port, 
        uint32_t gx_pin,uint16_t m_port, uint32_t m_pin, uint8_t cs_pol){

    //The chip needs a delay for setup, provide it here
    delayms(10);

    target->port = spiPort;
    target->gx_pin = gx_pin;
    target->gx_port = gx_port;
    target->m_pin = m_pin;
    target->m_port = m_port;
    target->cs_pol = cs_pol;

    lsm9ds1_gyrSetup(target);
    lsm9ds1_accSetup(target);
    lsm9ds1_magSetup(target);

    return;
}

void lsm9ds1_accSetup(LSM9DS1 *target){

    //Set maximum data rate, 
    SPI_write(target->port,target->gx_port,
            target->gx_pin,target->cs_pol,LSM9DS1_CTRL_REG6_XL, 
            0xC0);

    LSM9DS1_setAccScale(target,LSM9DS1_DEFAULT_ACC_SCALE);
            
}

void lsm9ds1_gyrSetup(LSM9DS1 *target){

    //Set maximum data rate and highest low pass filter spec.
    SPI_write(target->port,target->gx_port,
            target->gx_pin,target->cs_pol,LSM9DS1_CTRL_REG1_G, 
            0xC3);

    LSM9DS1_setGyrScale(target,LSM9DS1_DEFAULT_GYR_SCALE);

    //Output data comes after high and low pass filters
    SPI_write(target->port,target->gx_port,
            target->gx_pin,target->cs_pol,LSM9DS1_CTRL_REG2_G, 
            0x02);

    //High pass filter enable, cutoff = 1Hz (this also depends on the output data rate)
    SPI_write(target->port,target->gx_port,
            target->gx_pin,target->cs_pol,LSM9DS1_CTRL_REG3_G, 
            0x46);

}

void lsm9ds1_magSetup(LSM9DS1 *target){
   
    //Set ultra-high performace, temperature compensation, self test, fast and maximum output data rate
    SPI_write(target->port,target->m_port,
            target->m_pin,target->cs_pol,LSM9DS1_CTRL_REG1_M, 
            0xFF);

    //Set default scale
    LSM9DS1_setMagScale(target,LSM9DS1_DEFAULT_MAG_SCALE);

    //Set continuous conversion
    SPI_write(target->port,target->m_port,
            target->m_pin,target->cs_pol,LSM9DS1_CTRL_REG3_M, 
            0x00);

    //Set Z axis ultra high performane and little endian data formate
    SPI_write(target->port,target->m_port,
            target->m_pin,target->cs_pol,LSM9DS1_CTRL_REG4_M, 
            0x0C);
    return;
}

void LSM9DS1_setMagScale(LSM9DS1 *target, lsm9ds1_magScale scale){

    uint8_t regValue;
    regValue =  SPI_read(target->port,target->m_port,
            target->m_pin,target->cs_pol,LSM9DS1_CTRL_REG2_M | 0x80);

    //Clear existing value
    regValue &= ~0x60;

    switch(scale){

        case LSM9DS1_MAG_4:

            SPI_write(target->port,target->m_port,
                    target->m_pin,target->cs_pol,LSM9DS1_CTRL_REG2_M, 
                    regValue);
            break;

        case LSM9DS1_MAG_8:

            SPI_write(target->port,target->m_port,
                    target->m_pin,target->cs_pol,LSM9DS1_CTRL_REG2_M, 
                    (regValue | 0x20));
            break;

        case LSM9DS1_MAG_12:

            SPI_write(target->port,target->m_port,
                    target->m_pin,target->cs_pol,LSM9DS1_CTRL_REG2_M, 
                    (regValue | 0x40));
            break;

        case LSM9DS1_MAG_16:

            SPI_write(target->port,target->m_port,
                    target->m_pin,target->cs_pol,LSM9DS1_CTRL_REG2_M, 
                    (regValue | 0x60));
            break;

        default:
            //Do nothing
            break;
    }

}


void LSM9DS1_setAccScale(LSM9DS1 *target, lsm9ds1_accScale scale){

    uint8_t regValue;
    regValue =  SPI_read(target->port,target->gx_port,
            target->gx_pin,target->cs_pol,LSM9DS1_CTRL_REG6_XL | 0x80);

    //Clear existing value
    regValue &= ~0x18;

    switch(scale){

        case LSM9DS1_ACC_2:

            SPI_write(target->port,target->gx_port,
                    target->gx_pin,target->cs_pol,LSM9DS1_CTRL_REG6_XL, 
                    regValue);
            break;

        case LSM9DS1_ACC_4:

            SPI_write(target->port,target->gx_port,
                    target->gx_pin,target->cs_pol,LSM9DS1_CTRL_REG6_XL, 
                    (regValue | 0x10));
            break;

        case LSM9DS1_ACC_8:

            SPI_write(target->port,target->gx_port,
                    target->gx_pin,target->cs_pol,LSM9DS1_CTRL_REG6_XL, 
                    (regValue | 0x18));
            break;

        case LSM9DS1_ACC_16:

            SPI_write(target->port,target->gx_port,
                    target->gx_pin,target->cs_pol,LSM9DS1_CTRL_REG6_XL, 
                    (regValue | 0x08));
            break;

        default:
            //Do nothing
            break;
    }
}

void LSM9DS1_setGyrScale(LSM9DS1 *target, lsm9ds1_gyrScale scale){

    uint8_t regValue;
    regValue =  SPI_read(target->port,target->gx_port,
            target->gx_pin,target->cs_pol,LSM9DS1_CTRL_REG1_G | 0x80);

    //Clear existing value
    regValue &= ~0x18;

    switch(scale){

        case LSM9DS1_GYR_245:

            SPI_write(target->port,target->gx_port,
                    target->gx_pin,target->cs_pol,LSM9DS1_CTRL_REG1_G, 
                    regValue);
            break;

        case LSM9DS1_GYR_500:

            SPI_write(target->port,target->gx_port,
                    target->gx_pin,target->cs_pol,LSM9DS1_CTRL_REG1_G, 
                    (regValue |= 0x08));
            break;

        case LSM9DS1_GYR_2000:

            SPI_write(target->port,target->gx_port,
                    target->gx_pin,target->cs_pol,LSM9DS1_CTRL_REG1_G, 
                    (regValue |= 0x18));
            break;
        default:
            //Do nothing
            break;
    }
}

uint8_t LSM9DS1_getMagScale(LSM9DS1 *target){

    uint8_t regValue;
    regValue =  SPI_read(target->port,target->m_port,
            target->m_pin,target->cs_pol,LSM9DS1_CTRL_REG2_M | 0x80);

    switch(regValue & 0x60){

        case 0:

            return LSM9DS1_MAG_4;
            break;

        case 0x20:

            return LSM9DS1_MAG_8;
            break;

        case 0x40:

            return LSM9DS1_MAG_12;
            break;

        case 0x60:

            return LSM9DS1_MAG_16;
            break;
        default:
            break;
    }

        return 0;

}

uint8_t LSM9DS1_getAccScale(LSM9DS1 *target){

    uint8_t regValue;
    regValue =  SPI_read(target->port,target->gx_port,
            target->gx_pin,target->cs_pol,LSM9DS1_CTRL_REG6_XL | 0x80);

    switch(regValue & 0x18){

        case 0:

            return LSM9DS1_ACC_2;
            break;

        case 0x08:

            return LSM9DS1_ACC_16;
            break;

        case 0x10:

            return LSM9DS1_ACC_4;
            break;

        case 0x18:

            return LSM9DS1_ACC_8;
            break;
        default:
            break;
    }

        return 0;
}

uint16_t LSM9DS1_getGyrScale(LSM9DS1 *target){

    uint8_t regValue;
    regValue =  SPI_read(target->port,target->gx_port,
            target->gx_pin,target->cs_pol,LSM9DS1_CTRL_REG1_G | 0x80);

    switch(regValue & 0x18){

        case 0:

            return LSM9DS1_GYR_245;
            break;

        case 0x08:

            return LSM9DS1_GYR_500;
            break;

        case 0x18:

            return LSM9DS1_GYR_2000;
            break;
        default:
            break;
    }

        return 0;
}

uint8_t LSM9DS1_magReady(LSM9DS1 *target){

    uint8_t regValue;
    regValue =  SPI_read(target->port,target->m_port,
            target->m_pin,target->cs_pol,LSM9DS1_STATUS_REG_M | 0x80);

    return (regValue & 0x08);
}

void LSM9DS1_getMag(LSM9DS1 *target,int16_t *data){

    uint16_t newData[6];

    //Continuous read of magneto requries first two bits set
    SPI_repeatRead(target->port,target->m_port,
            target->m_pin,target->cs_pol,LSM9DS1_OUT_X_L_M | 0xC0,newData,6);

    data[0] = (newData[1] << 8) | newData[0];
    data[1] = (newData[3] << 8) | newData[2];
    data[2] = (newData[5] << 8) | newData[4];
}

uint8_t LSM9DS1_gyrReady(LSM9DS1 *target){

    uint8_t regValue;
    regValue =  SPI_read(target->port,target->gx_port,
            target->gx_pin,target->cs_pol,LSM9DS1_STATUS_REG_0 | 0x80);

    return (regValue & 0x02);
}

void LSM9DS1_getGyr(LSM9DS1 *target,int16_t *data){

    uint16_t newData[6];

    SPI_repeatRead(target->port,target->gx_port,
            target->gx_pin,target->cs_pol,LSM9DS1_OUT_X_L_G | 0x80,newData,6);

    data[0] = (newData[1] << 8) | newData[0];
    data[1] = (newData[3] << 8) | newData[2];
    data[2] = (newData[5] << 8) | newData[4];
    return;
}

uint8_t LSM9DS1_accReady(LSM9DS1 *target){

    uint8_t regValue;
    regValue =  SPI_read(target->port,target->gx_port,
            target->gx_pin,target->cs_pol,LSM9DS1_STATUS_REG_0 | 0x80);

    return (regValue & 0x01);
}

void LSM9DS1_getAcc(LSM9DS1 *target,int16_t *data){

    uint16_t newData[6];

    SPI_repeatRead(target->port,target->gx_port,
            target->gx_pin,target->cs_pol,LSM9DS1_OUT_X_L_XL | 0x80,newData,6);

    data[0] = (newData[1] << 8) | newData[0];
    data[1] = (newData[3] << 8) | newData[2];
    data[2] = (newData[5] << 8) | newData[4];
    return;
}

uint8_t LSM9DS1_tempReady(LSM9DS1 *target){

    uint8_t regValue;
    regValue =  SPI_read(target->port,target->gx_port,
            target->gx_pin,target->cs_pol,LSM9DS1_STATUS_REG_0 | 0x80);

    return (regValue & 0x04);
}

float LSM9DS1_getTemp(LSM9DS1 *target){

    uint16_t newData[2];

    SPI_repeatRead(target->port,target->gx_port,
            target->gx_pin,target->cs_pol,LSM9DS1_OUT_TEMP_L | 0x80,newData,2);

    //Temperature sensitivity 16 bit / Degree, bias = 25 degrees
    return (((int16_t)((newData[1] << 8) | newData[0])/16.0f) + 25);

}

uint8_t LSM9DS1_gxWAI(LSM9DS1 *target){

    return SPI_read(target->port,target->gx_port,
            target->gx_pin,target->cs_pol,LSM9DS1_WHO_AM_I_M | 0x80);
}

uint8_t LSM9DS1_mWAI(LSM9DS1 *target){

    return SPI_read(target->port,target->m_port,
            target->m_pin,target->cs_pol,LSM9DS1_WHO_AM_I_XG | 0x80);
}

void LSM9DS1_writeByte(LSM9DS1 *target, uint8_t component, uint8_t address, uint8_t data){

    if(component == LSM9DS1_M){

        SPI_write(target->port,target->m_port,
                target->m_pin,target->cs_pol,address,data);
    }
    else{

        SPI_write(target->port,target->gx_port,
                target->gx_pin,target->cs_pol,address,data);
    }
}

uint16_t LSM9DS1_readByte(LSM9DS1 *target, uint8_t component, uint8_t address){

    if(component == LSM9DS1_M){

        return SPI_read(target->port,target->m_port,
                target->m_pin,target->cs_pol,address | 0x80);
    }
    else{

        return SPI_read(target->port,target->gx_port,
                target->gx_pin,target->cs_pol,address | 0x80);
    }
}
