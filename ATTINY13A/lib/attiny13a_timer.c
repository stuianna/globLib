/*!**************************************************************************
    @file attiny13a_timer.c
    @brief Source file for attiny13a TIMER
    @author Stuart Ianna
    @version 0.1
    @date October 2018
    @copyright GNU GPLv3
    @warning None
    @bug 

    @details

    @par Compilers
    - avr-gcc V4.9.2
******************************************************************************/
#include "attiny13a_timer.h"

//TIMER_IC is enabled in the makefile
#ifdef TIMER_IC
static uint8_t IC_time;
static uint8_t IC_highTime;
#endif

/****************************************************************************/
//	                TIMER_ISR -> Enabled in makefile
/****************************************************************************/


#ifdef TIMER_ISR
static void (*tim0_handler)(void);

void IC_ISR(void);

ISR(TIM0_COMPA_vect){

    tim0_handler();
}

mcu_error TIMER_enableISR(timer_main timerNumber, void(*handler)(void)){

    (void)timerNumber;
    cli();
    tim0_handler = handler;
    TIMSK0 |= (1<<OCIE0A);
    sei();
    return E_TIMER_NOERROR;

}

mcu_error TIMER_disableISR(timer_main timerNumber){

    cli();
    TIMSK0 &= ~(1<<OCIE0A);
    sei();
    return E_TIMER_NOERROR;

}

mcu_error TIMER_setupCount(timer_main timerNumber, uint16_t frequency, void (*handler)(void)){

    //Use ctc mode to set the timeout time
    (void)timerNumber;
    tim0_handler = handler;
    TCCR0A |= (1 << WGM01);
    TIMSK0 |= (1<<OCIE0A);
    return TIMER_setFrequency(timerNumber,frequency);
}

mcu_error TIMER_setPeriod(timer_main timerNumber, uint32_t period){

    //Possibly a better way the set the period which doesn't invlude this divide term.
    return TIMER_setFrequency(timerNumber,(uint16_t)(1e6/period));
}

mcu_error TIMER_setFrequency(timer_main timerNumber,uint16_t frequency){

    cli();
    TCCR0B =  0x0;

    //The divisions in the if statements should be evaluated at compile time.

    if(frequency > (F_CPU / 960)){

        return E_TIMER_PERIOD;
    }
    else if(frequency > 600){

        //Prescalar = 64 
        TCCR0B |=  0x3;
        OCR0A = (CLOCK_DIV_64/frequency);
    }
    else if(frequency > (F_CPU / 240000)){

        //Prescalar = 1024 
        TCCR0B |=  0x5;
        OCR0A =  (CLOCK_DIV_1024/frequency);
    }
    else{

        return E_TIMER_PERIOD;
    }

    sei();
    return E_TIMER_NOERROR;

}
#endif

/****************************************************************************/
//			                PAUSE RESUME
/****************************************************************************/

mcu_error TIMER_pause(timer_main timerNumber){

    (void)timerNumber;

    //Timer is stopped by clearing the prescaler bits.
    TCCR0B &= ~0x7;
    return 0;
}

mcu_error TIMER_resume(timer_main timerNumber){

    //Problem here
    //Timer prescaler value gets overwritten when timer is paused
    //Use setup function to restart timer, or read prescaler in main
    //application.
    return 0;
}

/****************************************************************************/
//			   INPUT CAPTUER -> TIMER_IC enabled in Makefile
/****************************************************************************/

#ifdef TIMER_IC
mcu_error TIMER_setupIC(timer_main timerNumber, timer_channel timerChannel){

    (void)timerNumber;
    (void)timerChannel;

    //Set frequency dependant on main clock, see attiny13a_timer.h
    TCCR0B =  0x0;

#if F_CPU == 9600000
    TCCR0B |=  0x4; //256 prescaler
#elif F_CPU == 4800000
    TCCR0B |=  0x4; //256 prescaler
#elif F_CPU == 1200000
    TCCR0B |=  0x3; //64 prescaler
#elif F_CPU == 600000
    TCCR0B |=  0x3; //64 prescaler
#endif
    
    //Setup the GPIO pin as an input with interrupts on both edges.
    //This is used here both to save space and to force independant usage
    //of input capture and GPIO_ISR, as they both need the interrupt vector
    cli();
    DDRB &= ~(1<<IC_PIN);
    *(uint8_t*)PORTB &= ~(1<<IC_PIN);
    PCMSK |= (1<<IC_PIN);
    GIMSK |= 1<<PCIE;
    MCUCR &= ~(1<<ISC01);
    MCUCR |= 1<<ISC00;
    sei();

    return E_TIMER_NOERROR;
}

//External interrupt vector, stolen from attiny13a_gpio.c
ISR(PCINT0_vect){

    //Need to check if the pin is high or low
    if(pinRead(PORTB,IC_PIN)){

        IC_highTime = TCNT0;
    }
    else{

        IC_time =  TCNT0 - IC_highTime;
    }
}

uint16_t TIMER_getIC(timer_main timerNumber, timer_channel channel){

   (void) timerNumber;
   (void) channel;

   if(0 == IC_time){

       return 0;
   }

   uint8_t tempTime = IC_time;
   IC_time = 0;

   //Take one of the IC_time reading for zero based couter.
   //Multiplyers can be calculated as
   // (1e6 *prescaler / F_CPU)
   // This is the resolution of the counter in microseconds
   
#if F_CPU == 9600000
    return  ((uint16_t)(tempTime-1) * 27);
#elif F_CPU == 4800000
    return  ((uint16_t)(tempTime-1) * 53);
#elif F_CPU == 1200000
    return  ((uint16_t)(tempTime-1) * 53);
#elif F_CPU == 600000
    return  ((uint16_t)(tempTime-1) * 106);
#else
    return  ((uint16_t)(tempTime-1) * 27);
#endif
}

#endif

/****************************************************************************/
//			            The following are always active
/****************************************************************************/

mcu_error TIMER_setupPWM(timer_main timerNumber, timer_channel timerChannel, uint16_t frequency, uint8_t duty){

    //PWM is setup as fast PWM

    (void)timerNumber;
    (void)frequency;

    TCCR0B =  0x0;
    TCCR0B |=  0x1;

    if(timerChannel == CHANNEL_1){

        TCCR0A |= (1 << WGM01) | (1 << WGM00);
        TCCR0A |= (1 << COM0A1);

        //Setup port as output.
        DDRB |= 0x1;
    }
    else{

        TCCR0A |= (1 << WGM01) | (1 << WGM00);
        TCCR0A |= (1 << COM0B1);

        //Setup port as output.
        DDRB |= 0x2;
    }

    TIMER_setDuty(timerNumber,timerChannel,duty);
    return E_TIMER_NOERROR;
}

mcu_error TIMER_setupPulse(timer_main timerNumber, timer_channel timerChannel, uint16_t frequency, uint16_t pulse){

    (void)timerNumber;
    (void)frequency;

    TCCR0B =  0x0;

    //Prescaler value is dependent on clock frequency.

#if F_CPU == 9600000
    TCCR0B |=  0x4; //256 prescaler
#elif F_CPU == 4800000
    TCCR0B |=  0x4; //256 prescaler
#elif F_CPU == 1200000
    TCCR0B |=  0x3; //64 prescaler
#elif F_CPU == 600000
    TCCR0B |=  0x3; //64 prescaler
#endif


    if(timerChannel == CHANNEL_1){

        TCCR0A |= (1 << WGM01) | (1 << WGM00);
        TCCR0A |= (1 << COM0A1);

        //Setup port as output.
        DDRB |= 0x1;
    }
    else{

        TCCR0A |= (1 << WGM01) | (1 << WGM00);
        TCCR0A |= (1 << COM0B1);

        //Setup port as output.
        DDRB |= 0x2;
    }

    TIMER_setPulse(timerNumber,timerChannel,pulse);
    return E_TIMER_NOERROR;
}


/****************************************************************************/
//			                GET / SET COUNT
/****************************************************************************/

uint8_t TIMER_getCount(timer_main timerNumber){

    (void) timerNumber;
    return TCNT0;

}
mcu_error TIMER_setCount(timer_main timerNumber, uint8_t count){

    (void) timerNumber;
    return TCNT0 = count;
}
/****************************************************************************/
//			                PERIOD  / FREQUENCY
/****************************************************************************/

mcu_error TIMER_setDuty(timer_main timerNumber, timer_channel channel,  uint8_t duty){

    if(channel == CHANNEL_1){

        OCR0A = (duty * 0xFF)/100;
    }
    else{

        OCR0B = (duty * 0xFF)/100;

    }
    return E_TIMER_NOERROR;
}

mcu_error TIMER_setPulse(timer_main timerNumber, timer_channel channel,  uint16_t pulse){

   //Dividers can be calculated as
   // (1e6 *prescaler / F_CPU)
   // This is the resolution of the counter in microseconds
#if F_CPU == 9600000
    OCR0B = (pulse / 27);
#elif F_CPU == 4800000
    OCR0B = (pulse / 53);
#elif F_CPU == 1200000
    OCR0B = (pulse / 53);
#elif F_CPU == 600000
    OCR0B = (pulse / 106);
#endif

    return E_TIMER_NOERROR;
}

