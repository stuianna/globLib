#include "stm32f76xxx_flash.h"

void FLASH_setWaitStates(uint32_t hclkFrequency){

	uint8_t waitStates = 0;

	if(hclkFrequency <= 30000000){ waitStates = 0; }
	else if(hclkFrequency <= 90000000){ waitStates = 2; }
	else if(hclkFrequency <= 120000000){ waitStates = 3; }
	else if(hclkFrequency <= 150000000){ waitStates = 4; }
	else if(hclkFrequency <= 180000000){ waitStates = 5; }
	else if(hclkFrequency <= 210000000){ waitStates = 6; }
	else { waitStates = 7; }

	MODIFY_REG(FLASH->ACR,FLASH_ACR_LATENCY_Msk,waitStates << FLASH_ACR_LATENCY_Pos);
}
