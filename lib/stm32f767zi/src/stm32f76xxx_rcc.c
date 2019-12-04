#include "stm32f76xxx_rcc.h"

void rcc_enableHSE(void);
void rcc_disableHSE(void);
void rcc_enableHSI(void);
void rcc_disableHSI(void);
void rcc_configurePLL(Rcc_ClockConfig *config);
void rcc_setAhbPrescaler(Rcc_Bus_Ahb_Prescaler p);

const Rcc_ClockConfig template_HSI_16 = {
	.systemClock 	= RCC_SYSCLOCK_HSI,
	.pllInput 		= RCC_PLL_INPUT_HSI,
	.ahbPrescale  = RCC_PRESCALER_AHB_0,
	.pllM 				= 0,
	.pllN 				= 0,
	.pllP 				= RCC_PLL_DIVISOR_2,
	.pllQ 				= 0,
	.pllR 				= 0,
	.target  			= 16000000
};

const Rcc_ClockConfig template_HSI_8 = {
	.systemClock 	= RCC_SYSCLOCK_HSI,
	.pllInput 		= RCC_PLL_INPUT_HSI,
	.ahbPrescale  = RCC_PRESCALER_AHB_2,
	.pllM 				= 0,
	.pllN 				= 0,
	.pllP 				= RCC_PLL_DIVISOR_2,
	.pllQ 				= 0,
	.pllR 				= 0,
	.target  			= 8000000
};

const Rcc_ClockConfig template_HSI_1 = {
	.systemClock 	= RCC_SYSCLOCK_HSI,
	.pllInput 		= RCC_PLL_INPUT_HSI,
	.ahbPrescale  = RCC_PRESCALER_AHB_16,
	.pllM 				= 0,
	.pllN 				= 0,
	.pllP 				= RCC_PLL_DIVISOR_2,
	.pllQ 				= 0,
	.pllR 				= 0,
	.target  			= 1000000
};

const Rcc_ClockConfig template_PLL_48 = {
	.systemClock 	= RCC_SYSCLOCK_PLL,
	.pllInput 		= RCC_PLL_INPUT_HSI,
	.ahbPrescale  = RCC_PRESCALER_AHB_0,
	.pllM 				= 8,
	.pllN 				= 192,
	.pllP 				= RCC_PLL_DIVISOR_8,
	.pllQ 				= 8,
	.pllR 				= 2,
	.target  			= 48000000
};

const Rcc_ClockConfig template_PLL_108 = {
	.systemClock 	= RCC_SYSCLOCK_PLL,
	.pllInput 		= RCC_PLL_INPUT_HSI,
	.ahbPrescale  = RCC_PRESCALER_AHB_0,
	.pllM 				= 8,
	.pllN 				= 216,
	.pllP 				= RCC_PLL_DIVISOR_4,
	.pllQ 				= 9,
	.pllR 				= 2,
	.target  			= 108000000
};

const Rcc_ClockConfig template_PLL_216 = {
	.systemClock 	= RCC_SYSCLOCK_PLL,
	.pllInput 		= RCC_PLL_INPUT_HSI,
	.ahbPrescale  = RCC_PRESCALER_AHB_0,
	.pllM 				= 8,
	.pllN 				= 216,
	.pllP 				= RCC_PLL_DIVISOR_2,
	.pllQ 				= 9,
	.pllR 				= 2,
	.target  			= 216000000
};

void rcc_enableHSI(void){

	RCC->CR |= RCC_CR_HSION;
	while(!(RCC->CR & RCC_CR_HSIRDY)){};
}

void rcc_disableHSI(void){

	RCC->CR &= ~RCC_CR_HSION;
}

void rcc_enableHSE(void){

	RCC->CR |= RCC_CR_HSION;
	while(!(RCC->CR & RCC_CR_HSIRDY)){};
}

void rcc_disableHSE(void){

	RCC->CR &= ~RCC_CR_HSEON;
}

void rcc_setAhbPrescaler(Rcc_Bus_Ahb_Prescaler p){
	MODIFY_REG(RCC->CFGR,RCC_CFGR_HPRE_Msk,p << RCC_CFGR_HPRE_Pos);
}

void rcc_configurePLL(Rcc_ClockConfig *config){

	// Disable PLL so it can be configured
	RCC->CR &= ~RCC_CR_PLLON;

	// PLL Configuration
	if(config->pllInput == RCC_PLL_INPUT_HSI){
		rcc_enableHSI();
		RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLSRC;
	}else{
		rcc_enableHSE();
		RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC;
	}

	// Set divisors
	MODIFY_REG(RCC->PLLCFGR,
			(RCC_PLLCFGR_PLLR_Msk |
			 RCC_PLLCFGR_PLLQ_Msk |
			 RCC_PLLCFGR_PLLP_Msk |
			 RCC_PLLCFGR_PLLN_Msk |
			 RCC_PLLCFGR_PLLM_Msk ),(
				 config->pllR << RCC_PLLCFGR_PLLR_Pos |
				 config->pllQ << RCC_PLLCFGR_PLLQ_Pos |
				 config->pllP << RCC_PLLCFGR_PLLP_Pos |
				 config->pllN << RCC_PLLCFGR_PLLN_Pos |
				 config->pllM << RCC_PLLCFGR_PLLM_Pos ));


	// Enable and wait for PLL to be ready
	RCC->CR |= RCC_CR_PLLON;
	while(!(RCC->CR & RCC_CR_PLLRDY));

	// Adjust wait statues
	FLASH_setWaitStates(config->target);

	// Set PLL as source and wait to be ready
	MODIFY_REG(RCC->CFGR,RCC_CFGR_SW_Msk,RCC_CFGR_SW_1);
	while(!(RCC->CFGR & RCC_CFGR_SWS_1)){};

}

uint8_t RCC_setSystemClock(Rcc_ClockConfig *config){

	// Enable the required clock source
	switch(config->systemClock){
		default:
		case RCC_SYSCLOCK_HSI:

			// Is HSI already the clock source?
			if(!(RCC->CFGR & RCC_CFGR_SW_Msk)){

				rcc_setAhbPrescaler(config->ahbPrescale);
				SystemCoreClockUpdate();
				return !(SystemCoreClock == config->target);
			}

			rcc_enableHSI();
			RCC->CFGR = RCC->CFGR & ~RCC_CFGR_SW_Msk;
			while(RCC->CFGR & RCC_CFGR_SWS_Msk){};

			rcc_setAhbPrescaler(config->ahbPrescale);
			FLASH_setWaitStates(config->target);
			SystemCoreClockUpdate();

			// Update the peripheral bus prescalers to none.
			RCC_setBusPrescaler(RCC_APB1_PERIPHERAL,RCC_PRESCALER_APB_0);
			RCC_setBusPrescaler(RCC_APB2_PERIPHERAL,RCC_PRESCALER_APB_0);
			return !(SystemCoreClock == config->target);

		case RCC_SYSCLOCK_HSE:

			// IS HSE already the clock source?
			if(!(RCC->CFGR & RCC_CFGR_SWS_0)){

				rcc_setAhbPrescaler(config->ahbPrescale);
				SystemCoreClockUpdate();
				return !(SystemCoreClock == config->target);
			}
			rcc_enableHSE();
			RCC->CFGR = RCC->CFGR & ~RCC_CFGR_SW_Msk;
			RCC->CFGR |= RCC_CFGR_SW_0;
			while(RCC->CFGR & RCC_CFGR_SWS_0){};

			rcc_setAhbPrescaler(config->ahbPrescale);
			FLASH_setWaitStates(config->target);
			SystemCoreClockUpdate();

			// Update the peripheral bus prescalers to none.
			RCC_setBusPrescaler(RCC_APB1_PERIPHERAL,RCC_PRESCALER_APB_0);
			RCC_setBusPrescaler(RCC_APB2_PERIPHERAL,RCC_PRESCALER_APB_0);
			return !(SystemCoreClock == config->target);

		case RCC_SYSCLOCK_PLL:

			// Need to switch back to HSI to change PLL settings
			if((RCC->CFGR & RCC_CFGR_SWS_Msk) == RCC_CFGR_SWS_1){
				rcc_enableHSI();
				RCC->CFGR = RCC->CFGR & ~RCC_CFGR_SW_Msk;
				while(RCC->CFGR & RCC_CFGR_SWS_Msk){};
			}
			break;
	}

	rcc_configurePLL(config);

	rcc_setAhbPrescaler(config->ahbPrescale);
	SystemCoreClockUpdate();

	RCC_setBusMaximum(RCC_APB1_PERIPHERAL);
	RCC_setBusMaximum(RCC_APB2_PERIPHERAL);

	return !(SystemCoreClock == config->target);
}

uint32_t RCC_setBusMaximum(Rcc_Clock clock){

	uint8_t prescaleTarget = 0;

	switch(clock){
		case RCC_APB1_PERIPHERAL:
			// APB1 bus has a maximum speed of 54MHz
			prescaleTarget = SystemCoreClock / 54000000;
			break;
		case RCC_APB2_PERIPHERAL:
			// APB2 bus has a maximum speed of 108MHz
			prescaleTarget = SystemCoreClock / 108000000;
			break;
		default:
			return 0;
	}

	if(prescaleTarget > 2){
		return RCC_setBusPrescaler(clock,RCC_PRESCALER_APB_4);
	}else if(prescaleTarget > 0){
		return RCC_setBusPrescaler(clock,RCC_PRESCALER_APB_2);
	}else{
		return RCC_setBusPrescaler(clock,RCC_PRESCALER_APB_0);
	}

	return 0;
}

uint32_t RCC_getClockSpeed(Rcc_Clock clock){

	switch(clock){
		case RCC_APB1_PERIPHERAL:

			if(!(RCC->CFGR & RCC_CFGR_PPRE1_Msk)){
				return SystemCoreClock;
			}
			return SystemCoreClock >> ((((RCC->CFGR & RCC_CFGR_PPRE1_Msk) >> RCC_CFGR_PPRE1_Pos)) - 3);

		case RCC_APB1_TIMER:

			if((((RCC->CFGR & RCC_CFGR_PPRE1_Msk) >> RCC_CFGR_PPRE1_Pos)) - 3){
				return 2*SystemCoreClock >> ((((RCC->CFGR & RCC_CFGR_PPRE1_Msk) >> RCC_CFGR_PPRE1_Pos)) - 3);
			}
			return SystemCoreClock >> ((((RCC->CFGR & RCC_CFGR_PPRE1_Msk) >> RCC_CFGR_PPRE1_Pos)) - 3);

		case RCC_APB2_PERIPHERAL:

			if(!(RCC->CFGR & RCC_CFGR_PPRE2_Msk)){
				return SystemCoreClock;
			}
			return SystemCoreClock >> ((((RCC->CFGR & RCC_CFGR_PPRE2_Msk) >> RCC_CFGR_PPRE2_Pos)) - 3);

		case RCC_APB2_TIMER:

			if((((RCC->CFGR & RCC_CFGR_PPRE2_Msk) >> RCC_CFGR_PPRE2_Pos)) - 3){
				return 2*SystemCoreClock >> ((((RCC->CFGR & RCC_CFGR_PPRE2_Msk) >> RCC_CFGR_PPRE2_Pos)) - 3);
			}
			return SystemCoreClock >> ((((RCC->CFGR & RCC_CFGR_PPRE2_Msk) >> RCC_CFGR_PPRE2_Pos)) - 3);

		case RCC_SYSTEM_CLOCK:
			return SystemCoreClock;
		case RCC_CORTEX_CLOCK:
			return SystemCoreClock;
	}
	return 0;
}

uint32_t RCC_setBusPrescaler(Rcc_Clock clock, Rcc_Bus_Apb_Prescaler prescale){

	switch(clock){
		case RCC_APB1_PERIPHERAL:
			MODIFY_REG(RCC->CFGR,RCC_CFGR_PPRE1_Msk,prescale << RCC_CFGR_PPRE1_Pos);
			return SystemCoreClock >> ((((RCC->CFGR & RCC_CFGR_PPRE1_Msk) >> RCC_CFGR_PPRE1_Pos)) - 3);

		case RCC_APB2_PERIPHERAL:
			MODIFY_REG(RCC->CFGR,RCC_CFGR_PPRE2_Msk,prescale << RCC_CFGR_PPRE2_Pos);
			return SystemCoreClock >> ((((RCC->CFGR & RCC_CFGR_PPRE2_Msk) >> RCC_CFGR_PPRE2_Pos)) - 3);

		default:
			break;
	}
	return 0;
}

bool RCC_clockEnable(RCC_Peripheral peripheral){

    switch(peripheral){
        case RCC_AHB1ENR_GPIOAEN:
        case RCC_AHB1ENR_GPIOBEN:
        case RCC_AHB1ENR_GPIOCEN:
        case RCC_AHB1ENR_GPIODEN:
        case RCC_AHB1ENR_GPIOEEN:
        case RCC_AHB1ENR_GPIOFEN:
        case RCC_AHB1ENR_GPIOGEN:
        case RCC_AHB1ENR_GPIOHEN:
        case RCC_AHB1ENR_GPIOIEN:
        case RCC_AHB1ENR_GPIOJEN:
        case RCC_AHB1ENR_GPIOKEN:
            SET_BIT(RCC->AHB1ENR,peripheral);
            break;

        case RCC_APB1ENR_USART2EN:
        case RCC_APB1ENR_USART3EN:
            SET_BIT(RCC->APB1ENR,peripheral);
            break;

        case RCC_APB2ENR_SYSCFGEN:
            SET_BIT(RCC->APB2ENR,peripheral);
            break;
        default:
            break;
    }

    switch(peripheral){
        case RCC_APB2ENR_USART1EN:
            SET_BIT(RCC->APB2ENR,peripheral);
            break;
        default:
            return ERROR;
    }
    return SUCCESS;
}

bool RCC_clockDisable(RCC_Peripheral peripheral){

    switch(peripheral){
        case RCC_AHB1ENR_GPIOAEN:
        case RCC_AHB1ENR_GPIOBEN:
        case RCC_AHB1ENR_GPIOCEN:
        case RCC_AHB1ENR_GPIODEN:
        case RCC_AHB1ENR_GPIOEEN:
        case RCC_AHB1ENR_GPIOFEN:
        case RCC_AHB1ENR_GPIOGEN:
        case RCC_AHB1ENR_GPIOHEN:
        case RCC_AHB1ENR_GPIOIEN:
        case RCC_AHB1ENR_GPIOJEN:
        case RCC_AHB1ENR_GPIOKEN:
            CLEAR_BIT(RCC->AHB1ENR,peripheral);
            break;

        case RCC_APB1ENR_USART2EN:
        case RCC_APB1ENR_USART3EN:
            CLEAR_BIT(RCC->APB1ENR,peripheral);
            break;

        case RCC_APB2ENR_SYSCFGEN:
            CLEAR_BIT(RCC->APB2ENR,peripheral);
            break;
        default:
            break;
    }

    switch(peripheral){
        case RCC_APB2ENR_USART1EN:
            CLEAR_BIT(RCC->APB2ENR,peripheral);
            break;
        default:
            return ERROR;
    }
    return SUCCESS;

}
