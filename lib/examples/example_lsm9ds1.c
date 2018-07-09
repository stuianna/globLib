#include <stm32f1x3_core.h>
#include <coms128.h>
#include <lsm9ds1.h>

//Instance for the chip
LSM9DS1 chip;

//Containers to store the data in
int16_t magData[3];
int16_t accData[3];
int16_t gyrData[3];
float temp;

int main(void){

    CLOCK_setSpeed(CLOCK_EX_8_OUT_72);
    USART_setup(USART_1);
    USART_setBaud(USART_1,USART_BAUD_115200);
    COMS(USART_1);

    //SPI and GPIO chip select pins must be setup before and externally of LSM9DS1_setup()
    pinSetup(GPIO_DO,PORTA,PIN4);
    pinSetup(GPIO_DO,PORTA,PIN3);
    SPI_setup(SPI_1);

    //Setup chip
    LSM9DS1_setup(&chip,SPI_1,PORTA,PIN3,PORTA,PIN4,SPI_CS_LOW);

    while(1){
       
        //Magneto is the slower update (150Hz), use it to slow the loop time
        //Gyro and accel can be read faster (950Hz)
        while(!LSM9DS1_magReady(&chip));
        LSM9DS1_getMag(&chip,magData);
        LSM9DS1_getGyr(&chip,gyrData);
        LSM9DS1_getAcc(&chip,accData);
        temp = LSM9DS1_getTemp(&chip);

        //Print all the data
        printt(magData[0]);
        printt(magData[1]);
        printt(magData[2]);
        printt(gyrData[0]);
        printt(gyrData[1]);
        printt(gyrData[2]);
        printt(accData[0]);
        printt(accData[1]);
        printt(accData[2]);
        printl(temp);
    }

    return 0;
}


