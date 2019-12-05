#include "stm32f76xxx_core.h"
#include "printf.h"

void _putchar(void);
void printClockSpeeds(void);
void updateClock(Rcc_ClockConfig*);

int main(void){

	USART_setup(USART_3C);

	Rcc_ClockConfig  pll216 = template_PLL_216;					// Use the PLL outputting at 216Mhz
	Rcc_ClockConfig  hsi8 	= template_HSI_8;						// Use the HSI outputting at 8Mhz

	// The following PLL setting are specific to the STM32F767
	// These settings set the system clock to 14Mhz.
	// STM32CubeMX can be used to help find the settings.
	Rcc_ClockConfig  custom;
	custom.systemClock 	= RCC_SYSCLOCK_PLL;							// Use the PLL as system clock
	custom.pllInput 		= RCC_PLL_INPUT_HSI;						// USE the HSI clock as PLL input
	custom.pllM 				= 8;														// PLL input divisor, this should scale the
																											// Input clock to between 1-2MHz (this case to 2MHz)
	custom.pllN 				= 168;													// PLL multiplier, this should be chosen to the output is
																											// between 100 and 432Mhz
	custom.pllQ       	= 7;														// PLL USB divisor, the prescaler should be set so the USB
																											// clock is > 48Mhz. Note if random number generator is used
																											// the output should be <= 48Mhz. In this case it is equal to 48Mhz.
	custom.pllR     		= 2;														// Divisor for the DSI clock
	custom.pllP 				= RCC_PLL_DIVISOR_6;						// Final PLL output divisor.
	custom.ahbPrescale 	= RCC_PRESCALER_AHB_4;					// Additional prescaler applied to HCLK (sysclock) and APB busses.

	// Print the original clock speeds (16Mhz)
	printClockSpeeds();

	// Set and print the clock at 216Mhz
	updateClock(&pll216);
	printClockSpeeds();

	// Set and print the clock at 8Mhz
	updateClock(&hsi8);
	printClockSpeeds();

	// Set and print the clock at 14Mhz
	updateClock(&custom);
	printClockSpeeds();


}

void updateClock(Rcc_ClockConfig *config){

	RCC_setSystemClock(config);

	// Need to reset timing dependant peripherals
	USART_setBaud(USART3,USART_BAUD_9600);
}

void printClockSpeeds(void){

	printf("\n\nSystem Clock = %.2fMHz\n",SystemCoreClock/1000000.0);
	printf("AHB1 Peripheral Clock = %.2fMHz\n",RCC_getClockSpeed(RCC_APB1_PERIPHERAL)/1000000.0);
	printf("AHB1 Timer Clock = %.2fMHz\n",RCC_getClockSpeed(RCC_APB1_TIMER)/1000000.0);
	printf("AHB2 Peripheral Clock = %.2fMHz\n",RCC_getClockSpeed(RCC_APB2_PERIPHERAL)/1000000.0);
	printf("AHB2 Timer Clock = %.2fMHz\n",RCC_getClockSpeed(RCC_APB2_TIMER)/1000000.0);
	printf("Systick Clock = %.2fMHz\n",RCC_getClockSpeed(RCC_CORTEX_CLOCK)/1000000.0);
}

void _putchar(char byte){

	USART_put(USART3,byte);
}

