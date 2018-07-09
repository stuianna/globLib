/*!
    @file stm32f1x3_gpio.c
    @brief Source file for stm32f103 GPIO 
    @author Stuart Ianna
    @version 1.0
    @date May 2018
    @copyright GNU GPLv3
    @warning None
    @bug 

    @details

    @par Verified Compilers
    - arm-none-eabi-gcc (15:4.9.3+svn231177-1) 4.9.3 20150529 (prerelease)
*/

#include "stm32f1x3_gpio.h"

#define PORTA_EXTICR 0x0
#define PORTB_EXTICR 0x1
#define PORTC_EXTICR 0x2

static void ( *pin0_handler)(void);
static void ( *pin1_handler)(void);
static void ( *pin2_handler)(void);
static void ( *pin3_handler)(void);
static void ( *pin4_handler)(void);
static void ( *pin5_handler)(void);
static void ( *pin6_handler)(void);
static void ( *pin7_handler)(void);
static void ( *pin8_handler)(void);
static void ( *pin9_handler)(void);
static void ( *pin10_handler)(void);
static void ( *pin11_handler)(void);
static void ( *pin12_handler)(void);
static void ( *pin13_handler)(void);
static void ( *pin14_handler)(void);
static void ( *pin15_handler)(void);

static mcu_error gpio_setEXTIPortMask(gpio_pin pin, uint8_t mask, void (*handle)(void));
static uint8_t gpio_translatePin(gpio_pin pin);

//Initialize a port's pin to a function defined by macro definitions passed in as mode
mcu_error pinSetup(gpio_mode mode, gpio_port port, gpio_pin pin)
{
	//Determine the pin offset from the actual pin
	uint8_t correspontingPin = gpio_translatePin(pin);
	

	//Enable corresponding clock
	switch(port)
        {

		case PORTA:
                        rcc_periph_clock_enable(RCC_GPIOA);
			break;

		case PORTB:
                        rcc_periph_clock_enable(RCC_GPIOB);
			break;

		case PORTC:
                        rcc_periph_clock_enable(RCC_GPIOC);
			break;
        default:

            if(MCU_debugIsEnabled()){

                MCU_printError(E_GPIO_PIN);
            }
            return E_GPIO_PIN;
            break;

	}
	
        //STM32f1 has two function registers. Higher pin numbers need to be shifted down. (P170 Reference manual)
	if(correspontingPin > 0x07){

            (*((volatile unsigned long *)(GPIO_BASE+port+CRH_OFFSET))) |= (mode<<((correspontingPin-0x08)<<2));
            (*((volatile unsigned long *)(GPIO_BASE+port+CRH_OFFSET))) &= ~((mode^0xF)<<((correspontingPin-0x08)<<2));
	}
	else{
            (*((volatile unsigned long *)(GPIO_BASE+port+CRL_OFFSET))) |= (mode<<(correspontingPin<<2));
            (*((volatile unsigned long *)(GPIO_BASE+port+CRL_OFFSET))) &= ~((mode^0xF)<<(correspontingPin<<2));
	}
    return E_GPIO_NOERROR;

}

//Set a corresponing port's pin high
void pinHigh(gpio_port port, gpio_pin pin){

    (*((volatile unsigned long *)(GPIO_BASE+port+BSRR_OFFSET))) |= pin;
}

//Set a corresponding port's pin low
void pinLow(gpio_port port, gpio_pin pin){

    (*((volatile unsigned long *)(GPIO_BASE+port+BRR_OFFSET))) |= pin;
}

//Set a corresponding port's pin to a given state (defined as macro definition)
void pinWrite(gpio_port port, gpio_pin pin, gpio_state state)
{
    if(state)
    {
        pinLow(port,pin);
    }
    else
    {
        pinHigh(port,pin);
    }
}

//Toggle a given port's pin
void pinToggle(gpio_port port, gpio_pin pin){

    pinWrite(port,pin,pinRead(port,pin));

}

//Read the input state of a port's pin.
uint8_t pinRead(gpio_port port, gpio_pin pin){

	if(*((volatile unsigned long *)((GPIO_BASE+port+IDR_OFFSET))) & pin){
		return 1;
	}
	else{
		return 0;
	}
}

mcu_error GPIO_ISREnable(gpio_port port, gpio_pin pin, gpio_isr trigger, void (*handle)(void)){


    //Need to enable clock first
    rcc_periph_clock_enable(RCC_AFIO);

    uint8_t correspondingPin = gpio_translatePin(pin);

    //Check to make sure pin is available
    if((EXTI_IMR & pin)){

            if(MCU_debugIsEnabled()){

                MCU_printError(E_GPIO_ISR);
            }
        return E_GPIO_ISR;
    }
    
    //Setup the pin
    pinSetup(GPIO_DI,port,pin);

    //Configure the EXTI line to be mapped to that port
    switch(port){

        case PORTA:

            gpio_setEXTIPortMask(pin, PORTA_EXTICR, handle);
            break;

        case PORTB:

            gpio_setEXTIPortMask(pin, PORTB_EXTICR, handle);
            break;

        case PORTC:

            gpio_setEXTIPortMask(pin, PORTC_EXTICR, handle);
            break;

        default:

            if(MCU_debugIsEnabled()){

                MCU_printError(E_GPIO_PORT);
            }
            return E_GPIO_PORT;
            break;
    }
    
    //Set the mask to let interrupts through
    exti_enable_request(pin);

    //Set rising or falling
    if((trigger == GPIO_RISING)){

        exti_set_trigger(pin,EXTI_TRIGGER_RISING);

    }

    else if((trigger == GPIO_FALLING)){

        exti_set_trigger(pin,EXTI_TRIGGER_FALLING);

    }

    else if (trigger == GPIO_BOTH){

        exti_set_trigger(pin,EXTI_TRIGGER_BOTH);
    }
    else{

        if(MCU_debugIsEnabled()){

            MCU_printError(E_GPIO_TRIGGER);
        }
        return E_GPIO_TRIGGER;
    }
    

    //Enable interrupts

    if(correspondingPin == 0){

        nvic_enable_irq(NVIC_EXTI0_IRQ);
    }
    else if(correspondingPin == 1){

        nvic_enable_irq(NVIC_EXTI1_IRQ);
    }
    else if(correspondingPin == 2){

        nvic_enable_irq(NVIC_EXTI2_IRQ);
    }
    else if(correspondingPin == 3){

        nvic_enable_irq(NVIC_EXTI3_IRQ);
    }
    else if(correspondingPin == 4){

        nvic_enable_irq(NVIC_EXTI4_IRQ);
    }
    else if(correspondingPin <= 9){

        nvic_enable_irq(NVIC_EXTI9_5_IRQ);
    }
    else if(correspondingPin <= 15){
        nvic_enable_irq(NVIC_EXTI15_10_IRQ);

    }
    else{

        if(MCU_debugIsEnabled()){

            MCU_printError(E_GPIO_PIN);
        }
        return E_GPIO_PIN;
    }

    return E_GPIO_NOERROR;

}

//Sets the correct external interrupt configuration register for the given pin
//Page 190 Reference manual
static mcu_error gpio_setEXTIPortMask(gpio_pin pin, uint8_t mask, void (*handle)(void)){

    switch(pin){

        case PIN0:

            AFIO_EXTICR(0) &= ~(0xF);
            AFIO_EXTICR(0) |= (mask);
            pin0_handler = handle;
            break;

        case PIN1:

            AFIO_EXTICR(0) &= ~(0xF<<4);
            AFIO_EXTICR(0) |= (mask << 0x4);
            pin1_handler = handle;
            break;

        case PIN2:

            AFIO_EXTICR(0) &= ~(0xF<<8);
            AFIO_EXTICR(0) |= (mask << 0x8);
            pin2_handler = handle;
            break;

        case PIN3:

            AFIO_EXTICR(0) &= ~(0xF<<12);
            AFIO_EXTICR(0) |= (mask << 0xC);
            pin3_handler = handle;
            break;
            
        case PIN4:

            AFIO_EXTICR(1) &= ~(0xF);
            AFIO_EXTICR(1) |= (mask);
            pin4_handler = handle;
            break;

        case PIN5:

            AFIO_EXTICR(1) &= ~(0xF<<4);
            AFIO_EXTICR(1) |= (mask << 0x4);
            pin5_handler = handle;
            break;

        case PIN6:

            AFIO_EXTICR(1) &= ~(0xF<<8);
            AFIO_EXTICR(1) |= (mask << 0x8);
            pin6_handler = handle;
            break;

        case PIN7:

            AFIO_EXTICR(1) &= ~(0xF<<12);
            AFIO_EXTICR(1) |= (mask << 0xC);
            pin7_handler = handle;
            break;

        case PIN8:

            AFIO_EXTICR(2) &= ~(0xF);
            AFIO_EXTICR(2) |= (mask);
            pin8_handler = handle;
            break;

        case PIN9:

            AFIO_EXTICR(2) &= ~(0xF<<4);
            AFIO_EXTICR(2) |= (mask << 0x4);
            pin9_handler = handle;
            break;

        case PIN10:

            AFIO_EXTICR(2) &= ~(0xF<<8);
            AFIO_EXTICR(2) |= (mask << 0x8);
            pin10_handler = handle;
            break;

        case PIN11:

            AFIO_EXTICR(2) &= ~(0xF<<12);
            AFIO_EXTICR(2) |= (mask << 0xC);
            pin11_handler = handle;
            break;

        case PIN12:

            AFIO_EXTICR(3) &= ~(0xF);
            AFIO_EXTICR(3) |= (mask);
            pin12_handler = handle;
            break;

        case PIN13:

            AFIO_EXTICR(3) &= ~(0xF<<4);
            AFIO_EXTICR(3) |= (mask << 0x4);
            pin13_handler = handle;
            break;

        case PIN14:

            AFIO_EXTICR(3) &= ~(0xF<<8);
            AFIO_EXTICR(3) |= (mask << 0x8);
            pin14_handler = handle;
            break;

        case PIN15:
            AFIO_EXTICR(3) &= ~(0xF<<12);
            AFIO_EXTICR(3) |= (mask << 0xC);
            pin15_handler = handle;
            break;

        default:

            if(MCU_debugIsEnabled()){

                MCU_printError(E_GPIO_PIN);
            }
            return E_GPIO_PIN;
            break;

    }
    
    return E_GPIO_NOERROR;

}
void exti0_isr(void){

        exti_reset_request(EXTI0);
        pin0_handler();
}

void exti1_isr(void){

        exti_reset_request(EXTI1);
        pin1_handler();
}

void exti2_isr(void){

        exti_reset_request(EXTI2);
        pin2_handler();
}

void exti3_isr(void){

        exti_reset_request(EXTI3);
        pin3_handler();
}

void exti4_isr(void){

        exti_reset_request(EXTI4);
        pin4_handler();
}

void exti9_5_isr(void){


    //Pin 5 interrupt
    if((EXTI_PR & PIN5) != 0){

        exti_reset_request(EXTI5);
        pin5_handler();
    }
    if((EXTI_PR & PIN6) != 0){

        exti_reset_request(EXTI6);
        pin6_handler();
    }
    if((EXTI_PR & PIN7) != 0){

        exti_reset_request(EXTI7);
        pin7_handler();
    }
    if((EXTI_PR & PIN8) != 0){

        exti_reset_request(EXTI8);
        pin8_handler();
    }
    if((EXTI_PR & PIN9) != 0){

        exti_reset_request(EXTI9);
        pin9_handler();
    }

}

void exti15_10_isr(void){

    if((EXTI_PR & PIN10) != 0){

        exti_reset_request(EXTI10);
        pin10_handler();
    }

    if((EXTI_PR & PIN11) != 0){

        exti_reset_request(EXTI11);
        pin11_handler();
    }
    if((EXTI_PR & PIN12) != 0){

        exti_reset_request(EXTI12);
        pin12_handler();
    }
    if((EXTI_PR & PIN13) != 0){

        exti_reset_request(EXTI13);
        pin13_handler();
    }
    if((EXTI_PR & PIN14) != 0){

        exti_reset_request(EXTI14);
        pin14_handler();
    }
    if((EXTI_PR & PIN15) != 0){

        exti_reset_request(EXTI15);
        pin15_handler();
    }

}

mcu_error GPIO_ISRDisable(gpio_pin pin){

    uint8_t correspondingPin = gpio_translatePin(pin);
  
    //Disable NVIC
    if(correspondingPin == 0){

        nvic_enable_irq(NVIC_EXTI0_IRQ);
    }
    else if(correspondingPin == 1){

        nvic_enable_irq(NVIC_EXTI1_IRQ);
    }
    else if(correspondingPin == 2){

        nvic_enable_irq(NVIC_EXTI2_IRQ);
    }
    else if(correspondingPin == 3){

        nvic_enable_irq(NVIC_EXTI3_IRQ);
    }
    else if(correspondingPin == 4){

        nvic_enable_irq(NVIC_EXTI4_IRQ);
    }
    else if(correspondingPin <= 9){

        nvic_enable_irq(NVIC_EXTI9_5_IRQ);
    }
    else if(correspondingPin <= 15){
        nvic_enable_irq(NVIC_EXTI15_10_IRQ);

    }
    else{

        if(MCU_debugIsEnabled()){

            MCU_printError(E_GPIO_PIN);
        }
        return E_GPIO_PIN;
    }

    //Disable Mask
    exti_disable_request(pin);
    return E_GPIO_NOERROR;
}

static uint8_t gpio_translatePin(gpio_pin pin){
    
    uint8_t	correspontingPin;

	switch(pin){

		case PIN0:
			correspontingPin = 0x00;
			break;

		case PIN1:
			correspontingPin = 0x01;
			break;

		case PIN2:
			correspontingPin = 0x02;
			break;

		case PIN3:
			correspontingPin = 0x03;
			break;

		case PIN4:
			correspontingPin = 0x04;
			break;

		case PIN5:
			correspontingPin = 0x05;
			break;

		case PIN6:
			correspontingPin = 0x06;
			break;

		case PIN7:
			correspontingPin = 0x07;
			break;

		case PIN8:
			correspontingPin = 0x08;
			break;

		case PIN9:
			correspontingPin = 0x09;
			break;

		case PIN10:
			correspontingPin = 0x0A;
			break;

		case PIN11:
			correspontingPin = 0x0B;
			break;

		case PIN12:
			correspontingPin = 0x0C;
			break;

		case PIN13:
			correspontingPin = 0x0D;
			break;

		case PIN14:
			correspontingPin = 0x0E;
			break;

		case PIN15:
			correspontingPin = 0x0F;
			break;
			
		default:
            correspontingPin = 0;
			break;
	}

    return correspontingPin;
}

