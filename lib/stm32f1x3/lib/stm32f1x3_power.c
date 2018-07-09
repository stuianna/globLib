/*!**************************************************************************
    @file stm32f1x3_power.c
    @brief Source file for stm32f103 power
    @author Stuart Ianna
    @version 1.0
    @date July 2018
    @copyright GNU GPLv3
    @warning None
    @bug 

    @details

    @par Verified Compilers
    - arm-none-eabi-gcc (15:4.9.3+svn231177-1) 4.9.3 20150529 (prerelease)
******************************************************************************/

#include "stm32f1x3_power.h"


void hard_fault_handler(void){

    //Enable backup domain clocks
    rcc_periph_clock_enable(RCC_BKP);
    rcc_periph_clock_enable(RCC_PWR);

    if(BKP_DR1 != 1){

        //No bootloader used, stall here
        while(1);

    }

    //Bootloader used

    //Bootloader uses 5678 to detect reset by hard fault
    pwr_disable_backup_domain_write_protect();
    BKP_DR1 = 5678;
    pwr_enable_backup_domain_write_protect();

    //Disconnect USB and reset
    USB_disconnect();
    POWER_reset();

    //Never getting here
    while(1);
}

void POWER_reset(void){

    scb_reset_system();
}

void POWER_sleep(void){

    SCB_SCR |= SCB_SCR_SLEEPONEXIT;
    __WFI();
}

void POWER_noSleep(void){

    //Clear flag
    SCB_SCR &= ~SCB_SCR_SLEEPONEXIT;
}

void POWER_standby(void){
    
    rcc_periph_clock_enable(RCC_PWR);
    PWR_CR |= PWR_CR_CWUF;
    SCB_SCR |= SCB_SCR_SLEEPDEEP;
    PWR_CR |= PWR_CR_PDDS;
    PWR_CSR &= ~PWR_CSR_WUF;
    __WFI();
}

void POWER_stop(void){

    rcc_periph_clock_enable(RCC_PWR);
    SCB_SCR |= SCB_SCR_SLEEPDEEP;
    PWR_CR &= ~PWR_CR_PDDS;
    PWR_CR |= PWR_CR_LPDS;
    __WFI();
}

uint8_t POWER_fromStop(void){

    rcc_periph_clock_enable(RCC_PWR);

    if((PWR_CSR & PWR_CSR_WUF) || (PWR_CSR & PWR_CSR_SBF)){
        
        //Clear flag
        PWR_CR |= PWR_CR_CWUF;
        PWR_CR |= PWR_CR_CSBF;
        return 1;
    }
    return 0;

}

uint8_t POWER_fromStandby(void){

    rcc_periph_clock_enable(RCC_PWR);

    if(PWR_CSR & PWR_CSR_WUF){

        //Clear flag
        PWR_CR |= PWR_CR_CWUF;
        PWR_CR |= PWR_CR_CSBF;
        return 1;
    }

    return 0;
}

