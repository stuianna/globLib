#ifndef LIB_STM32F76XXX_RCC_H
#define LIB_STM32F76XXX_RCC_H

#include "stm32f76xxx_core.h"

/**
  @addtogroup STM32F76XXX
  @{
*/

/*!
    @addtogroup RCC
    @brief Module for reset and clock control (RCC) configuration.
		@details This module can be used for setting enabling / disabling
		peripheral clocks as well as setting system clock and peripheral
		bus speeds.

		By far the best way to determing what parameters are required when
		setting up the PLL or bus clocks is to use the STM32CubeMX graphical
		tool. The prescalers and settings there can be used directly in the
		clock configuration structure.

    @author Stuart Ianna
    @date November 2019
    @warning none
    @todo
  @{
*/

/*!
    @brief RCC peripheral clocks.
    @details This abstraction allows for common naming and a single function to be
    used to enable and disable clocks.
*/
typedef enum{

    RCC_GPIOA       = RCC_AHB1ENR_GPIOAEN,          //!<GPIOA Clock
    RCC_GPIOB       = RCC_AHB1ENR_GPIOBEN,          //!<GPIOB Clock
    RCC_GPIOC       = RCC_AHB1ENR_GPIOCEN,          //!<GPIOC Clock
    RCC_GPIOD       = RCC_AHB1ENR_GPIODEN,          //!<GPIOD Clock
    RCC_GPIOE       = RCC_AHB1ENR_GPIOEEN,          //!<GPIOE Clock
    RCC_GPIOF       = RCC_AHB1ENR_GPIOFEN,          //!<GPIOF Clock
    RCC_GPIOG       = RCC_AHB1ENR_GPIOGEN,          //!<GPIOG Clock
    RCC_GPIOH       = RCC_AHB1ENR_GPIOHEN,          //!<GPIOH Clock
    RCC_GPIOI       = RCC_AHB1ENR_GPIOIEN,          //!<GPIOI Clock
    RCC_GPIOJ       = RCC_AHB1ENR_GPIOJEN,          //!<GPIOJ Clock
    RCC_GPIOK       = RCC_AHB1ENR_GPIOKEN,          //!<GPIOK Clock
    RCC_SYSCFG      = RCC_APB2ENR_SYSCFGEN,         //!<System configuration clock.
    RCC_USART1      = RCC_APB2ENR_USART1EN,         //!<USART 1 clock
    RCC_USART2      = RCC_APB1ENR_USART2EN,         //!<USART 2 clock
    RCC_USART3      = RCC_APB1ENR_USART3EN,         //!<USART 3 clock
}RCC_Peripheral;

/*!
    @brief Used to select the system clock source in RCC_setSystemClock
*/
typedef enum{
	RCC_SYSCLOCK_HSI = 0x00,				//!< High speed internal oscillator (HSI)
	RCC_SYSCLOCK_HSE = 0x01,				//!< High speed external oscillator (HSE)
	RCC_SYSCLOCK_PLL = 0x10,				//!< Phase locked loop (PLL)
}Rcc_Sys_Clock;

/*!
    @brief Used to select the input to the phase locked loop (PLL)
*/
typedef enum{
	RCC_PLL_INPUT_HSI = 0x00,				//!< HSI is used as PLL input.
	RCC_PLL_INPUT_HSE = 0x01,				//!< HSE is used as PLL input.
}Rcc_Pll_Input;

/*!
    @brief Peripheral busses which can have different clock rates.
		@details This can be used with RCC_getClockSpeed() to quesry
		the speed of the given bus.
*/
typedef enum{
	RCC_APB1_PERIPHERAL,						//!< APB1 peripheral clock bus (PCKL1)
	RCC_APB1_TIMER,									//!< APB1 timer clock bus
	RCC_APB2_PERIPHERAL,						//!< APB2 peripheral clock bus (PCKL1)
	RCC_APB2_TIMER,									//!< APB2 timer clock bus
	RCC_SYSTEM_CLOCK,								//!< System clock
	RCC_CORTEX_CLOCK,								//!< Cortex system clock (systick)
}Rcc_Clock;

/*!
    @brief APB prescalers for the peripheral busses
		@details These prescalers are for APB1 and APB2
*/
typedef enum{
	RCC_PRESCALER_APB_0 	= 0x3,					//!< No prescaler on the bus
	RCC_PRESCALER_APB_2 	= 0x4,					//!< Divisor of 2
	RCC_PRESCALER_APB_4 	= 0x5,					//!< Divisor of 4
	RCC_PRESCALER_APB_8 	= 0x6,					//!< Divisor of 8
	RCC_PRESCALER_APB_16 	= 0x7,					//!< Divisor of 16
}Rcc_Bus_Apb_Prescaler ;

/*!
    @brief AHB prescalers for the system bus.
		@details This prescaler effects all clocks except the ethernet bus.
*/
typedef enum{
	RCC_PRESCALER_AHB_0 		= 0x0,					//!< No prescaler on the bus
	RCC_PRESCALER_AHB_2 		= 0x8,					//!< Divisor of 2
	RCC_PRESCALER_AHB_4 		= 0x9,					//!< Divisor of 4
	RCC_PRESCALER_AHB_8 		= 0xA,					//!< Divisor of 8
	RCC_PRESCALER_AHB_16 		= 0xB,					//!< Divisor of 16
	RCC_PRESCALER_AHB_64 		= 0xC,					//!< Divisor of 64
	RCC_PRESCALER_AHB_128 	= 0xD,					//!< Divisor of 128
	RCC_PRESCALER_AHB_256 	= 0xE,					//!< Divisor of 256
	RCC_PRESCALER_AHB_512 	= 0xF,					//!< Divisor of 512
}Rcc_Bus_Ahb_Prescaler ;

/*!
    @brief Pll output divisors.
		@details This divisor should be used in conjuction with other
		pll scalers to ensure the system bus doesn't exceed 216Mhz.'
*/
typedef enum{
	RCC_PLL_DIVISOR_2 = 0x00,								//!< Divisor of 2 on PLL output
	RCC_PLL_DIVISOR_4 = 0x01,								//!< Divisor of 4 on PLL output
	RCC_PLL_DIVISOR_6 = 0x02,								//!< Divisor of 6 on PLL output
	RCC_PLL_DIVISOR_8 = 0x03,								//!< Divisor of 8 on PLL output
}Rcc_Pll_Divisor;

/*!
    @brief Structure containing required parameeters for RCC_setSystemClock.
		@details These parameters should be chosed to conform to the limits setout
		in the MCU datasheet. Alternatively, STM32CubeMX can be used. Multiple
		templates exist in this file for 'standard' configurations.
*/
typedef struct{
	Rcc_Sys_Clock systemClock; 					//!< The input source for the main clock
	Rcc_Pll_Input pllInput; 						//!< The input source for the PLL
	Rcc_Bus_Ahb_Prescaler ahbPrescale;	//!< The main prescaler for the AHB bus
	uint8_t pllM;												//!< VCO denominator
	uint16_t pllN;  										//!< VCO numerator
	Rcc_Pll_Divisor pllP;						     //!< General clock divisor
	uint8_t pllQ;												//!< USB OTG FS, RNG clock divisor
	uint8_t pllR;												//!< DSI clock output
	uint32_t target;										//!< The target freqeuncy, used to set flash wait states.
}Rcc_ClockConfig;

extern const Rcc_ClockConfig template_HSI_16;		//<! Predefined template for HSI clock @ 16Mhz
extern const Rcc_ClockConfig template_HSI_8; 		//<! Predefined template for HSI clock @ 16Mhz
extern const Rcc_ClockConfig template_HSI_1; 		//<! Predefined template for HSI clock @ 16Mhz
extern const Rcc_ClockConfig template_PLL_48; 	//<! Predefined template for PLL clock @ 48Mhz
extern const Rcc_ClockConfig template_PLL_108;	//<! Predefined template for PLL clock @ 108Mhz
extern const Rcc_ClockConfig template_PLL_216;	//<! Predefined template for PLL clock @ 216Mhz

/*!
    @brief Change the speed and source of the system clock.
		@details These parameters passed should be chosed to conform to the limits setout
		in the MCU datasheet. Alternatively, STM32CubeMX can be used. Multiple
		templates exist in this file for 'standard' configurations.
		in the MCU datasheet. Alternatively, STM32CubeMX can be used.
		@param config Clock configuration parameters defined by Rcc_ClockConfig
		@note This function doesn't disable any other clock sources which may
		not be used.
		@return Returns SUCCESS if new clock speed matches the target member
		of Rcc_ClockConfig. ERROR if a mismatch occurs.
*/
uint8_t RCC_setSystemClock(Rcc_ClockConfig *config);
/*!
    @brief Get the speed of a defined clock bus in Hz.
		@param clock Target alternate clock bus.
    @return The target clock speed in Hz.
*/
uint32_t RCC_getClockSpeed(Rcc_Clock clock);
/*!
    @brief Set the clock bus prescaler.
		@details Prescalers can only be set for the AHB, APB1 and APB2 peripheral busses.
		An attempt to set the prescaler on another bus will have no effect.
		@param clock Target alternate clock bus.
		@param prescaler The prescale value to set.
    @return The resulting frequency of the peripheral bus.
*/
uint32_t RCC_setBusPrescaler(Rcc_Clock clock, Rcc_Bus_Apb_Prescaler prescaler);
/*!
    @brief Set the clock speed of a bus to its maximum allowed value.
		@details This function is only relevent for the AHB1 and AHB2 peripheral
		busses. The AHB1 bus has a maximum speed of 54Mhz. The AHB2 bus has a maximum
		speed of 108Mhz.
		This function is used internally by RCC_setSystemClock to ensure the
		peripheral busses are not set at too high a frequency.
		@param clock Target alternate clock bus to set (AHB1 or AHB2).
    @return The resulting frequency of the peripheral bus.
*/
uint32_t RCC_setBusMaximum(Rcc_Clock clock);
/*!
    @brief Enables a given peripheral clock.
    @param peripheral The clock to enable, defined by #RCC_Peripheral
    @return SUCCESS if enable, FAIL if not.
*/
bool RCC_clockEnable(RCC_Peripheral peripheral);
/*!
    @brief Disables a given peripheral clock.
    @param peripheral The clock to disable, defined by #RCC_Peripheral
    @return SUCCESS if enable, FAIL if not.
*/
bool RCC_clockDisable(RCC_Peripheral peripheral);


/**@}*/
/**@}*/

#endif
