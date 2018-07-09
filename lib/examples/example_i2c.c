#include <stm32f1x3_core.h>

#define I2C_SLAVE_ADDRESS 0xA0

int main(void){

    volatile int i;
    uint8_t recData;

    //Setup a port
    I2C_setup(I2C_2);

	while (1) {

        //Typical read sequence
        I2C_start(I2C_2);
        I2C_address(I2C_2,I2C_SLAVE_ADDRESS,I2C_WRITE);
        I2C_write(I2C_2,0x75);
        I2C_start(I2C_2);
        I2C_address(I2C_2,I2C_SLAVE_ADDRESS,I2C_READ);
        recData = I2C_repeatRead(I2C_2)<<8;
        I2C_stop(I2C_2);        //Need to send stop before last byte read
        recData |= I2C_read(I2C_2);

        //Typical write sequence
        I2C_start(I2C_2);
        I2C_address(I2C_2,MPU_DEVICE_ADDRESS,I2C_WRITE);
        I2C_write(I2C_2,address);
        I2C_write(I2C_2,data);
        I2C_stop(I2C_2);
	}

	return 0;
}

