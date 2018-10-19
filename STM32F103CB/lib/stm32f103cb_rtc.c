/*!**************************************************************************
 @file stm32f103cb_rtc.c
    @brief Source file for stm32f103cb real time clock
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

#include "stm32f103cb_rtc.h"

//This is the callback function used by systick during RTC_calibrate()
void rtc_callback(void);
static volatile uint8_t flag;

/****************************************************************************/
//			                ISR FUNCTION POINTERS
/****************************************************************************/
static void (*rtc_alarm_handler)(void);
static void (*rtc_resolution_handler)(void);
static void (*rtc_overflow_handler)(void);

/****************************************************************************/
//			                SETUP FUNCTIONS 
/****************************************************************************/
mcu_error RTC_setup(uint32_t resolution){

    if((resolution > 0xFFFFF) && MCU_debugIsEnabled()){

        MCU_printError(E_RTC_PRELOAD);
        return E_RTC_PRELOAD;
    }

    RTC_clockEnable();
    rtc_awake_from_off(RCC_LSI);
    rtc_set_counter_val(0);
    RTC_setPrescaler(resolution);

    return E_RTC_NOERROR;
}

mcu_error RTC_setPrescaler(uint32_t resolution){

    if((resolution > 0xFFFFF) && MCU_debugIsEnabled()){

        MCU_printError(E_RTC_PRELOAD);
        return E_RTC_PRELOAD;
    }

    rtc_set_prescale_val(resolution);

    return E_RTC_NOERROR;
}

//Calibrate uses systick to check the actural clock count needed
//to get the resolution
mcu_error RTC_calibrate(uint32_t resolution){

    //Make sure resolution is non zero and not too large
    if(((resolution > 0x3FFFF) || (!resolution) ) && MCU_debugIsEnabled()){

        MCU_printError(E_RTC_PRELOAD);
        return E_RTC_PRELOAD;
    }

    SYSTICK_setup(resolution*1000,&rtc_callback);
    rtc_set_counter_val(0);
    rtc_set_prescale_val(0);
    while(!flag);
    rtc_set_prescale_val(rtc_get_counter_val()-4);
    rtc_set_counter_val(0);
    SYSTICK_stop();

    return E_RTC_NOERROR;
}

void rtc_callback(void){

    flag = 1;

}

void RTC_setCount(uint32_t count){

    rtc_set_counter_val(count);

}

void RTC_clockEnable(void){

    rtc_awake_from_off(RCC_LSI);
}

uint32_t RTC_getCount(void){

    return rtc_get_counter_val();

}


/****************************************************************************/
//			            Resolution (second) ISR
/****************************************************************************/
void RTC_enableResolutionISR(void (*res_isr)(void)){

    RTC_disableResolutionISR();
    rtc_resolution_handler = res_isr;
    rtc_interrupt_enable(RTC_SEC);
    nvic_enable_irq(NVIC_RTC_IRQ);
}

void RTC_disableResolutionISR(void){

    rtc_interrupt_disable(RTC_SEC);
}

uint8_t RTC_isResolutionEnabled(void){

    return (RTC_CRH & RTC_CRH_SECIE);
}

/****************************************************************************/
//			                    ALARM ISR
/****************************************************************************/

void RTC_enableAlarmISR(uint32_t count,void (*al_isr)(void)){

    RTC_disableAlarmISR();
    rtc_alarm_handler = al_isr;
    rtc_interrupt_enable(RTC_ALR);
    rtc_set_alarm_time(count);
    nvic_enable_irq(NVIC_RTC_IRQ);
}

void RTC_disableAlarmISR(void){

    rtc_interrupt_disable(RTC_ALR);
}

uint8_t RTC_isAlarmEnabled(void){

    return (RTC_CRH & RTC_CRH_ALRIE);
}

/****************************************************************************/
//			                OVERFLOW ISR
/****************************************************************************/
void RTC_enableOverflowISR(void (*ov_isr)(void)){

    RTC_disableOverflowISR();
    rtc_overflow_handler = ov_isr;
    rtc_interrupt_enable(RTC_OW);
    nvic_enable_irq(NVIC_RTC_IRQ);
}

void RTC_disableOverflowISR(void){

    rtc_interrupt_disable(RTC_OW);
}

uint8_t RTC_isOverflowEnabled(void){

    return (RTC_CRH & RTC_CRH_OWIE);
}

/****************************************************************************/
//			                MAIN ISR
/****************************************************************************/
void rtc_isr(void){

    if(rtc_check_flag(RTC_OW)){

        rtc_clear_flag(RTC_OW);
        rtc_overflow_handler();
    }
    else if(rtc_check_flag(RTC_ALR)){

        rtc_clear_flag(RTC_ALR);
        rtc_alarm_handler();
    }
    else if(rtc_check_flag(RTC_SEC)){

        rtc_clear_flag(RTC_SEC);
        rtc_resolution_handler();
    }
}
