#ifndef LIB_STM32F76XXX_FLASH_H
#define LIB_STM32F76XXX_FLASH_H

/**
  @addtogroup STM32F76XXX
  @{
*/

/*!
    @addtogroup FLASH
    @brief Module for accessing and configuring the flash memeory controller.
    @author Stuart Ianna
    @date December 2019
    @warning
    @todo

  @{
*/

#include "stm32f76xxx_core.h"

/*!
    @brief Set the number of wait states used when accessing flash memory.
    @details This function should be called when the system clock frequency
		is changed. It is used internally by the RCC module.
		@warning This function assumes a CPU voltage of 3.3V. A different number
		of wait states are requred for lower voltages. See reference manual P91.
    @param hclkFrequency The new freqeuency of the system clock.
    @return none.
*/
void FLASH_setWaitStates(uint32_t hclkFrequency);

/**@}*/
/**@}*/

#endif
