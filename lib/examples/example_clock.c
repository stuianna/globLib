#include <stm32f1x3_core.h>
#include <coms128.h>
#include <mpu6050.h>

//Create a data structure for the device
MPU6050 mpu;

int main(void)
{

    //This should be done before other peripherals are set up
    CLOCK_setSpeed(CLOCK_EX_8_OUT_72);

    //Serial communications
    USART_setup(USART_1);
    COMS(USART_1);

    //Print out clock freuqency
    prints("Clock speed =");
    printl(CLOCK_getSpeed());

    //Done
	while (1);

	return 0;
}
