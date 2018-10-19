/*!**************************************************************************
    @file stm32f103cb_timer.c
    @brief Source file for stm32f103cb TIMER
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
#include "stm32f103cb_timer.h"

typedef enum{

    T1C1_STAT = 0x1,
    T1C2_STAT = 0x2,
    T1C3_STAT = 0x4,
    T1C4_STAT = 0x8,

    T2C1_STAT = 0x1,
    T2C2_STAT = 0x2,
    T2C3_STAT = 0x4,
    T2C4_STAT = 0x8,

    T3C1_STAT = 0x1,
    T3C2_STAT = 0x2,
    T3C3_STAT = 0x4,
    T3C4_STAT = 0x8,

    T4C1_STAT = 0x1,
    T4C2_STAT = 0x2,
    T4C3_STAT = 0x4,
    T4C4_STAT = 0x8,

}timer_capture;

/****************************************************************************/
//				                TIMER1
/****************************************************************************/

static mcu_error timer1_setupPWM(timer_channel timerChannel, uint32_t frequency, uint8_t duty);
static mcu_error timer1_setupPulse(timer_channel timerChannel,uint32_t frequency,uint32_t pulse);
static mcu_error timer1_setupIC(timer_channel timerChannel);

static void timer1_input_capture(void);
static void (*tim1_up_handler)(void);
static void (*tim1_cc_handler)(void);

//This is all used with input capture
static volatile uint8_t t1_status;
static volatile uint16_t t1c1_rise;
static volatile uint16_t t1c2_rise;
static volatile uint16_t t1c3_rise;
static volatile uint16_t t1c4_rise;
static volatile uint16_t t1c1_cap;
static volatile uint16_t t1c2_cap;
static volatile uint16_t t1c3_cap;
static volatile uint16_t t1c4_cap;

/****************************************************************************/
//				                TIMER2
/****************************************************************************/

static mcu_error timer2_setupPWM(timer_channel timerChannel, uint32_t frequency, uint8_t duty);
static mcu_error timer2_setupPulse(timer_channel timerChannel,uint32_t frequency,uint32_t pulse);
static mcu_error timer2_setupIC(timer_channel timerChannel);

static void timer2_input_capture(void);
static void (*tim2_up_handler)(void);

//This is all used with input capture
static volatile uint8_t t2_stat_reg;
static volatile uint8_t t2_status;
static volatile uint16_t t2c1_rise;
static volatile uint16_t t2c2_rise;
static volatile uint16_t t2c3_rise;
static volatile uint16_t t2c4_rise;
static volatile uint16_t t2c1_cap;
static volatile uint16_t t2c2_cap;
static volatile uint16_t t2c3_cap;
static volatile uint16_t t2c4_cap;

/****************************************************************************/
//				                TIMER3
/****************************************************************************/

static mcu_error timer3_setupPWM(timer_channel timerChannel, uint32_t frequency, uint8_t duty);
static mcu_error timer3_setupPulse(timer_channel timerChannel,uint32_t frequency,uint32_t pulse);
static mcu_error timer3_setupIC(timer_channel timerChannel);

static void timer3_input_capture(void);
static void (*tim3_up_handler)(void);

//This is all used with input capture
static volatile uint8_t t3_status;
static volatile uint16_t t3c1_rise;
static volatile uint16_t t3c2_rise;
static volatile uint16_t t3c3_rise;
static volatile uint16_t t3c4_rise;
static volatile uint16_t t3c1_cap;
static volatile uint16_t t3c2_cap;
static volatile uint16_t t3c3_cap;
static volatile uint16_t t3c4_cap;

/****************************************************************************/
//				                TIMER4
/****************************************************************************/

static mcu_error timer4_setupPWM(timer_channel timerChannel, uint32_t frequency, uint8_t duty);
static mcu_error timer4_setupPulse(timer_channel timerChannel,uint32_t frequency,uint32_t pulse);
static mcu_error timer4_setupIC(timer_channel timerChannel);

static void timer4_input_capture(void);
static void (*tim4_up_handler)(void);

//This is all used with input capture
static volatile uint8_t t4_status;
static volatile uint16_t t4c1_rise;
static volatile uint16_t t4c2_rise;
static volatile uint16_t t4c3_rise;
static volatile uint16_t t4c4_rise;
static volatile uint16_t t4c1_cap;
static volatile uint16_t t4c2_cap;
static volatile uint16_t t4c3_cap;
static volatile uint16_t t4c4_cap;

/****************************************************************************/
//			                    IRQ
/****************************************************************************/

static void timer1_input_capture(void){

    if((TIM_SR(TIM1) & TIM_SR_UIF)){

        //This checks if a whole period has passed with no capture event for each channel
        //Probably a better way to do this

        if(!(t1_status & T1C1_STAT)){

            t1c1_cap = 0;
        }
        if(!(t1_status & T1C2_STAT)){

            t1c2_cap = 0;
        }
        if(!(t1_status & T1C3_STAT)){

            t1c3_cap = 0;
        }
        if(!(t1_status & T1C4_STAT)){

            t1c4_cap = 0;
        }

        t1_status = 0;
    }

    if((TIM_SR(TIM1) & TIM_SR_CC1IF)){

        //Falling edge detection
        if(TIM_CCER(TIM1) & TIM_CCER_CC1P){

            t1c1_cap = TIM_CCR1(TIM1) - t1c1_rise;

            //Set rising edge
            TIM_CCER(TIM1) &= ~TIM_CCER_CC1P;

            //Mark read instance
            t1_status |= T1C1_STAT;
        }
        //Rising edge detection
        else{

            //Get rise time
            t1c1_rise = TIM_CCR1(TIM1);     

            //Set falling edge
            TIM_CCER(TIM1) |= TIM_CCER_CC1P;

            //Mark read instance
            t1_status |= T1C1_STAT;
        }

    }
    if((TIM_SR(TIM1) & TIM_SR_CC2IF)){

        //Falling edge detection
        if(TIM_CCER(TIM1) & TIM_CCER_CC2P){

            //Record the time
            t1c2_cap = TIM_CCR2(TIM1) - t1c2_rise;

            //Set rising edge
            TIM_CCER(TIM1) &= ~TIM_CCER_CC2P;

            //Mark read instance
            t1_status |= T1C2_STAT;
        }
        //Rising edge detection
        else{

            t1c2_rise = TIM_CCR2(TIM1); 

            //Set falling edge
            TIM_CCER(TIM1) |= TIM_CCER_CC2P;

            //Mark read instance
            t1_status |= T1C2_STAT;

        }
    }
    if((TIM_SR(TIM1) &TIM_SR_CC3IF)){

        //Falling edge detection
        if(TIM_CCER(TIM1) & TIM_CCER_CC3P){


            //Record the time
            t1c3_cap = TIM_CCR3(TIM1) - t1c3_rise;

            //Set rising edge
            TIM_CCER(TIM1) &= ~TIM_CCER_CC3P;

            //Mark read instance
            t1_status |= T1C3_STAT;
        }
        //Rising edge detection
        else{

            t1c3_rise = TIM_CCR3(TIM1); 

            //Set falling edge
            TIM_CCER(TIM1) |= TIM_CCER_CC3P;

            //Mark read instance
            t1_status |= T1C3_STAT;

        }
    }
    if((TIM_SR(TIM1) & TIM_SR_CC4IF)){

        //Falling edge detection
        if(TIM_CCER(TIM1) & TIM_CCER_CC4P){

            //Record the time
            t1c4_cap = TIM_CCR4(TIM1) - t1c4_rise;

            //Set rising edge
            TIM_CCER(TIM1) &= ~TIM_CCER_CC4P;

            //Mark read instance
            t1_status |= T1C4_STAT;
        }
        //Rising edge detection
        else{

            t1c4_rise = TIM_CCR4(TIM1); 

            //Set falling edge
            TIM_CCER(TIM1) |= TIM_CCER_CC4P;

            //Mark read instance
            t1_status |= T1C4_STAT;

        }

    }
}

static void timer2_input_capture(void){

    if(t2_stat_reg){

        //This checks if a whole period has passed with no capture event for each channel
        //Probably a better way to do this

        if(!(t2_status & T2C1_STAT)){

            t2c1_cap = 0;
        }
        if(!(t2_status & T2C2_STAT)){

            t2c2_cap = 0;
        }
        if(!(t2_status & T2C3_STAT)){

            t1c3_cap = 0;
        }
        if(!(t2_status & T2C4_STAT)){

            t2c4_cap = 0;
        }

        t2_status = 0;
    }

    if((TIM_SR(TIM2) & TIM_SR_CC1IF)){

        //Falling edge detection
        if(TIM_CCER(TIM2) & TIM_CCER_CC1P){

            t2c1_cap = TIM_CCR1(TIM2) - t2c1_rise;

            //Set rising edge
            TIM_CCER(TIM2) &= ~TIM_CCER_CC1P;

            //Mark read instance
            t2_status |= T2C1_STAT;
        }
        //Rising edge detection
        else{

            //Get rise time
            t2c1_rise = TIM_CCR1(TIM2);     

            //Set falling edge
            TIM_CCER(TIM2) |= TIM_CCER_CC1P;

            //Mark read instance
            t2_status |= T2C1_STAT;
        }

    }
    if((TIM_SR(TIM2) & TIM_SR_CC2IF)){

        //Falling edge detection
        if(TIM_CCER(TIM2) & TIM_CCER_CC2P){


            //Record the time
            t2c2_cap = TIM_CCR2(TIM2) - t2c2_rise;

            //Set rising edge
            TIM_CCER(TIM2) &= ~TIM_CCER_CC2P;

            //Mark read instance
            t2_status |= T2C2_STAT;
        }
        //Rising edge detection
        else{

            t2c2_rise = TIM_CCR2(TIM2); 

            //Set falling edge
            TIM_CCER(TIM2) |= TIM_CCER_CC2P;

            //Mark read instance
            t2_status |= T2C2_STAT;

        }
    }
    if((TIM_SR(TIM2) &TIM_SR_CC3IF)){

        //Falling edge detection
        if(TIM_CCER(TIM2) & TIM_CCER_CC3P){


            //Record the time
            t2c3_cap = TIM_CCR3(TIM2) - t2c3_rise;

            //Set rising edge
            TIM_CCER(TIM2) &= ~TIM_CCER_CC3P;

            //Mark read instance
            t2_status |= T2C3_STAT;
        }
        //Rising edge detection
        else{

            t2c3_rise = TIM_CCR3(TIM2); 

            //Set falling edge
            TIM_CCER(TIM2) |= TIM_CCER_CC3P;

            //Mark read instance
            t2_status |= T2C3_STAT;

        }
    }
    if((TIM_SR(TIM2) & TIM_SR_CC4IF)){

        //Falling edge detection
        if(TIM_CCER(TIM2) & TIM_CCER_CC4P){

            //Record the time
            t2c4_cap = TIM_CCR4(TIM2) - t2c4_rise;

            //Set rising edge
            TIM_CCER(TIM2) &= ~TIM_CCER_CC4P;

            //Mark read instance
            t2_status |= T2C4_STAT;
        }
        //Rising edge detection
        else{

            t2c4_rise = TIM_CCR4(TIM2); 

            //Set falling edge
            TIM_CCER(TIM2) |= TIM_CCER_CC4P;

            //Mark read instance
            t2_status |= T2C4_STAT;

        }

    }

}

static void timer3_input_capture(void){

    if((TIM_SR(TIM3) & TIM_SR_UIF)){

        //This checks if a whole period has passed with no capture event for each channel
        //Probably a better way to do this

        if(!(t3_status & T3C1_STAT)){

            t3c1_cap = 0;
        }
        if(!(t3_status & T3C2_STAT)){

            t3c2_cap = 0;
        }
        if(!(t3_status & T3C3_STAT)){

            t3c3_cap = 0;
        }
        if(!(t3_status & T3C4_STAT)){

            t3c4_cap = 0;
        }

        t3_status = 0;
    }

    if((TIM_SR(TIM3) & TIM_SR_CC1IF)){

        //Falling edge detection
        if(TIM_CCER(TIM3) & TIM_CCER_CC1P){

            t3c1_cap = TIM_CCR1(TIM3) - t3c1_rise;

            //Set rising edge
            TIM_CCER(TIM3) &= ~TIM_CCER_CC1P;

            //Mark read instance
            t3_status |= T3C1_STAT;
        }
        //Rising edge detection
        else{

            //Get rise time
            t3c1_rise = TIM_CCR1(TIM3);     

            //Set falling edge
            TIM_CCER(TIM3) |= TIM_CCER_CC1P;

            //Mark read instance
            t3_status |= T3C1_STAT;
        }

    }
    if((TIM_SR(TIM3) & TIM_SR_CC2IF)){

        //Falling edge detection
        if(TIM_CCER(TIM3) & TIM_CCER_CC2P){


            //Record the time
            t3c2_cap = TIM_CCR2(TIM3) - t3c2_rise;

            //Set rising edge
            TIM_CCER(TIM3) &= ~TIM_CCER_CC2P;

            //Mark read instance
            t3_status |= T3C2_STAT;
        }
        //Rising edge detection
        else{

            t3c2_rise = TIM_CCR2(TIM3); 

            //Set falling edge
            TIM_CCER(TIM3) |= TIM_CCER_CC2P;

            //Mark read instance
            t3_status |= T3C2_STAT;

        }
    }
    if((TIM_SR(TIM3) &TIM_SR_CC3IF)){

        //Falling edge detection
        if(TIM_CCER(TIM3) & TIM_CCER_CC3P){


            //Record the time
            t3c3_cap = TIM_CCR3(TIM3) - t3c3_rise;

            //Set rising edge
            TIM_CCER(TIM3) &= ~TIM_CCER_CC3P;

            //Mark read instance
            t3_status |= T2C3_STAT;
        }
        //Rising edge detection
        else{

            t3c3_rise = TIM_CCR3(TIM3); 

            //Set falling edge
            TIM_CCER(TIM3) |= TIM_CCER_CC3P;

            //Mark read instance
            t3_status |= T3C3_STAT;

        }
    }
    if((TIM_SR(TIM3) & TIM_SR_CC4IF)){

        //Falling edge detection
        if(TIM_CCER(TIM3) & TIM_CCER_CC4P){

            //Record the time
            t3c4_cap = TIM_CCR4(TIM3) - t3c4_rise;

            //Set rising edge
            TIM_CCER(TIM3) &= ~TIM_CCER_CC4P;

            //Mark read instance
            t3_status |= T3C4_STAT;
        }
        //Rising edge detection
        else{

            t3c4_rise = TIM_CCR4(TIM3); 

            //Set falling edge
            TIM_CCER(TIM3) |= TIM_CCER_CC4P;

            //Mark read instance
            t3_status |= T3C4_STAT;

        }

    }
}

static void timer4_input_capture(void){

    if((TIM_SR(TIM4) & TIM_SR_UIF)){

        //This checks if a whole period has passed with no capture event for each channel
        //Probably a better way to do this

        if(!(t4_status & T4C1_STAT)){

            t4c1_cap = 0;
        }
        if(!(t4_status & T4C2_STAT)){

            t4c2_cap = 0;
        }
        if(!(t4_status & T4C3_STAT)){

            t4c3_cap = 0;
        }
        if(!(t4_status & T4C4_STAT)){

            t4c4_cap = 0;
        }

        t4_status = 0;
    }

    if((TIM_SR(TIM4) & TIM_SR_CC1IF)){

        //Falling edge detection
        if(TIM_CCER(TIM4) & TIM_CCER_CC1P){

            t4c1_cap = TIM_CCR1(TIM4) - t4c1_rise;

            //Set rising edge
            TIM_CCER(TIM4) &= ~TIM_CCER_CC1P;

            //Mark read instance
            t4_status |= T4C1_STAT;
        }
        //Rising edge detection
        else{

            //Get rise time
            t4c1_rise = TIM_CCR1(TIM4);     

            //Set falling edge
            TIM_CCER(TIM4) |= TIM_CCER_CC1P;

            //Mark read instance
            t4_status |= T4C1_STAT;
        }

    }
    if((TIM_SR(TIM4) & TIM_SR_CC2IF)){

        //Falling edge detection
        if(TIM_CCER(TIM4) & TIM_CCER_CC2P){


            //Record the time
            t4c2_cap = TIM_CCR2(TIM4) - t4c2_rise;

            //Set rising edge
            TIM_CCER(TIM4) &= ~TIM_CCER_CC2P;

            //Mark read instance
            t4_status |= T4C2_STAT;
        }
        //Rising edge detection
        else{

            t4c2_rise = TIM_CCR2(TIM4); 

            //Set falling edge
            TIM_CCER(TIM4) |= TIM_CCER_CC2P;

            //Mark read instance
            t4_status |= T4C2_STAT;

        }
    }
    if((TIM_SR(TIM4) &TIM_SR_CC3IF)){

        //Falling edge detection
        if(TIM_CCER(TIM4) & TIM_CCER_CC3P){


            //Record the time
            t4c3_cap = TIM_CCR3(TIM4) - t4c3_rise;

            //Set rising edge
            TIM_CCER(TIM4) &= ~TIM_CCER_CC3P;

            //Mark read instance
            t4_status |= T2C3_STAT;
        }
        //Rising edge detection
        else{

            t4c3_rise = TIM_CCR3(TIM4); 

            //Set falling edge
            TIM_CCER(TIM4) |= TIM_CCER_CC3P;

            //Mark read instance
            t4_status |= T4C3_STAT;

        }
    }
    if((TIM_SR(TIM4) & TIM_SR_CC4IF)){

        //Falling edge detection
        if(TIM_CCER(TIM4) & TIM_CCER_CC4P){

            //Record the time
            t4c4_cap = TIM_CCR4(TIM4) - t4c4_rise;

            //Set rising edge
            TIM_CCER(TIM4) &= ~TIM_CCER_CC4P;

            //Mark read instance
            t4_status |= T4C4_STAT;
        }
        //Rising edge detection
        else{

            t4c4_rise = TIM_CCR4(TIM4); 

            //Set falling edge
            TIM_CCER(TIM4) |= TIM_CCER_CC4P;

            //Mark read instance
            t4_status |= T4C4_STAT;

        }

    }
}

mcu_error TIMER_enableISR(timer_main timerNumber, void(*handler)(void)){

    mcu_error error;

    switch(timerNumber){

        case TIMER_1:

            nvic_disable_irq(NVIC_TIM1_UP_IRQ);
            timer_disable_irq(TIM1,TIM_DIER_UIE);
            tim1_up_handler = handler;
            timer_enable_irq(TIM1,TIM_DIER_UIE);
            nvic_enable_irq(NVIC_TIM1_UP_IRQ);
            break;

        case TIMER_2:

            nvic_disable_irq(NVIC_TIM2_IRQ);
            timer_disable_irq(TIM2,TIM_DIER_UIE);
            tim2_up_handler = handler;
            timer_enable_irq(TIM2,TIM_DIER_UIE);
            nvic_enable_irq(NVIC_TIM2_IRQ);
            break;

        case TIMER_3:

            nvic_disable_irq(NVIC_TIM3_IRQ);
            timer_disable_irq(TIM3,TIM_DIER_UIE);
            tim3_up_handler = handler;
            timer_enable_irq(TIM3,TIM_DIER_UIE);
            nvic_enable_irq(NVIC_TIM3_IRQ);
            break;

        case TIMER_4:

            nvic_disable_irq(NVIC_TIM4_IRQ);
            timer_disable_irq(TIM4,TIM_DIER_UIE);
            tim4_up_handler = handler;
            timer_enable_irq(TIM4,TIM_DIER_UIE);
            nvic_enable_irq(NVIC_TIM4_IRQ);
            break;

        default:
			if(MCU_debugIsEnabled()){
				MCU_printError(E_TIMER_NOTIMER);
			}
            error = E_TIMER_NOTIMER;

            break;
    }

    return error;

}

mcu_error TIMER_disableISR(timer_main timerNumber){

    mcu_error error;

    switch(timerNumber){

        case TIMER_1:

            nvic_disable_irq(NVIC_TIM1_UP_IRQ);
            timer_disable_irq(TIM1,TIM_DIER_UIE);

            break;
        case TIMER_2:

            nvic_disable_irq(NVIC_TIM2_IRQ);
            timer_disable_irq(TIM2,TIM_DIER_UIE);
            break;
        case TIMER_3:

            nvic_disable_irq(NVIC_TIM3_IRQ);
            timer_disable_irq(TIM3,TIM_DIER_UIE);
            break;
        case TIMER_4:

            nvic_disable_irq(NVIC_TIM4_IRQ);
            timer_disable_irq(TIM4,TIM_DIER_UIE);
            break;
        default:
			if(MCU_debugIsEnabled()){
				MCU_printError(E_TIMER_NOTIMER);
			}
            error = E_TIMER_NOTIMER;

            break;
    }

    return error;

}

void tim1_cc_isr(void){

    tim1_cc_handler();
    timer_clear_flag(TIM1,TIM_SR_UIF);
} 

void tim1_up_isr(void){

    tim1_up_handler();
    timer_clear_flag(TIM1,TIM_SR_UIF);
}

void tim2_isr(void){

    t2_stat_reg = TIM_SR(TIM2) & TIM_SR_UIF;
    timer_clear_flag(TIM2,TIM_SR_UIF);
    tim2_up_handler();
}

void tim3_isr(void){

    timer_clear_flag(TIM3,TIM_SR_UIF);
    tim3_up_handler();
}

void tim4_isr(void){

    timer_clear_flag(TIM4,TIM_SR_UIF);
    tim4_up_handler();
}
/****************************************************************************/
//			                PAUSE RESUME
/****************************************************************************/

mcu_error TIMER_pause(timer_main timerNumber){

    mcu_error error = E_TIMER_NOERROR;

    switch(timerNumber){

        case TIMER_1:

            timer_disable_counter(TIM1);
            break;
        case TIMER_2:
            
            timer_disable_counter(TIM2);
            break;
        case TIMER_3:

            timer_disable_counter(TIM3);
            break;
        case TIMER_4:

            timer_disable_counter(TIM4);
            break;
        default:
			if(MCU_debugIsEnabled()){
				MCU_printError(E_TIMER_NOTIMER);
			}
            error = E_TIMER_NOTIMER;

            break;
    }

    return error;
}

mcu_error TIMER_resume(timer_main timerNumber){

    mcu_error error = E_TIMER_NOERROR;

    switch(timerNumber){

        case TIMER_1:

            timer_enable_counter(TIM1);
            break;
        case TIMER_2:
            
            timer_enable_counter(TIM2);
            break;
        case TIMER_3:

            timer_enable_counter(TIM3);
            break;
        case TIMER_4:

            timer_enable_counter(TIM4);
            break;
        default:
			if(MCU_debugIsEnabled()){
				MCU_printError(E_TIMER_NOTIMER);
			}
            error = E_TIMER_NOTIMER;

            break;
    }

    return error;
}

/****************************************************************************/
//			                SETUP FUNCTIONS
/****************************************************************************/

mcu_error TIMER_setupIC(timer_main timerNumber, timer_channel timerChannel){

    mcu_error error = E_TIMER_NOERROR;

    switch(timerNumber){

        case TIMER_1:

                error = timer1_setupIC(timerChannel);
            break;
        case TIMER_2:

                error = timer2_setupIC(timerChannel);
                TIMER_enableISR(TIMER_2,&timer2_input_capture);
            break;
        case TIMER_3:

                error = timer3_setupIC(timerChannel);
                TIMER_enableISR(TIMER_3,&timer3_input_capture);
            break;
        case TIMER_4:

                error = timer4_setupIC(timerChannel);
                TIMER_enableISR(TIMER_4,&timer4_input_capture);
                break;
        default:
			if(MCU_debugIsEnabled()){
				MCU_printError(E_TIMER_NOTIMER);
			}
            error = E_TIMER_NOTIMER;

            break;
    }

    return error;
}

mcu_error TIMER_setupPWM(timer_main timerNumber, timer_channel timerChannel, uint32_t frequency, uint8_t duty){

    mcu_error error = E_TIMER_NOERROR;

    switch(timerNumber){

        case TIMER_1:

                error = timer1_setupPWM(timerChannel,frequency,duty);
            break;
        case TIMER_2:

                error = timer2_setupPWM(timerChannel,frequency,duty);
            break;
        case TIMER_3:

                error = timer3_setupPWM(timerChannel,frequency,duty);
            break;
        case TIMER_4:

                error = timer4_setupPWM(timerChannel,frequency,duty);
                break;
        default:
			if(MCU_debugIsEnabled()){
				MCU_printError(E_TIMER_NOTIMER);
			}
            error = E_TIMER_NOTIMER;

            break;
    }

    return error;
}
mcu_error TIMER_setupPulse(timer_main timerNumber, timer_channel timerChannel, uint32_t frequency, uint32_t pulse){

    mcu_error error = E_TIMER_NOERROR;

    switch(timerNumber){

        case TIMER_1:

                error = timer1_setupPulse(timerChannel,frequency,pulse);
            break;
        case TIMER_2:

                error = timer2_setupPulse(timerChannel,frequency,pulse);
            break;
        case TIMER_3:

                error = timer3_setupPulse(timerChannel,frequency,pulse);
            break;
        case TIMER_4:

                error = timer4_setupPulse(timerChannel,frequency,pulse);
                break;
        default:
			if(MCU_debugIsEnabled()){
				MCU_printError(E_TIMER_NOTIMER);
			}
            error = E_TIMER_NOTIMER;

            break;
    }

    return error;
}

mcu_error TIMER_setupCount(timer_main timerNumber, uint32_t frequency, void (*handler)(void)){

    mcu_error error = E_TIMER_NOERROR;

    switch(timerNumber){

        case TIMER_1:

            //Set the clock
            rcc_periph_clock_enable(RCC_TIM1);

            //Setup main timer
            timer_disable_counter(TIM1);
            timer_reset(TIM1);
            timer_set_mode(TIM1, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE,TIM_CR1_DIR_UP);
            TIMER_setFrequency(TIMER_1,frequency);

            TIMER_enableISR(timerNumber,handler);

            timer_enable_counter(TIM1);

            break;
        case TIMER_2:

            //Set the clock
            rcc_periph_clock_enable(RCC_TIM2);

            //Setup main timer
            timer_disable_counter(TIM2);
            timer_reset(TIM2);
            timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE,TIM_CR1_DIR_UP);
            TIMER_setFrequency(timerNumber ,frequency);

            TIMER_enableISR(timerNumber,handler);

            timer_enable_counter(TIM2);

            break;
        case TIMER_3:

            //Set the clock
            rcc_periph_clock_enable(RCC_TIM3);

            //Setup main timer
            timer_disable_counter(TIM3);
            timer_reset(TIM3);
            timer_set_mode(TIM3, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE,TIM_CR1_DIR_UP);
            TIMER_setFrequency(timerNumber, frequency);

            TIMER_enableISR(timerNumber,handler);

            timer_enable_counter(TIM3);
            break;

        case TIMER_4:

            //Set the clock
            rcc_periph_clock_enable(RCC_TIM4);

            //Setup main timer
            timer_disable_counter(TIM4);
            timer_reset(TIM4);
            timer_set_mode(TIM4, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE,TIM_CR1_DIR_UP);
            TIMER_setFrequency(timerNumber, frequency);

            TIMER_enableISR(timerNumber,handler);

            timer_enable_counter(TIM4);
            break;

        default:
			if(MCU_debugIsEnabled()){
				MCU_printError(E_TIMER_NOTIMER);
			}
            error = E_TIMER_NOTIMER;

            break;
    }

    return error;


}

static mcu_error timer1_setupIC(timer_channel timerChannel){

    mcu_error error;

    //Set the clock
    rcc_periph_clock_enable(RCC_TIM1);

    //Setup main timer
    timer_disable_counter(TIM1);
    timer_set_mode(TIM1, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE,TIM_CR1_DIR_UP);
    TIMER_setPeriod(TIMER_1,0xFFFF);

    switch(timerChannel){

        case CHANNEL_1:

            pinSetup(GPIO_IC,PORTA,PIN8);

            TIM_CCMR1(TIM1) |= TIM_CCMR1_CC1S_IN_TI1;
            TIM_CCER(TIM1) |= TIM_CCER_CC1E;

            //Set inital trigger as rising
            TIM_CCER(TIM1) &= ~TIM_CCER_CC1P;

            //Interrupts have different configuration for advanced timers
            nvic_disable_irq(NVIC_TIM1_CC_IRQ);
            timer_enable_irq(TIM1,TIM_DIER_CC1IE);
            tim1_cc_handler = timer1_input_capture;
            nvic_enable_irq(NVIC_TIM1_CC_IRQ);

            timer_enable_counter(TIM1);
            break;

        case CHANNEL_2:

            pinSetup(GPIO_IC,PORTA,PIN9);

            TIM_CCMR1(TIM1) |= TIM_CCMR1_CC2S_IN_TI2;
            TIM_CCER(TIM1) |= TIM_CCER_CC2E;

            //Set inital trigger as rising
            TIM_CCER(TIM1) &= ~TIM_CCER_CC2P;

            //Interrupts have different configuration for advanced timers
            nvic_disable_irq(NVIC_TIM1_CC_IRQ);
            timer_enable_irq(TIM1,TIM_DIER_CC2IE);
            tim1_cc_handler = timer1_input_capture;
            nvic_enable_irq(NVIC_TIM1_CC_IRQ);

            timer_enable_counter(TIM1);
            break;

        case CHANNEL_3:

            pinSetup(GPIO_IC,PORTA,PIN10);

            TIM_CCMR2(TIM1) |= TIM_CCMR2_CC3S_IN_TI3;
            TIM_CCER(TIM1) |= TIM_CCER_CC3E;

            //Set inital trigger as rising
            TIM_CCER(TIM1) &= ~TIM_CCER_CC3P;

            //Interrupts have different configuration for advanced timers
            nvic_disable_irq(NVIC_TIM1_CC_IRQ);
            timer_enable_irq(TIM1,TIM_DIER_CC3IE);
            tim1_cc_handler = timer1_input_capture;
            nvic_enable_irq(NVIC_TIM1_CC_IRQ);

            timer_enable_counter(TIM1);

            break;
        case CHANNEL_4:

            pinSetup(GPIO_IC,PORTA,PIN11);

            TIM_CCMR2(TIM1) |= TIM_CCMR2_CC4S_IN_TI4;
            TIM_CCER(TIM1) |= TIM_CCER_CC4E;

            //Set inital trigger as rising
            TIM_CCER(TIM1) &= ~TIM_CCER_CC4P;

            //Interrupts have different configuration for advanced timers
            nvic_disable_irq(NVIC_TIM1_CC_IRQ);
            timer_enable_irq(TIM1,TIM_DIER_CC4IE);
            tim1_cc_handler = timer1_input_capture;
            nvic_enable_irq(NVIC_TIM1_CC_IRQ);

            timer_enable_counter(TIM1);
            break;

        default:
			if(MCU_debugIsEnabled()){
				MCU_printError(E_TIMER_NOCHANNEL);
			}
            error = E_TIMER_NOCHANNEL;
            break;
    }

    return error;
}

static mcu_error timer2_setupIC(timer_channel timerChannel){

    mcu_error error;

    //Set the clock
    rcc_periph_clock_enable(RCC_TIM2);

    //Setup main timer
    timer_disable_counter(TIM2);
    timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE,TIM_CR1_DIR_UP);
    TIMER_setPeriod(TIMER_2,0xFFFF);

    switch(timerChannel){

        case CHANNEL_1:

            pinSetup(GPIO_IC,PORTA,PIN0);

            TIM_CCMR1(TIM2) |= TIM_CCMR1_CC1S_IN_TI1;
            TIM_CCER(TIM2) |= TIM_CCER_CC1E;

            //Set inital trigger as rising
            TIM_CCER(TIM2) &= ~TIM_CCER_CC1P;

            timer_enable_irq(TIM2,TIM_DIER_CC1IE);

            timer_enable_counter(TIM2);
            break;

        case CHANNEL_2:

            pinSetup(GPIO_IC,PORTA,PIN1);

            TIM_CCMR1(TIM2) |= TIM_CCMR1_CC2S_IN_TI2;
            TIM_CCER(TIM2) |= TIM_CCER_CC2E;

            //Set inital trigger as rising
            TIM_CCER(TIM2) &= ~TIM_CCER_CC2P;

            timer_enable_irq(TIM2,TIM_DIER_CC2IE);

            timer_enable_counter(TIM2);
            break;

        case CHANNEL_3:

            pinSetup(GPIO_IC,PORTA,PIN2);

            TIM_CCMR2(TIM2) |= TIM_CCMR2_CC3S_IN_TI3;
            TIM_CCER(TIM2) |= TIM_CCER_CC3E;

            //Set inital trigger as rising
            TIM_CCER(TIM2) &= ~TIM_CCER_CC3P;

            timer_enable_irq(TIM2,TIM_DIER_CC3IE);

            timer_enable_counter(TIM2);

            break;
        case CHANNEL_4:

            pinSetup(GPIO_IC,PORTA,PIN3);

            TIM_CCMR2(TIM2) |= TIM_CCMR2_CC4S_IN_TI4;
            TIM_CCER(TIM2) |= TIM_CCER_CC4E;

            //Set inital trigger as rising
            TIM_CCER(TIM2) &= ~TIM_CCER_CC4P;

            timer_enable_irq(TIM2,TIM_DIER_CC4IE);

            timer_enable_counter(TIM2);
            break;

        default:
			if(MCU_debugIsEnabled()){
				MCU_printError(E_TIMER_NOCHANNEL);
			}
            error = E_TIMER_NOCHANNEL;
            break;
    }

    return error;
}

static mcu_error timer3_setupIC(timer_channel timerChannel){

    mcu_error error;

    //Set the clock
    rcc_periph_clock_enable(RCC_TIM3);

    //Setup main timer
    timer_disable_counter(TIM3);
    timer_set_mode(TIM3, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE,TIM_CR1_DIR_UP);
    TIMER_setPeriod(TIMER_3,0xFFFF);

    switch(timerChannel){

        case CHANNEL_1:

            pinSetup(GPIO_IC,PORTA,PIN6);

            TIM_CCMR1(TIM3) |= TIM_CCMR1_CC1S_IN_TI1;
            TIM_CCER(TIM3) |= TIM_CCER_CC1E;

            //Set inital trigger as rising
            TIM_CCER(TIM3) &= ~TIM_CCER_CC1P;

            timer_enable_irq(TIM3,TIM_DIER_CC1IE);

            timer_enable_counter(TIM3);
            break;

        case CHANNEL_2:

            pinSetup(GPIO_IC,PORTA,PIN7);

            TIM_CCMR1(TIM3) |= TIM_CCMR1_CC2S_IN_TI2;
            TIM_CCER(TIM3) |= TIM_CCER_CC2E;

            //Set inital trigger as rising
            TIM_CCER(TIM3) &= ~TIM_CCER_CC2P;

            timer_enable_irq(TIM3,TIM_DIER_CC2IE);

            timer_enable_counter(TIM3);
            break;

        case CHANNEL_3:

            pinSetup(GPIO_IC,PORTB,PIN0);

            TIM_CCMR2(TIM3) |= TIM_CCMR2_CC3S_IN_TI3;
            TIM_CCER(TIM3) |= TIM_CCER_CC3E;

            //Set inital trigger as rising
            TIM_CCER(TIM3) &= ~TIM_CCER_CC3P;

            timer_enable_irq(TIM3,TIM_DIER_CC3IE);

            timer_enable_counter(TIM3);

            break;
        case CHANNEL_4:

            pinSetup(GPIO_IC,PORTB,PIN1);

            TIM_CCMR2(TIM3) |= TIM_CCMR2_CC4S_IN_TI4;
            TIM_CCER(TIM3) |= TIM_CCER_CC4E;

            //Set inital trigger as rising
            TIM_CCER(TIM3) &= ~TIM_CCER_CC4P;

            timer_enable_irq(TIM3,TIM_DIER_CC4IE);

            timer_enable_counter(TIM3);
            break;

        default:
			if(MCU_debugIsEnabled()){
				MCU_printError(E_TIMER_NOCHANNEL);
			}
            error = E_TIMER_NOCHANNEL;
            break;
    }

    return error;
}

static mcu_error timer4_setupIC(timer_channel timerChannel){

    mcu_error error;

    //Set the clock
    rcc_periph_clock_enable(RCC_TIM4);

    //Setup main timer
    timer_disable_counter(TIM4);
    timer_set_mode(TIM4, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE,TIM_CR1_DIR_UP);
    TIMER_setPeriod(TIMER_4,0xFFFF);

    switch(timerChannel){

        case CHANNEL_1:

            pinSetup(GPIO_IC,PORTB,PIN6);

            TIM_CCMR1(TIM4) |= TIM_CCMR1_CC1S_IN_TI1;
            TIM_CCER(TIM4) |= TIM_CCER_CC1E;

            //Set inital trigger as rising
            TIM_CCER(TIM4) &= ~TIM_CCER_CC1P;

            timer_enable_irq(TIM4,TIM_DIER_CC1IE);

            timer_enable_counter(TIM4);
            break;

        case CHANNEL_2:

            pinSetup(GPIO_IC,PORTB,PIN7);

            TIM_CCMR1(TIM4) |= TIM_CCMR1_CC2S_IN_TI2;
            TIM_CCER(TIM4) |= TIM_CCER_CC2E;

            //Set inital trigger as rising
            TIM_CCER(TIM4) &= ~TIM_CCER_CC2P;

            timer_enable_irq(TIM4,TIM_DIER_CC2IE);

            timer_enable_counter(TIM4);
            break;

        case CHANNEL_3:

            pinSetup(GPIO_IC,PORTB,PIN8);

            TIM_CCMR2(TIM4) |= TIM_CCMR2_CC3S_IN_TI3;
            TIM_CCER(TIM4) |= TIM_CCER_CC3E;

            //Set inital trigger as rising
            TIM_CCER(TIM4) &= ~TIM_CCER_CC3P;

            timer_enable_irq(TIM4,TIM_DIER_CC3IE);

            timer_enable_counter(TIM4);

            break;
        case CHANNEL_4:

            pinSetup(GPIO_IC,PORTB,PIN9);

            TIM_CCMR2(TIM4) |= TIM_CCMR2_CC4S_IN_TI4;
            TIM_CCER(TIM4) |= TIM_CCER_CC4E;

            //Set inital trigger as rising
            TIM_CCER(TIM4) &= ~TIM_CCER_CC4P;

            timer_enable_irq(TIM4,TIM_DIER_CC4IE);

            timer_enable_counter(TIM4);
            break;

        default:
			if(MCU_debugIsEnabled()){
				MCU_printError(E_TIMER_NOCHANNEL);
			}
            error = E_TIMER_NOCHANNEL;
            break;
    }

    return error;

}

uint16_t TIMER_getIC(timer_main timerNumber, timer_channel channel){

    switch(timerNumber){

        case TIMER_1:

            switch(channel){

                case CHANNEL_1:

                        return t1c1_cap;
                        break;

                case CHANNEL_2:

                        return t1c2_cap;
                        break;

                case CHANNEL_3:

                        return t1c3_cap;
                        break;
                      
                case CHANNEL_4:

                        return t1c4_cap;
                        break;
            }

            break;
        case TIMER_2:

            switch(channel){

                case CHANNEL_1:

                        return t2c1_cap;
                        break;

                case CHANNEL_2:

                        return t2c2_cap;
                        break;

                case CHANNEL_3:

                        return t2c3_cap;
                        break;
                      
                case CHANNEL_4:

                        return t2c4_cap;
                        break;
            }
            break;
        case TIMER_3:

            switch(channel){

                case CHANNEL_1:

                        return t3c1_cap;
                        break;

                case CHANNEL_2:

                        return t3c2_cap;
                        break;

                case CHANNEL_3:

                        return t3c3_cap;
                        break;
                      
                case CHANNEL_4:

                        return t3c4_cap;
                        break;
            }

            break;
        case TIMER_4:

            switch(channel){

                case CHANNEL_1:

                        return t4c1_cap;
                        break;

                case CHANNEL_2:

                        return t4c2_cap;
                        break;

                case CHANNEL_3:

                        return t4c3_cap;
                        break;
                      
                case CHANNEL_4:

                        return t4c4_cap;
                        break;
            }
            break;

        default:
			if(MCU_debugIsEnabled()){
				MCU_printError(E_TIMER_NOTIMER);
			}
            return E_TIMER_NOTIMER;

            break;
    }

    return E_TIMER_NOERROR;


}

mcu_error timer1_setupPWM(timer_channel timerChannel, uint32_t frequency, uint8_t duty){

    mcu_error error;

    //Set the clock
    rcc_periph_clock_enable(RCC_TIM1);

    //Setup main timer
    timer_disable_counter(TIM1);
    timer_set_mode(TIM1, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE,TIM_CR1_DIR_DOWN);
    timer_enable_break_main_output(TIM1);

    switch(timerChannel){

        case CHANNEL_1:

            pinSetup(GPIO_PWM,PORTA,PIN8);

            timer_set_oc_mode(TIM1, TIM_OC1, TIM_OCM_PWM1);
            timer_enable_oc_output(TIM1, TIM_OC1);
            TIMER_setPeriod(TIMER_1,(uint32_t)(1000000.0f/(float)frequency));
            TIMER_setDuty(TIMER_1,CHANNEL_1,duty);
            timer_enable_counter(TIM1);
            break;

        case CHANNEL_2:

            pinSetup(GPIO_PWM,PORTA,PIN9);

            timer_set_oc_mode(TIM1, TIM_OC2, TIM_OCM_PWM1);
            timer_enable_oc_output(TIM1, TIM_OC2);
            TIMER_setPeriod(TIMER_1,(uint32_t)(1000000.0f/(float)frequency));
            TIMER_setDuty(TIMER_1,CHANNEL_2,duty);
            timer_enable_counter(TIM1);
            break;
        case CHANNEL_3:

            pinSetup(GPIO_PWM,PORTA,PIN10);

            timer_set_oc_mode(TIM1, TIM_OC3, TIM_OCM_PWM1);
            timer_enable_oc_output(TIM1, TIM_OC3);
            TIMER_setPeriod(TIMER_1,(uint32_t)(1000000.0f/(float)frequency));
            TIMER_setDuty(TIMER_1,CHANNEL_3,duty);
            timer_enable_counter(TIM1);
            break;
        case CHANNEL_4:

            pinSetup(GPIO_PWM,PORTA,PIN11);

            timer_set_oc_mode(TIM1, TIM_OC4, TIM_OCM_PWM1);
            timer_enable_oc_output(TIM1, TIM_OC4);
            TIMER_setPeriod(TIMER_1,(uint32_t)(1000000.0f/(float)frequency));
            TIMER_setDuty(TIMER_1,CHANNEL_4,duty);
            timer_enable_counter(TIM1);
            break;

        default:
			if(MCU_debugIsEnabled()){
				MCU_printError(E_TIMER_NOCHANNEL);
			}
            error = E_TIMER_NOCHANNEL;
            break;
    }

    return error;

}


mcu_error timer2_setupPWM(timer_channel timerChannel, uint32_t frequency, uint8_t duty){

    mcu_error error;

    //Set the clock
    rcc_periph_clock_enable(RCC_TIM2);

    //Setup main timer
    timer_disable_counter(TIM2);
    timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE,TIM_CR1_DIR_DOWN);

    switch(timerChannel){

        case CHANNEL_1:

            pinSetup(GPIO_PWM,PORTA,PIN0);

            timer_set_oc_mode(TIM2, TIM_OC1, TIM_OCM_PWM1);
            timer_enable_oc_output(TIM2, TIM_OC1);
            TIMER_setPeriod(TIMER_2,(uint32_t)(1000000.0f/(float)frequency));
            TIMER_setDuty(TIMER_2,CHANNEL_1,duty);
            timer_enable_counter(TIM2);
            break;

        case CHANNEL_2:

            pinSetup(GPIO_PWM,PORTA,PIN1);

            timer_set_oc_mode(TIM2, TIM_OC2, TIM_OCM_PWM1);
            timer_enable_oc_output(TIM2, TIM_OC2);
            TIMER_setPeriod(TIMER_2,(uint32_t)(1000000.0f/(float)frequency));
            TIMER_setDuty(TIMER_2,CHANNEL_2,duty);
            timer_enable_counter(TIM2);
            break;
            
        case CHANNEL_3:

            pinSetup(GPIO_PWM,PORTA,PIN2);

            timer_set_oc_mode(TIM2, TIM_OC3, TIM_OCM_PWM1);
            timer_enable_oc_output(TIM2, TIM_OC3);
            TIMER_setPeriod(TIMER_2,(uint32_t)(1000000.0f/(float)frequency));
            TIMER_setDuty(TIMER_2,CHANNEL_3,duty);
            timer_enable_counter(TIM2);
            break;
        case CHANNEL_4:

            pinSetup(GPIO_PWM,PORTA,PIN3);

            timer_set_oc_mode(TIM2, TIM_OC4, TIM_OCM_PWM1);
            timer_enable_oc_output(TIM2, TIM_OC4);
            TIMER_setPeriod(TIMER_2,(uint32_t)(1000000.0f/(float)frequency));
            TIMER_setDuty(TIMER_2,CHANNEL_4,duty);
            timer_enable_counter(TIM2);
            break;

        default:
			if(MCU_debugIsEnabled()){
				MCU_printError(E_TIMER_NOCHANNEL);
			}
            error = E_TIMER_NOCHANNEL;
            break;
    }

    return error;
}

mcu_error timer3_setupPWM(timer_channel timerChannel, uint32_t frequency, uint8_t duty){

    mcu_error error;

    //Set the clock
    rcc_periph_clock_enable(RCC_TIM3);

    //Setup main timer
    timer_disable_counter(TIM3);
    timer_set_mode(TIM3, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE,TIM_CR1_DIR_DOWN);

    switch(timerChannel){

        case CHANNEL_1:

            pinSetup(GPIO_PWM,PORTA,PIN6);

            timer_set_oc_mode(TIM3, TIM_OC1, TIM_OCM_PWM1);
            timer_enable_oc_output(TIM3, TIM_OC1);
            TIMER_setPeriod(TIMER_3,(uint32_t)(1000000.0f/(float)frequency));
            TIMER_setDuty(TIMER_3,CHANNEL_1,duty);
            timer_enable_counter(TIM3);
            break;

        case CHANNEL_2:

            pinSetup(GPIO_PWM,PORTA,PIN7);

            timer_set_oc_mode(TIM3, TIM_OC2, TIM_OCM_PWM1);
            timer_enable_oc_output(TIM3, TIM_OC2);
            TIMER_setPeriod(TIMER_3,(uint32_t)(1000000.0f/(float)frequency));
            TIMER_setDuty(TIMER_3,CHANNEL_2,duty);
            timer_enable_counter(TIM3);
            break;
            
        case CHANNEL_3:

            pinSetup(GPIO_PWM,PORTB,PIN0);

            timer_set_oc_mode(TIM3, TIM_OC3, TIM_OCM_PWM1);
            timer_enable_oc_output(TIM3, TIM_OC3);
            TIMER_setPeriod(TIMER_3,(uint32_t)(1000000.0f/(float)frequency));
            TIMER_setDuty(TIMER_3,CHANNEL_3,duty);
            timer_enable_counter(TIM3);
            break;

        case CHANNEL_4:

            pinSetup(GPIO_PWM,PORTB,PIN1);

            timer_set_oc_mode(TIM3, TIM_OC4, TIM_OCM_PWM1);
            timer_enable_oc_output(TIM3, TIM_OC4);
            TIMER_setPeriod(TIMER_3,(uint32_t)(1000000.0f/(float)frequency));
            TIMER_setDuty(TIMER_3,CHANNEL_4,duty);
            timer_enable_counter(TIM3);
            break;

        default:
			if(MCU_debugIsEnabled()){
				MCU_printError(E_TIMER_NOCHANNEL);
			}
            error = E_TIMER_NOCHANNEL;
            break;
    }

    return error;

}

mcu_error timer4_setupPWM(timer_channel timerChannel, uint32_t frequency, uint8_t duty){

    mcu_error error;

    //Set the clock
    rcc_periph_clock_enable(RCC_TIM4);

    //Setup main timer
    timer_disable_counter(TIM4);
    timer_set_mode(TIM4, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE,TIM_CR1_DIR_DOWN);

    switch(timerChannel){

        case CHANNEL_1:

            pinSetup(GPIO_PWM,PORTB,PIN6);

            timer_set_oc_mode(TIM4, TIM_OC1, TIM_OCM_PWM1);
            timer_enable_oc_output(TIM4, TIM_OC1);
            TIMER_setPeriod(TIMER_4,(uint32_t)(1000000.0f/(float)frequency));
            TIMER_setDuty(TIMER_4,CHANNEL_1,duty);
            timer_enable_counter(TIM4);
            break;

        case CHANNEL_2:

            pinSetup(GPIO_PWM,PORTB,PIN7);

            timer_set_oc_mode(TIM4, TIM_OC2, TIM_OCM_PWM1);
            timer_enable_oc_output(TIM4, TIM_OC2);
            TIMER_setPeriod(TIMER_4,(uint32_t)(1000000.0f/(float)frequency));
            TIMER_setDuty(TIMER_4,CHANNEL_2,duty);
            timer_enable_counter(TIM4);
            break;
            
        case CHANNEL_3:

            pinSetup(GPIO_PWM,PORTB,PIN8);

            timer_set_oc_mode(TIM4, TIM_OC3, TIM_OCM_PWM1);
            timer_enable_oc_output(TIM4, TIM_OC3);
            TIMER_setPeriod(TIMER_4,(uint32_t)(1000000.0f/(float)frequency));
            TIMER_setDuty(TIMER_4,CHANNEL_3,duty);
            timer_enable_counter(TIM4);
            break;

        case CHANNEL_4:

            pinSetup(GPIO_PWM,PORTB,PIN9);

            timer_set_oc_mode(TIM4, TIM_OC4, TIM_OCM_PWM1);
            timer_enable_oc_output(TIM4, TIM_OC4);
            TIMER_setPeriod(TIMER_4,(uint32_t)(1000000.0f/(float)frequency));
            TIMER_setDuty(TIMER_4,CHANNEL_4,duty);
            timer_enable_counter(TIM4);
            break;

        default:
			if(MCU_debugIsEnabled()){
				MCU_printError(E_TIMER_NOCHANNEL);
			}
            error = E_TIMER_NOCHANNEL;
            break;
    }

    return error;

}

mcu_error timer1_setupPulse(timer_channel timerChannel,uint32_t frequency,uint32_t pulse){

    mcu_error error;

    //Set the clock
    rcc_periph_clock_enable(RCC_TIM1);

    //Setup main timer
    timer_disable_counter(TIM1);
    timer_set_mode(TIM1, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE,TIM_CR1_DIR_DOWN);
    timer_enable_break_main_output(TIM1);

    switch(timerChannel){

        case CHANNEL_1:

            pinSetup(GPIO_PWM,PORTA,PIN8);

            timer_set_oc_mode(TIM1, TIM_OC1, TIM_OCM_PWM1);
            timer_enable_oc_output(TIM1, TIM_OC1);
            TIMER_setPeriod(TIMER_1,(uint32_t)(1000000.0f/(float)frequency));
            TIMER_setPulse(TIMER_1,CHANNEL_1,pulse);
            timer_enable_counter(TIM1);
            break;

        case CHANNEL_2:

            pinSetup(GPIO_PWM,PORTA,PIN9);

            timer_set_oc_mode(TIM1, TIM_OC2, TIM_OCM_PWM1);
            timer_enable_oc_output(TIM1, TIM_OC2);
            TIMER_setPeriod(TIMER_1,(uint32_t)(1000000.0f/(float)frequency));
            TIMER_setPulse(TIMER_1,CHANNEL_2,pulse);
            timer_enable_counter(TIM1);
            break;
        case CHANNEL_3:

            pinSetup(GPIO_PWM,PORTA,PIN10);

            timer_set_oc_mode(TIM1, TIM_OC3, TIM_OCM_PWM1);
            timer_enable_oc_output(TIM1, TIM_OC3);
            TIMER_setPeriod(TIMER_1,(uint32_t)(1000000.0f/(float)frequency));
            TIMER_setPulse(TIMER_1,CHANNEL_3,pulse);
            timer_enable_counter(TIM1);

            break;
        case CHANNEL_4:

            pinSetup(GPIO_PWM,PORTA,PIN11);

            timer_set_oc_mode(TIM1, TIM_OC4, TIM_OCM_PWM1);
            timer_enable_oc_output(TIM1, TIM_OC4);
            TIMER_setPeriod(TIMER_1,(uint32_t)(1000000.0f/(float)frequency));
            TIMER_setPulse(TIMER_1,CHANNEL_4,pulse);
            timer_enable_counter(TIM1);
            break;

        default:
			if(MCU_debugIsEnabled()){
				MCU_printError(E_TIMER_NOCHANNEL);
			}
            error = E_TIMER_NOCHANNEL;
            break;
    }

    return error;
}

mcu_error timer2_setupPulse(timer_channel timerChannel,uint32_t frequency,uint32_t pulse){

    mcu_error error;

    //Set the clock
    rcc_periph_clock_enable(RCC_TIM2);

    //Setup main timer
    timer_disable_counter(TIM2);
    timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE,TIM_CR1_DIR_DOWN);

    switch(timerChannel){

        case CHANNEL_1:

            pinSetup(GPIO_PWM,PORTA,PIN0);

            timer_set_oc_mode(TIM2, TIM_OC1, TIM_OCM_PWM1);
            timer_enable_oc_output(TIM2, TIM_OC1);
            TIMER_setPeriod(TIMER_2,(uint32_t)(1000000.0f/(float)frequency));
            TIMER_setPulse(TIMER_2,CHANNEL_1,pulse);
            timer_enable_counter(TIM2);
            break;

        case CHANNEL_2:

            pinSetup(GPIO_PWM,PORTA,PIN1);

            timer_set_oc_mode(TIM2, TIM_OC2, TIM_OCM_PWM1);
            timer_enable_oc_output(TIM2, TIM_OC2);
            TIMER_setPeriod(TIMER_2,(uint32_t)(1000000.0f/(float)frequency));
            TIMER_setPulse(TIMER_2,CHANNEL_2,pulse);
            timer_enable_counter(TIM2);
            break;

        case CHANNEL_3:

            pinSetup(GPIO_PWM,PORTA,PIN2);

            timer_set_oc_mode(TIM2, TIM_OC3, TIM_OCM_PWM1);
            timer_enable_oc_output(TIM2, TIM_OC3);
            TIMER_setPeriod(TIMER_2,(uint32_t)(1000000.0f/(float)frequency));
            TIMER_setPulse(TIMER_2,CHANNEL_3,pulse);
            timer_enable_counter(TIM2);

            break;

        case CHANNEL_4:

            pinSetup(GPIO_PWM,PORTA,PIN3);

            timer_set_oc_mode(TIM2, TIM_OC4, TIM_OCM_PWM1);
            timer_enable_oc_output(TIM2, TIM_OC4);
            TIMER_setPeriod(TIMER_2,(uint32_t)(1000000.0f/(float)frequency));
            TIMER_setPulse(TIMER_2,CHANNEL_4,pulse);
            timer_enable_counter(TIM2);
            break;

        default:
			if(MCU_debugIsEnabled()){
				MCU_printError(E_TIMER_NOCHANNEL);
			}
            error = E_TIMER_NOCHANNEL;
            break;
    }

    return error;

}

mcu_error timer3_setupPulse(timer_channel timerChannel,uint32_t frequency,uint32_t pulse){

    mcu_error error;

    //Set the clock
    rcc_periph_clock_enable(RCC_TIM3);

    //Setup main timer
    timer_disable_counter(TIM3);
    timer_set_mode(TIM3, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE,TIM_CR1_DIR_DOWN);

    switch(timerChannel){

        case CHANNEL_1:

            pinSetup(GPIO_PWM,PORTA,PIN6);

            timer_set_oc_mode(TIM3, TIM_OC1, TIM_OCM_PWM1);
            timer_enable_oc_output(TIM3, TIM_OC1);
            TIMER_setPeriod(TIMER_3,(uint32_t)(1000000.0f/(float)frequency));
            TIMER_setPulse(TIMER_3,CHANNEL_1,pulse);
            timer_enable_counter(TIM3);
            break;

        case CHANNEL_2:

            pinSetup(GPIO_PWM,PORTA,PIN7);

            timer_set_oc_mode(TIM3, TIM_OC2, TIM_OCM_PWM1);
            timer_enable_oc_output(TIM3, TIM_OC2);
            TIMER_setPeriod(TIMER_3,(uint32_t)(1000000.0f/(float)frequency));
            TIMER_setPulse(TIMER_3,CHANNEL_2,pulse);
            timer_enable_counter(TIM3);
            break;

        case CHANNEL_3:

            pinSetup(GPIO_PWM,PORTB,PIN0);

            timer_set_oc_mode(TIM3, TIM_OC3, TIM_OCM_PWM1);
            timer_enable_oc_output(TIM3, TIM_OC3);
            TIMER_setPeriod(TIMER_3,(uint32_t)(1000000.0f/(float)frequency));
            TIMER_setPulse(TIMER_3,CHANNEL_3,pulse);
            timer_enable_counter(TIM3);

            break;

        case CHANNEL_4:

            pinSetup(GPIO_PWM,PORTB,PIN1);

            timer_set_oc_mode(TIM3, TIM_OC4, TIM_OCM_PWM1);
            timer_enable_oc_output(TIM3, TIM_OC4);
            TIMER_setPeriod(TIMER_3,(uint32_t)(1000000.0f/(float)frequency));
            TIMER_setPulse(TIMER_3,CHANNEL_4,pulse);
            timer_enable_counter(TIM3);
            break;

        default:
			if(MCU_debugIsEnabled()){
				MCU_printError(E_TIMER_NOCHANNEL);
			}
            error = E_TIMER_NOCHANNEL;
            break;
    }

    return error;

}

mcu_error timer4_setupPulse(timer_channel timerChannel,uint32_t frequency,uint32_t pulse){

    mcu_error error;

    //Set the clock
    rcc_periph_clock_enable(RCC_TIM4);

    //Setup main timer
    timer_disable_counter(TIM4);
    timer_set_mode(TIM4, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE,TIM_CR1_DIR_DOWN);

    switch(timerChannel){

        case CHANNEL_1:

            pinSetup(GPIO_PWM,PORTB,PIN6);

            timer_set_oc_mode(TIM4, TIM_OC1, TIM_OCM_PWM1);
            timer_enable_oc_output(TIM4, TIM_OC1);
            TIMER_setPeriod(TIMER_4,(uint32_t)(1000000.0f/(float)frequency));
            TIMER_setPulse(TIMER_4,CHANNEL_1,pulse);
            timer_enable_counter(TIM4);
            break;

        case CHANNEL_2:

            pinSetup(GPIO_PWM,PORTB,PIN7);

            timer_set_oc_mode(TIM4, TIM_OC2, TIM_OCM_PWM1);
            timer_enable_oc_output(TIM4, TIM_OC2);
            TIMER_setPeriod(TIMER_4,(uint32_t)(1000000.0f/(float)frequency));
            TIMER_setPulse(TIMER_4,CHANNEL_2,pulse);
            timer_enable_counter(TIM4);
            break;

        case CHANNEL_3:

            pinSetup(GPIO_PWM,PORTB,PIN8);

            timer_set_oc_mode(TIM4, TIM_OC3, TIM_OCM_PWM1);
            timer_enable_oc_output(TIM4, TIM_OC3);
            TIMER_setPeriod(TIMER_4,(uint32_t)(1000000.0f/(float)frequency));
            TIMER_setPulse(TIMER_4,CHANNEL_3,pulse);
            timer_enable_counter(TIM4);

            break;

        case CHANNEL_4:

            pinSetup(GPIO_PWM,PORTB,PIN9);

            timer_set_oc_mode(TIM4, TIM_OC4, TIM_OCM_PWM1);
            timer_enable_oc_output(TIM4, TIM_OC4);
            TIMER_setPeriod(TIMER_4,(uint32_t)(1000000.0f/(float)frequency));
            TIMER_setPulse(TIMER_4,CHANNEL_4,pulse);
            timer_enable_counter(TIM4);
            break;

        default:
			if(MCU_debugIsEnabled()){
				MCU_printError(E_TIMER_NOCHANNEL);
			}
            error = E_TIMER_NOCHANNEL;
            break;
    }

    return error;


}
/****************************************************************************/
//			                GET / SET COUNT
/****************************************************************************/

uint16_t TIMER_getCount(timer_main timerNumber){

    switch(timerNumber){

        case TIMER_1:

            return TIM_CNT(TIM1);
            break;
        case TIMER_2:

            return TIM_CNT(TIM2);
            break;

        case TIMER_3:

            return TIM_CNT(TIM3);
            break;

        case TIMER_4:

            return TIM_CNT(TIM4);
            break;

        default:
			if(MCU_debugIsEnabled()){
				MCU_printError(E_TIMER_NOTIMER);
			}
            return E_TIMER_NOTIMER;

            break;
    }

    return E_TIMER_NOERROR;

}
mcu_error TIMER_setCount(timer_main timerNumber, uint16_t count){

    switch(timerNumber){

        case TIMER_1:

            TIM_CNT(TIM1) = count;
            break;
        case TIMER_2:

            TIM_CNT(TIM2) = count;
            break;

        case TIMER_3:

            TIM_CNT(TIM3) = count;
            break;

        case TIMER_4:

            TIM_CNT(TIM4) = count;
            break;

        default:
			if(MCU_debugIsEnabled()){
				MCU_printError(E_TIMER_NOTIMER);
			}
            return E_TIMER_NOTIMER;

            break;
    }

    return E_TIMER_NOERROR;


}
/****************************************************************************/
//			                PERIOD  / FREQUENCY
/****************************************************************************/
mcu_error TIMER_setPeriod(timer_main timerNumber, uint32_t period){

    uint32_t prescaler;

    switch(timerNumber){

        case TIMER_1:

            //Timer 1 runs on apb2, so at full clock speed, get the clock cycles per microsecond
            prescaler = (CLOCK_getSpeed()/1000000);

            if(period <= 0xFFFF){

                timer_set_prescaler(TIM1,prescaler-1);
                timer_set_period(TIM1, period-1);

            }
            else if(period <= 655350){

                prescaler *= 10;
                timer_set_prescaler(TIM1,prescaler-1);
                timer_set_period(TIM1, (period/10.0) - 1);
            }
            else if(period <= 6553500){

                prescaler *= 100;
                timer_set_prescaler(TIM1,prescaler-1);
                timer_set_period(TIM1, (period/100.0) -1);
            }
            else{

                //Too long
                if(MCU_debugIsEnabled()){
                    MCU_printError(E_TIMER_PERIOD);
                }
                return E_TIMER_PERIOD;
            }

            break;

        case TIMER_2:

            //Timer 2 runs on apb1, get the clock cycles per microsecond
            prescaler = (CLOCK_getSpeed()/1000000);

            if(period <= 0xFFFF){

                timer_set_prescaler(TIM2,prescaler-1);
                timer_set_period(TIM2, period-1);

            }
            else if(period < 655350){

                prescaler *= 10;
                timer_set_prescaler(TIM2,prescaler-1);
                timer_set_period(TIM2, (period/10.0) - 1);
            }
            else if(period < 6553500){

                prescaler *= 100;
                timer_set_prescaler(TIM2,prescaler-1);
                timer_set_period(TIM2, (period/100.0) -1);
            }
            else{

                //Too long
                if(MCU_debugIsEnabled()){
                    MCU_printError(E_TIMER_PERIOD);
                }
                return E_TIMER_PERIOD;
            }
            break;

        case TIMER_3:

            //Timer 2 runs on apb1, get the clock cycles per microsecond
            prescaler = (CLOCK_getSpeed()/1000000);

            if(period <= 0xFFFF){

                timer_set_prescaler(TIM3,prescaler-1);
                timer_set_period(TIM3, period-1);

            }
            else if(period < 655350){

                prescaler *= 10;
                timer_set_prescaler(TIM3,prescaler-1);
                timer_set_period(TIM3, (period/10.0) - 1);
            }
            else if(period < 6553500){

                prescaler *= 100;
                timer_set_prescaler(TIM3,prescaler-1);
                timer_set_period(TIM3, (period/100.0) -1);
            }
            else{

                //Too long
                if(MCU_debugIsEnabled()){
                    MCU_printError(E_TIMER_PERIOD);
                }
                return E_TIMER_PERIOD;
            }
            break;

        case TIMER_4:

            prescaler = (CLOCK_getSpeed()/1000000);

            if(period <= 0xFFFF){

                timer_set_prescaler(TIM4,prescaler-1);
                timer_set_period(TIM4, period-1);

            }
            else if(period < 655350){

                prescaler *= 10;
                timer_set_prescaler(TIM4,prescaler-1);
                timer_set_period(TIM4, (period/10.0) - 1);
            }
            else if(period < 6553500){

                prescaler *= 100;
                timer_set_prescaler(TIM4,prescaler-1);
                timer_set_period(TIM4, (period/100.0) -1);
            }
            else{

                //Too long
                if(MCU_debugIsEnabled()){
                    MCU_printError(E_TIMER_PERIOD);
                }
                return E_TIMER_PERIOD;
            }
            break;

        default:
			if(MCU_debugIsEnabled()){
				MCU_printError(E_TIMER_NOTIMER);
			}
            return E_TIMER_NOTIMER;

            break;
    }

    return E_TIMER_NOERROR;

}

mcu_error TIMER_setFrequency(timer_main timerNumber, uint32_t frequency){

    mcu_error error = E_TIMER_NOERROR;
    switch(timerNumber){

        case TIMER_1:

            error = TIMER_setPeriod(timerNumber,1000000.0f/(float)frequency);
            break;

        case TIMER_2:

            error = TIMER_setPeriod(timerNumber,1000000.0f/(float)frequency);
            break;

        case TIMER_3:

            error = TIMER_setPeriod(timerNumber,1000000.0f/(float)frequency);
            break;

        case TIMER_4:

            error = TIMER_setPeriod(timerNumber,1000000.0f/(float)frequency);
            break;

        default:
			if(MCU_debugIsEnabled()){
				MCU_printError(E_TIMER_NOTIMER);
			}
            error =  E_TIMER_NOTIMER;

            break;
    }

    return error;

}

mcu_error TIMER_setDuty(timer_main timerNumber, timer_channel channel,  uint8_t duty){

    if (duty > 100){

        duty = 100;
    }
    

    switch(timerNumber){

        case TIMER_1:

            switch(channel){

                case CHANNEL_1:

                        //Timer doesn't completely disable with 0 duty cycle
                        if(duty == 0){

                            TIM_CCER(TIM1) &= ~TIM_CCER_CC1E;
                        }
                        else{
                            TIM_CCER(TIM1) |= TIM_CCER_CC1E;
                            timer_set_oc_value(TIM1, TIM_OC1, TIM_ARR(TIM1)*((float)(duty)/100.0f));

                        }
                            
                        break;

                case CHANNEL_2:

                        //Timer doesn't completely disable with 0 duty cycle
                        if(duty == 0){

                            TIM_CCER(TIM1) &= ~TIM_CCER_CC2E;
                        }
                        else{
                            TIM_CCER(TIM1) |= TIM_CCER_CC2E;
                            timer_set_oc_value(TIM1, TIM_OC2, TIM_ARR(TIM1)*((float)(duty)/100.0f));

                        }
                        break;

                case CHANNEL_3:

                        //Timer doesn't completely disable with 0 duty cycle
                        if(duty == 0){

                            TIM_CCER(TIM1) &= ~TIM_CCER_CC3E;
                        }
                        else{
                            TIM_CCER(TIM1) |= TIM_CCER_CC3E;
                            timer_set_oc_value(TIM1, TIM_OC3, TIM_ARR(TIM1)*((float)(duty)/100.0f));

                        }
                        break;
                      
                case CHANNEL_4:

                        //Timer doesn't completely disable with 0 duty cycle
                        if(duty == 0){

                            TIM_CCER(TIM1) &= ~TIM_CCER_CC4E;
                        }
                        else{
                            TIM_CCER(TIM1) |= TIM_CCER_CC4E;
                            timer_set_oc_value(TIM1, TIM_OC4, TIM_ARR(TIM1)*((float)(duty)/100.0f));

                        }
                        break;
            }

            break;
        case TIMER_2:

            switch(channel){

                case CHANNEL_1:

                        //Timer doesn't completely disable with 0 duty cycle
                        if(duty == 0){

                            TIM_CCER(TIM2) &= ~TIM_CCER_CC1E;
                        }
                        else{
                            TIM_CCER(TIM2) |= TIM_CCER_CC1E;
                            timer_set_oc_value(TIM2, TIM_OC1, TIM_ARR(TIM2)*((float)(duty)/100.0f));

                        }
                            
                        break;

                case CHANNEL_2:

                        //Timer doesn't completely disable with 0 duty cycle
                        if(duty == 0){

                            TIM_CCER(TIM2) &= ~TIM_CCER_CC2E;
                        }
                        else{
                            TIM_CCER(TIM2) |= TIM_CCER_CC2E;
                            timer_set_oc_value(TIM2, TIM_OC2, TIM_ARR(TIM2)*((float)(duty)/100.0f));

                        }
                        break;

                case CHANNEL_3:

                        //Timer doesn't completely disable with 0 duty cycle
                        if(duty == 0){

                            TIM_CCER(TIM2) &= ~TIM_CCER_CC3E;
                        }
                        else{
                            TIM_CCER(TIM2) |= TIM_CCER_CC3E;
                            timer_set_oc_value(TIM2, TIM_OC3, TIM_ARR(TIM2)*((float)(duty)/100.0f));

                        }
                        break;
                      
                case CHANNEL_4:

                        //Timer doesn't completely disable with 0 duty cycle
                        if(duty == 0){

                            TIM_CCER(TIM2) &= ~TIM_CCER_CC4E;
                        }
                        else{
                            TIM_CCER(TIM2) |= TIM_CCER_CC4E;
                            timer_set_oc_value(TIM2, TIM_OC4, TIM_ARR(TIM2)*((float)(duty)/100.0f));

                        }
                        break;
            }
            break;
        case TIMER_3:

            switch(channel){

                case CHANNEL_1:

                        //Timer doesn't completely disable with 0 duty cycle
                        if(duty == 0){

                            TIM_CCER(TIM3) &= ~TIM_CCER_CC1E;
                        }
                        else{
                            TIM_CCER(TIM3) |= TIM_CCER_CC1E;
                            timer_set_oc_value(TIM3, TIM_OC1, TIM_ARR(TIM3)*((float)(duty)/100.0f));

                        }
                            
                        break;

                case CHANNEL_2:

                        //Timer doesn't completely disable with 0 duty cycle
                        if(duty == 0){

                            TIM_CCER(TIM3) &= ~TIM_CCER_CC2E;
                        }
                        else{
                            TIM_CCER(TIM3) |= TIM_CCER_CC2E;
                            timer_set_oc_value(TIM3, TIM_OC2, TIM_ARR(TIM3)*((float)(duty)/100.0f));

                        }
                        break;

                case CHANNEL_3:

                        //Timer doesn't completely disable with 0 duty cycle
                        if(duty == 0){

                            TIM_CCER(TIM3) &= ~TIM_CCER_CC3E;
                        }
                        else{
                            TIM_CCER(TIM3) |= TIM_CCER_CC3E;
                            timer_set_oc_value(TIM3, TIM_OC3, TIM_ARR(TIM3)*((float)(duty)/100.0f));

                        }
                        break;
                      
                case CHANNEL_4:

                        //Timer doesn't completely disable with 0 duty cycle
                        if(duty == 0){

                            TIM_CCER(TIM3) &= ~TIM_CCER_CC4E;
                        }
                        else{
                            TIM_CCER(TIM3) |= TIM_CCER_CC4E;
                            timer_set_oc_value(TIM3, TIM_OC4, TIM_ARR(TIM3)*((float)(duty)/100.0f));

                        }
                        break;
            }
            break;
        case TIMER_4:

            switch(channel){

                case CHANNEL_1:

                        //Timer doesn't completely disable with 0 duty cycle
                        if(duty == 0){

                            TIM_CCER(TIM4) &= ~TIM_CCER_CC1E;
                        }
                        else{
                            TIM_CCER(TIM4) |= TIM_CCER_CC1E;
                            timer_set_oc_value(TIM4, TIM_OC1, TIM_ARR(TIM4)*((float)(duty)/100.0f));

                        }
                            
                        break;

                case CHANNEL_2:

                        //Timer doesn't completely disable with 0 duty cycle
                        if(duty == 0){

                            TIM_CCER(TIM4) &= ~TIM_CCER_CC2E;
                        }
                        else{
                            TIM_CCER(TIM4) |= TIM_CCER_CC2E;
                            timer_set_oc_value(TIM4, TIM_OC2, TIM_ARR(TIM4)*((float)(duty)/100.0f));

                        }
                        break;

                case CHANNEL_3:

                        //Timer doesn't completely disable with 0 duty cycle
                        if(duty == 0){

                            TIM_CCER(TIM4) &= ~TIM_CCER_CC3E;
                        }
                        else{
                            TIM_CCER(TIM4) |= TIM_CCER_CC3E;
                            timer_set_oc_value(TIM4, TIM_OC3, TIM_ARR(TIM4)*((float)(duty)/100.0f));

                        }
                        break;
                      
                case CHANNEL_4:

                        //Timer doesn't completely disable with 0 duty cycle
                        if(duty == 0){

                            TIM_CCER(TIM4) &= ~TIM_CCER_CC4E;
                        }
                        else{
                            TIM_CCER(TIM4) |= TIM_CCER_CC4E;
                            timer_set_oc_value(TIM4, TIM_OC4, TIM_ARR(TIM4)*((float)(duty)/100.0f));

                        }
                        break;
            }
                break;
        default:
			if(MCU_debugIsEnabled()){
				MCU_printError(E_TIMER_NOTIMER);
			}
            return E_TIMER_NOTIMER;

            break;
    }

    return E_TIMER_NOERROR;

}
mcu_error TIMER_setPulse(timer_main timerNumber, timer_channel channel,  uint32_t pulse){

    mcu_error error = E_TIMER_NOERROR;

    switch(timerNumber){

        case TIMER_1:

            switch(channel){

                case CHANNEL_1:

                        //Timer doesn't turn off completely at 0
                        if(pulse == 0){

                            TIM_CCER(TIM1) &= ~TIM_CCER_CC1E;
                            
                        }
                        else{

                            if(pulse > TIM_ARR(TIM1)){

                                error = E_TIMER_PULSE;
                                pulse = TIM_ARR(TIM1);

                                if(MCU_debugIsEnabled()){
                                    MCU_printError(E_TIMER_PULSE);
                                }
                                
                            }
                            TIM_CCER(TIM1) |= TIM_CCER_CC1E;
                            timer_set_oc_value(TIM1, TIM_OC1, pulse-1);
                        }

                        break;

                case CHANNEL_2:

                        //Timer doesn't turn off completely at 0
                        if(pulse == 0){

                            TIM_CCER(TIM1) &= ~TIM_CCER_CC2E;
                            
                        }
                        else{

                            if(pulse > TIM_ARR(TIM1)){

                                error = E_TIMER_PULSE;
                                pulse = TIM_ARR(TIM1);

                                if(MCU_debugIsEnabled()){
                                    MCU_printError(E_TIMER_PULSE);
                                }
                                
                            }
                            TIM_CCER(TIM1) |= TIM_CCER_CC2E;
                            timer_set_oc_value(TIM1, TIM_OC2, pulse-1);
                        }
                        break;

                case CHANNEL_3:
                        //Timer doesn't turn off completely at 0
                        if(pulse == 0){

                            TIM_CCER(TIM1) &= ~TIM_CCER_CC3E;
                            
                        }
                        else{

                            if(pulse > TIM_ARR(TIM1)){

                                error = E_TIMER_PULSE;
                                pulse = TIM_ARR(TIM1);

                                if(MCU_debugIsEnabled()){
                                    MCU_printError(E_TIMER_PULSE);
                                }
                                
                            }
                            TIM_CCER(TIM1) |= TIM_CCER_CC3E;
                            timer_set_oc_value(TIM1, TIM_OC3, pulse-1);
                        }
                        break;
                      
                case CHANNEL_4:

                        //Timer doesn't turn off completely at 0
                        if(pulse == 0){

                            TIM_CCER(TIM1) &= ~TIM_CCER_CC4E;
                            
                        }
                        else{

                            if(pulse > TIM_ARR(TIM1)){

                                error = E_TIMER_PULSE;
                                pulse = TIM_ARR(TIM1);

                                if(MCU_debugIsEnabled()){
                                    MCU_printError(E_TIMER_PULSE);
                                }
                                
                            }
                            TIM_CCER(TIM1) |= TIM_CCER_CC4E;
                            timer_set_oc_value(TIM1, TIM_OC4, pulse-1);
                        }
                        break;
            }

            break;
        case TIMER_2:

            switch(channel){

                case CHANNEL_1:

                        //Timer doesn't turn off completely at 0
                        if(pulse == 0){

                            TIM_CCER(TIM2) &= ~TIM_CCER_CC1E;
                            
                        }
                        else{

                            if(pulse > TIM_ARR(TIM2)){

                                error = E_TIMER_PULSE;
                                pulse = TIM_ARR(TIM2);

                                if(MCU_debugIsEnabled()){
                                    MCU_printError(E_TIMER_PULSE);
                                }
                                
                            }
                            TIM_CCER(TIM2) |= TIM_CCER_CC1E;
                            timer_set_oc_value(TIM2, TIM_OC1, pulse-1);
                        }

                        break;

                case CHANNEL_2:

                        //Timer doesn't turn off completely at 0
                        if(pulse == 0){

                            TIM_CCER(TIM2) &= ~TIM_CCER_CC2E;
                            
                        }
                        else{

                            if(pulse > TIM_ARR(TIM2)){

                                error = E_TIMER_PULSE;
                                pulse = TIM_ARR(TIM2);

                                if(MCU_debugIsEnabled()){
                                    MCU_printError(E_TIMER_PULSE);
                                }
                                
                            }
                            TIM_CCER(TIM2) |= TIM_CCER_CC2E;
                            timer_set_oc_value(TIM2, TIM_OC2, pulse-1);
                        }
                        break;

                case CHANNEL_3:
                        //Timer doesn't turn off completely at 0
                        if(pulse == 0){

                            TIM_CCER(TIM2) &= ~TIM_CCER_CC3E;
                            
                        }
                        else{

                            if(pulse > TIM_ARR(TIM2)){

                                error = E_TIMER_PULSE;
                                pulse = TIM_ARR(TIM2);

                                if(MCU_debugIsEnabled()){
                                    MCU_printError(E_TIMER_PULSE);
                                }
                                
                            }
                            TIM_CCER(TIM2) |= TIM_CCER_CC3E;
                            timer_set_oc_value(TIM2, TIM_OC3, pulse-1);
                        }
                        break;
                      
                case CHANNEL_4:

                        //Timer doesn't turn off completely at 0
                        if(pulse == 0){

                            TIM_CCER(TIM2) &= ~TIM_CCER_CC4E;
                            
                        }
                        else{

                            if(pulse > TIM_ARR(TIM2)){

                                error = E_TIMER_PULSE;
                                pulse = TIM_ARR(TIM2);

                                if(MCU_debugIsEnabled()){
                                    MCU_printError(E_TIMER_PULSE);
                                }
                                
                            }
                            TIM_CCER(TIM2) |= TIM_CCER_CC4E;
                            timer_set_oc_value(TIM2, TIM_OC4, pulse-1);
                        }
                        break;
            }

            break;
        case TIMER_3:

            switch(channel){

                case CHANNEL_1:

                        //Timer doesn't turn off completely at 0
                        if(pulse == 0){

                            TIM_CCER(TIM3) &= ~TIM_CCER_CC1E;
                            
                        }
                        else{

                            if(pulse > TIM_ARR(TIM3)){

                                error = E_TIMER_PULSE;
                                pulse = TIM_ARR(TIM3);

                                if(MCU_debugIsEnabled()){
                                    MCU_printError(E_TIMER_PULSE);
                                }
                                
                            }
                            TIM_CCER(TIM3) |= TIM_CCER_CC1E;
                            timer_set_oc_value(TIM3, TIM_OC1, pulse-1);
                        }

                        break;

                case CHANNEL_2:

                        //Timer doesn't turn off completely at 0
                        if(pulse == 0){

                            TIM_CCER(TIM3) &= ~TIM_CCER_CC2E;
                            
                        }
                        else{

                            if(pulse > TIM_ARR(TIM3)){

                                error = E_TIMER_PULSE;
                                pulse = TIM_ARR(TIM3);

                                if(MCU_debugIsEnabled()){
                                    MCU_printError(E_TIMER_PULSE);
                                }
                                
                            }
                            TIM_CCER(TIM3) |= TIM_CCER_CC2E;
                            timer_set_oc_value(TIM3, TIM_OC2, pulse-1);
                        }
                        break;

                case CHANNEL_3:
                        //Timer doesn't turn off completely at 0
                        if(pulse == 0){

                            TIM_CCER(TIM3) &= ~TIM_CCER_CC3E;
                            
                        }
                        else{

                            if(pulse > TIM_ARR(TIM3)){

                                error = E_TIMER_PULSE;
                                pulse = TIM_ARR(TIM3);

                                if(MCU_debugIsEnabled()){
                                    MCU_printError(E_TIMER_PULSE);
                                }
                                
                            }
                            TIM_CCER(TIM3) |= TIM_CCER_CC3E;
                            timer_set_oc_value(TIM3, TIM_OC3, pulse-1);
                        }
                        break;
                      
                case CHANNEL_4:

                        //Timer doesn't turn off completely at 0
                        if(pulse == 0){

                            TIM_CCER(TIM3) &= ~TIM_CCER_CC4E;
                            
                        }
                        else{

                            if(pulse > TIM_ARR(TIM3)){

                                error = E_TIMER_PULSE;
                                pulse = TIM_ARR(TIM3);

                                if(MCU_debugIsEnabled()){
                                    MCU_printError(E_TIMER_PULSE);
                                }
                                
                            }
                            TIM_CCER(TIM3) |= TIM_CCER_CC4E;
                            timer_set_oc_value(TIM3, TIM_OC4, pulse-1);
                        }
                        break;
            }

            break;
        case TIMER_4:

            switch(channel){

                case CHANNEL_1:

                        //Timer doesn't turn off completely at 0
                        if(pulse == 0){

                            TIM_CCER(TIM4) &= ~TIM_CCER_CC1E;
                            
                        }
                        else{

                            if(pulse > TIM_ARR(TIM4)){

                                error = E_TIMER_PULSE;
                                pulse = TIM_ARR(TIM4);

                                if(MCU_debugIsEnabled()){
                                    MCU_printError(E_TIMER_PULSE);
                                }
                                
                            }
                            TIM_CCER(TIM4) |= TIM_CCER_CC1E;
                            timer_set_oc_value(TIM4, TIM_OC1, pulse-1);
                        }

                        break;

                case CHANNEL_2:

                        //Timer doesn't turn off completely at 0
                        if(pulse == 0){

                            TIM_CCER(TIM4) &= ~TIM_CCER_CC2E;
                            
                        }
                        else{

                            if(pulse > TIM_ARR(TIM4)){

                                error = E_TIMER_PULSE;
                                pulse = TIM_ARR(TIM4);

                                if(MCU_debugIsEnabled()){
                                    MCU_printError(E_TIMER_PULSE);
                                }
                                
                            }
                            TIM_CCER(TIM4) |= TIM_CCER_CC2E;
                            timer_set_oc_value(TIM4, TIM_OC2, pulse-1);
                        }
                        break;

                case CHANNEL_3:
                        //Timer doesn't turn off completely at 0
                        if(pulse == 0){

                            TIM_CCER(TIM4) &= ~TIM_CCER_CC3E;
                            
                        }
                        else{

                            if(pulse > TIM_ARR(TIM4)){

                                error = E_TIMER_PULSE;
                                pulse = TIM_ARR(TIM4);

                                if(MCU_debugIsEnabled()){
                                    MCU_printError(E_TIMER_PULSE);
                                }
                                
                            }
                            TIM_CCER(TIM4) |= TIM_CCER_CC3E;
                            timer_set_oc_value(TIM4, TIM_OC3, pulse-1);
                        }
                        break;
                      
                case CHANNEL_4:

                        //Timer doesn't turn off completely at 0
                        if(pulse == 0){

                            TIM_CCER(TIM4) &= ~TIM_CCER_CC4E;
                            
                        }
                        else{

                            if(pulse > TIM_ARR(TIM4)){

                                error = E_TIMER_PULSE;
                                pulse = TIM_ARR(TIM4);

                                if(MCU_debugIsEnabled()){
                                    MCU_printError(E_TIMER_PULSE);
                                }
                                
                            }
                            TIM_CCER(TIM4) |= TIM_CCER_CC4E;
                            timer_set_oc_value(TIM4, TIM_OC4, pulse-1);
                        }
                        break;
            }

                break;
        default:
			if(MCU_debugIsEnabled()){
				MCU_printError(E_TIMER_NOTIMER);
			}
            error = E_TIMER_NOTIMER;

            break;
    }

    return error;

}



