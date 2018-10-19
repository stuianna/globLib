#include <stm32f103cb_core.h>
#include <coms128.h>
#include <mpu6050.h>

//Create a data structure for the device
MPU6050 mpu;

int main(void)
{
    //Serial communications
    USART_setup(USART_1);
    COMS(USART_1);

    //Debug enable 
    MCU_debugEnable(USART_add_put(USART_1));

    //Setup I2C
    I2C_setup(I2C_2);

    //Setup Sensor
    MPU6050_setup(&mpu,I2C_2,MPU_DEVICE_ADDRESS);

    //Set the accelerometer and gyroscope range
    MPU6050_setAcc(&mpu,MPU_ACC_8G);
    MPU6050_setGyr(&mpu,MPU_GYR_1000);

    //Container to store the calculated values
    float data[7];

    //For delay
    volatile int i;

	while (1) {

        //Get raw data
        MPU6050_calc(&mpu,MPU_ALL,data);

        //Acceleration
        printt(data[0]);
        printt(data[1]);
        printt(data[2]);

        //Temperature
        printt(data[3]);

        //Gyro
        printt(data[4]);
        printt(data[5]);
        printl(data[6]);
    
        //wait a bit
        for(i = 0; i < 40000;i++);
	}
	return 0;
}
