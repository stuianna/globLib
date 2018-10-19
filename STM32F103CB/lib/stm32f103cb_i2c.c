/*!**************************************************************************
    @file stm32f103cb_i2c.c
    @brief Source file for stm32f103cb I2C
    @author Stuart Ianna
    @version 0.1
    @date June 2018
    @copyright GNU GPLv3
    @warning None
    @bug 

    @details

    @par Compilers
    - arm-none-eabi-gcc (15:4.9.3+svn231177-1) 4.9.3 20150529 (prerelease)
******************************************************************************/

#include "stm32f103cb_i2c.h"

/****************************************************************************/
//				                I2C1
/****************************************************************************/

//I2C1 routines
static void i2c1_setup(void);
static mcu_error i2c1_write(uint8_t byte);
static uint8_t i2c1_read(void);
static uint8_t i2c1_repeatRead(void);
static mcu_error i2c1_start(void);
static mcu_error i2c1_stop(void);
static mcu_error i2c1_address(uint8_t address, uint8_t operation);

/****************************************************************************/
//				                I2C2
/****************************************************************************/

static void i2c2_setup(void);
static mcu_error i2c2_write(uint8_t byte);
static uint8_t i2c2_read(void);
static uint8_t i2c2_repeatRead(void);
static mcu_error i2c2_start(void);
static mcu_error i2c2_stop(void);
static mcu_error i2c2_address(uint8_t address, uint8_t operation);


/****************************************************************************/
//			                SETUP FUNCTIONS
/****************************************************************************/

mcu_error I2C_setup(i2c_periph peripheral){

	switch(peripheral){

		case I2C_1:

			//Setup the port
            i2c1_setup();
			break;

		case I2C_2:

			//Setup the port
            i2c2_setup();
			break;

		default:

			if(MCU_debugIsEnabled()){
				MCU_printError(E_I2C_PORT);
			}
            return E_I2C_PORT;
			break;
	}

	return E_I2C_NOERROR;
}

void i2c1_setup(void){

	//Setup the GPIO pins
	pinSetup(GPIO_I2C,PORTB,PIN6);
	pinSetup(GPIO_I2C,PORTB,PIN7);

    //Setup clock
    rcc_periph_clock_enable(RCC_AFIO);
    rcc_periph_clock_enable(RCC_I2C1);

    //Disable port
    i2c_peripheral_disable(I2C1);


    //The frequency of the APB bus needs to be entered
    I2C_CR2(I2C1) |= (rcc_apb2_frequency/1000000);

    //Set fast mode
    i2c_set_fast_mode(I2C1);

    //Clock speed should be 400kHz -> 2.5us divided by 25 = 100ns
    //CCR register should count until 900ns
    i2c_set_ccr(I2C1,(uint8_t)(900/(10000000000.0f/(float)rcc_apb2_frequency)));

    //Set duty cycle to one
    I2C_CCR(I2C1) |= I2C_CCR_DUTY;

    //Set rise time
    //For 400kHz rise time is 300ns so
    i2c_set_trise(I2C1, (uint8_t)(300/(1000000000.0f/(float)rcc_apb2_frequency)));

    //Set own address
    i2c_set_own_7bit_slave_address(I2C1, I2C_MY_ADDRESS);

    //Enable 
    i2c_peripheral_enable(I2C1);
}

void i2c2_setup(void){

	//Setup the GPIO pins
	pinSetup(GPIO_I2C,PORTB,PIN10);
	pinSetup(GPIO_I2C,PORTB,PIN11);

    //Setup clock
    rcc_periph_clock_enable(RCC_AFIO);
    rcc_periph_clock_enable(RCC_I2C2);

    //Disable port
    i2c_peripheral_disable(I2C2);

    //The frequency of the APB bus needs to be entered
    I2C_CR2(I2C2) |= (rcc_apb2_frequency/1000000);

    //Set fast mode
    i2c_set_fast_mode(I2C2);

    //Clock speed should be 400kHz -> 2.5us divided by 25 = 100ns
    //CCR register should count until 900ns
    i2c_set_ccr(I2C2,(uint8_t)(900/(10000000000.0f/(float)rcc_apb2_frequency)));

    //Set duty cycle to one
    I2C_CCR(I2C2) |= I2C_CCR_DUTY;

    //Set rise time
    //For 400kHz rise time is 300ns so
    i2c_set_trise(I2C2, (uint8_t)(300/(1000000000.0f/(float)rcc_apb2_frequency)));

    //Set own address
    i2c_set_own_7bit_slave_address(I2C2, I2C_MY_ADDRESS);

    //Enable 
    i2c_peripheral_enable(I2C2);
}

/****************************************************************************/
//			                    WRITE FUNCTIONS
/****************************************************************************/

mcu_error I2C_write(i2c_periph peripheral, uint8_t byte){

	switch(peripheral){

		case I2C_1:

            //Write a byte to the bus
            i2c1_write(byte);
			break;

		case I2C_2:

            //Write a byte to the bus
            i2c2_write(byte);
			break;

		default:

			if(MCU_debugIsEnabled()){
				MCU_printError(E_I2C_PORT);
			}
            return E_I2C_PORT;
			break;
	}

	return E_I2C_NOERROR;
}

mcu_error i2c1_write(uint8_t byte){

    uint32_t timeoutCounter = 0;

    i2c_send_data(I2C1, byte);
    while (!(I2C_SR1(I2C1) & (I2C_SR1_BTF | I2C_SR1_TxE))){

        timeoutCounter++;
        if(timeoutCounter >= I2C_TIMEOUT){

			if(MCU_debugIsEnabled()){
				MCU_printError(E_I2C_WRITE);
			}
            return E_I2C_WRITE;
        }
    }

    return E_I2C_NOERROR;
}

mcu_error i2c2_write(uint8_t byte){

    uint32_t timeoutCounter = 0;

    i2c_send_data(I2C2, byte);
    while (!(I2C_SR1(I2C2) & (I2C_SR1_BTF | I2C_SR1_TxE))){

        timeoutCounter++;
        if(timeoutCounter >= I2C_TIMEOUT){

			if(MCU_debugIsEnabled()){
				MCU_printError(E_I2C_WRITE);
			}
            return E_I2C_WRITE;
        }
    }

    return E_I2C_NOERROR;
}

/****************************************************************************/
//			                    READ FUNCTIONS
/****************************************************************************/
mcu_error I2C_read(i2c_periph peripheral){

	uint8_t byte;

	switch(peripheral){

		case I2C_1:

			//read a byte
            byte = i2c1_read();
			break;

		case I2C_2:

            //Read a byte
            byte = i2c2_read();
			break;

		default:
			byte =  E_I2C_PORT;

			if(MCU_debugIsEnabled()){
				MCU_printError(E_I2C_PORT);
			}
			break;
	}

	return byte;
}
uint8_t i2c1_read(void){

    uint32_t timeoutCounter = 0;

    I2C_CR1(I2C1) &= ~I2C_CR1_ACK;

    //Wait for byte to arrive
    while (!(I2C_SR1(I2C1) & I2C_SR1_RxNE)){

        timeoutCounter++;
        if(timeoutCounter >= I2C_TIMEOUT){

			if(MCU_debugIsEnabled()){
				MCU_printError(E_I2C_READ);
			}
            return E_I2C_READ;
        }

    }

    return I2C_DR(I2C1);
}

uint8_t i2c2_read(void){

    uint32_t timeoutCounter = 0;

    I2C_CR1(I2C2) &= ~I2C_CR1_ACK;

    //Wait for byte to arrive
    while (!(I2C_SR1(I2C2) & I2C_SR1_RxNE)){

        timeoutCounter++;
        if(timeoutCounter >= I2C_TIMEOUT){

			if(MCU_debugIsEnabled()){
				MCU_printError(E_I2C_READ);
			}
            return E_I2C_READ;
        }

    }

    return I2C_DR(I2C2);
}

uint8_t I2C_repeatRead(i2c_periph peripheral){

	uint8_t byte;

	switch(peripheral){

		case I2C_1:

			//read a byte
            byte = i2c1_repeatRead();
			break;

		case I2C_2:

            //Read a byte
            byte = i2c2_repeatRead();
			break;

		default:
			byte =  E_I2C_PORT;

			if(MCU_debugIsEnabled()){
				MCU_printError(E_I2C_PORT);
			}
			break;
	}

	return byte;
}

uint8_t i2c1_repeatRead(void){

    uint32_t timeoutCounter = 0;

    I2C_CR1(I2C1) |= I2C_CR1_ACK;

    //Wait for byte to arrive
    while (!(I2C_SR1(I2C1) & I2C_SR1_RxNE)){

        timeoutCounter++;
        if(timeoutCounter >= I2C_TIMEOUT){

			if(MCU_debugIsEnabled()){
				MCU_printError(E_I2C_READ);
			}
            return E_I2C_READ;
        }

    }

    return I2C_DR(I2C1);
}

uint8_t i2c2_repeatRead(void){

    uint32_t timeoutCounter = 0;

    I2C_CR1(I2C2) |= I2C_CR1_ACK;

    //Wait for byte to arrive
    while (!(I2C_SR1(I2C2) & I2C_SR1_RxNE)){

        timeoutCounter++;
        if(timeoutCounter >= I2C_TIMEOUT){

			if(MCU_debugIsEnabled()){
				MCU_printError(E_I2C_READ);
			}
            return E_I2C_READ;
        }

    }

    return I2C_DR(I2C2);
}

/****************************************************************************/
//			                    START FUNCTIONS
/****************************************************************************/
mcu_error I2C_start(i2c_periph peripheral){

	switch(peripheral){

		case I2C_1:

            //Generate start condition
            i2c1_start();
			break;

		case I2C_2:

            //Generate start condition
            i2c2_start();
			break;

		default:

			if(MCU_debugIsEnabled()){
				MCU_printError(E_I2C_PORT);
			}
            return E_I2C_PORT;
			break;
	}

	return E_I2C_NOERROR;
}

mcu_error i2c1_start(void){

    uint32_t timeoutCounter = 0;

    //Set the start bit
    i2c_send_start(I2C1);

    while (!((I2C_SR1(I2C1) & I2C_SR1_SB)& (I2C_SR2(I2C1) & (I2C_SR2_MSL | I2C_SR2_BUSY)))){

        timeoutCounter++;
        if(timeoutCounter >= I2C_TIMEOUT){

			if(MCU_debugIsEnabled()){
				MCU_printError(E_I2C_START);
			}
            return E_I2C_START;
        }

    }

    return E_I2C_NOERROR;
}

mcu_error i2c2_start(void){

    uint32_t timeoutCounter = 0;

    //Set the start bit
    i2c_send_start(I2C2);

    while (!((I2C_SR1(I2C2) & I2C_SR1_SB)& (I2C_SR2(I2C2) & (I2C_SR2_MSL | I2C_SR2_BUSY)))){

        timeoutCounter++;
        if(timeoutCounter >= I2C_TIMEOUT){

			if(MCU_debugIsEnabled()){
				MCU_printError(E_I2C_START);
			}
            return E_I2C_START;
        }

    }

    return E_I2C_NOERROR;
}

/****************************************************************************/
//			                    STOP FUNCTIONS
/****************************************************************************/

mcu_error I2C_stop(i2c_periph peripheral){

	switch(peripheral){

		case I2C_1:

            //Generate start condition
            i2c1_stop();
			break;

		case I2C_2:

            //Generate start condition
            i2c2_stop();
			break;

		default:

			if(MCU_debugIsEnabled()){
				MCU_printError(E_I2C_PORT);
			}
            return E_I2C_PORT;
			break;
	}

    return E_I2C_NOERROR;
}

mcu_error i2c1_stop(void){

    i2c_send_stop(I2C1);
    return E_I2C_NOERROR;
}

mcu_error i2c2_stop(void){

    i2c_send_stop(I2C2);
    return E_I2C_NOERROR;
}

/****************************************************************************/
//			                    ADDRESS FUNCTIONS
/****************************************************************************/

mcu_error I2C_address(i2c_periph peripheral, uint8_t address, uint8_t operation){

	switch(peripheral){

		case I2C_1:

            //Send address
            i2c1_address(address, operation);
			break;

		case I2C_2:

            //Send address
            i2c2_address(address, operation);
			break;

		default:

			if(MCU_debugIsEnabled()){
				MCU_printError(E_I2C_PORT);
			}
            return E_I2C_PORT;
			break;
	}

	return E_I2C_NOERROR;
}

mcu_error i2c1_address(uint8_t address, uint8_t operation){

    uint32_t timeoutCounter = 0;

    i2c_send_7bit_address(I2C1, address, operation);

    while (!(I2C_SR1(I2C1) & I2C_SR1_ADDR)){

        timeoutCounter++;
        if(timeoutCounter >= I2C_TIMEOUT){

			if(MCU_debugIsEnabled()){
				MCU_printError(E_I2C_STOP);
			}
            return E_I2C_STOP;
        }

    }

    //Read status register to clear flag
    address = I2C_SR2(I2C1);

    return E_I2C_NOERROR;

}

mcu_error i2c2_address(uint8_t address, uint8_t operation){

    uint32_t timeoutCounter = 0;

    i2c_send_7bit_address(I2C2, address, operation);

    while (!(I2C_SR1(I2C2) & I2C_SR1_ADDR)){

        timeoutCounter++;
        if(timeoutCounter >= I2C_TIMEOUT){

			if(MCU_debugIsEnabled()){
				MCU_printError(E_I2C_STOP);
			}
            return E_I2C_STOP;
        }

    }

    //Read status register to clear flag
    address = I2C_SR2(I2C2);

    return E_I2C_NOERROR;

}
