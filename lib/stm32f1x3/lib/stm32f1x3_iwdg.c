/*!***************************************************************************
    @file stm32f1x3_iwdg.c
    @brief Source file for stm32f103 WATCHDOG 
    @author Stuart Ianna
    @version 1.0
    @date June 2018
    @copyright GNU GPLv3
    @warning None
    @bug 

    @details

    @par Verified Compilers
    - arm-none-eabi-gcc (15:4.9.3+svn231177-1) 4.9.3 20150529 (prerelease)
******************************************************************************/

#include "stm32f1x3_iwdg.h"

mcu_error IWDG_setup(uint32_t timeoutms){

	//Watchdog runs on an independant 40khz oscillator
	//Prescaler rnges from 4-256 in powers of 2
	
	const float watchdog_LSI_period = 1/40.0f; //This is /40 as timeout is in ms
	uint16_t reloadValue;
	uint8_t prescaler; //These are the bits written to the register, not the value itself

	
	//Select the correct prescaler and calculate reload
	if(timeoutms < 400){

		//Prescaler = 4
		prescaler = 0;
		reloadValue = timeoutms/(watchdog_LSI_period * 4) -1; //Subtract 1 as counter counts to 0
	}
	else if(timeoutms < 800){

		//Prescaler = 8
		prescaler = 1;
		reloadValue = timeoutms/(watchdog_LSI_period * 8) -1; //Subtract 1 as counter counts to 0
	}
	else if(timeoutms < 1600){

		//Prescaler = 16 
		prescaler = 2;
		reloadValue = timeoutms/(watchdog_LSI_period * 16) -1; //Subtract 1 as counter counts to 0
	}
	else if(timeoutms < 3200){

		//Prescaler = 32 
		prescaler = 3;
		reloadValue = timeoutms/(watchdog_LSI_period * 32) -1; //Subtract 1 as counter counts to 0
	}
	else if(timeoutms < 6500){

		//Prescaler = 64 
		prescaler = 4;
		reloadValue = timeoutms/(watchdog_LSI_period * 64) -1; //Subtract 1 as counter counts to 0
	}
	else if(timeoutms < 13000){

		//Prescaler = 128 
		prescaler = 5;
		reloadValue = timeoutms/(watchdog_LSI_period * 128) -1; //Subtract 1 as counter counts to 0
	}
	else if(timeoutms < 26000){

		//Prescaler = 256 
		prescaler = 6;
		reloadValue = timeoutms/(watchdog_LSI_period * 256) -1; //Subtract 1 as counter counts to 0
	}
	else{

		//Shouldn't be this long
        if(MCU_debugIsEnabled()){

            MCU_printError(E_IWDG_PERIOD);
        }
        return E_IWDG_PERIOD;
	}


	//Write 0x5555 to enable access to the prescaler and reload registers
	IWDG_KR = 0x5555;

	//Write the preload value
	IWDG_PR = prescaler;

	//Write the reload value
	IWDG_RLR = reloadValue;

	//Write 0xCCCC to the key register to enable the watchdog timer
	IWDG_KR = 0xCCCC;

    return E_IWDG_NOERROR;
}

bool IWDG_resetCheck(void){

	if(RCC_CSR & 0x20000000){

		//Reset the flag
		RCC_CSR |= 0x1000000;
		return true;
	}

	return false;
}

void IWDG_feed(void){

	//Write 0xAAAA to the key register to reload the watchdog timer
	IWDG_KR = 0xAAAA; 
}

