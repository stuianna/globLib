/*!
    @file attiny13a_gpio.c
    @brief Source file for attiny13a GPIO 
    @author Stuart Ianna
    @version 0.11
    @date October 2018
    @copyright GNU GPLv3
    @warning None
    @bug 

    @details

    @par Compilers
     - avr-gcc 4.9.2
*/

#include "attiny13a_gpio.h"

#ifdef GPIO_ISR 
static void ( *isr_handler)(void);
#endif

//Initialize a port's pin to a function defined by macro definitions passed in as mode
mcu_error pinSetup(gpio_mode mode, gpio_port port, gpio_pin pin){

    switch(mode){

        case GPIO_DO:
            
            DDRB |= 1<<pin;
            break;

        case GPIO_DI:
            DDRB &= ~(1<<pin);
            *(uint8_t*)PORTB &= ~(1<<pin);
            break;

        default:
            return E_GPIO_PORT;
            break;
    }

    return E_GPIO_NOERROR;
}

//Set a corresponing port's pin high
void pinHigh(gpio_port port, gpio_pin pin){

    *(uint8_t*)PORTB |= 1<<pin;
}

//Set a corresponding port's pin low
void pinLow(gpio_port port, gpio_pin pin){

    *(uint8_t*)PORTB &= ~(1<<pin);
}

//Set a corresponding port's pin to a given state (defined as macro definition)
void pinWrite(gpio_port port, gpio_pin pin, gpio_state state)
{
    if(state)
    {
        pinHigh(port,pin);
    }
    else
    {
        pinLow(port,pin);
    }
}

//Toggle a given port's pin
void pinToggle(gpio_port port, gpio_pin pin){

    *(uint8_t*)PORTB ^= (1<<pin);
}

//Read the input state of a port's pin.
uint8_t pinRead(gpio_port port, gpio_pin pin){

    return PINB & (1<<pin) ;
}

#ifdef GPIO_ISR 


mcu_error GPIO_ISREnable(const gpio_port port, const gpio_pin pin, const gpio_isr trigger, void (*handle)(void)){

    cli();
    pinSetup(GPIO_DI,port,pin);
    PCMSK |= (1<<pin);
    GIMSK |= 1<<PCIE;
    isr_handler = handle;

    switch(trigger){

        case GPIO_RISING:

            MCUCR |= 1<<ISC00;
            MCUCR |= 1<<ISC01;
            break;
            
        case GPIO_FALLING:

            MCUCR &= ~(1<<ISC00);
            MCUCR |= 1<<ISC01;
            break;

        case GPIO_BOTH:

            MCUCR |= 1<<ISC00;
            MCUCR &= ~(1<<ISC01);
            break;

        case GPIO_LEVEL_LOW:

            MCUCR &= ~(1<<ISC00);
            MCUCR &= ~(1<<ISC01);
        default:

            return E_GPIO_TRIGGER;
            break;
    }
    sei();
    return E_GPIO_NOERROR;
}

mcu_error GPIO_ISRDisable(gpio_pin pin){

    PCMSK &= ~(1<<pin);

    if(0 == PCMSK){

        GIMSK &= ~(1<<PCIE);
    }

    return E_GPIO_NOERROR;
}


ISR(PCINT0_vect){

    isr_handler();
}

#endif

