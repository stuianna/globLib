/*!**************************************************************************
    @file stm32f1x3_usart.c
    @brief Source file for stm32f103 USART
    @author Stuart Ianna
    @version 1.0
    @date May 2018
    @copyright GNU GPLv3
    @warning None
    @bug The USART.get() function is not compatible when using ISRs. This is becuase the data register needs to be read to reset the flags. This cannot be efficiently done in both ISR and get function. This is not a large problem as the get function is blocking, and isn't really used with an ISR.

    @details
    @par Verified Compilers
    - arm-none-eabi-gcc (15:4.9.3+svn231177-1) 4.9.3 20150529 (prerelease)
******************************************************************************/

#include "stm32f1x3_usart.h"

/****************************************************************************/
//				USART1
/****************************************************************************/

//ISR function pointers 
static void (*usart1_isr_rx)(uint8_t c);
static void (*usart1_isr_tx)(void);

//USART1 routines
static uint8_t usart1_set_baud(uint8_t baud);
static uint8_t usart1_set_stop(uint8_t stop);
static uint8_t usart1_set_parity(uint8_t parity);
static uint8_t usart1_set_data(uint8_t data);
static uint8_t usart1_setup(void);
static void    usart1_default_rx_isr(uint8_t byte);
static void    usart1_default_tx_isr(void);
static void    usart1_put(uint8_t c);
static uint8_t usart1_get(void);

/****************************************************************************/
//				USART2
/****************************************************************************/

//ISR function pointers 
static void (*usart2_isr_rx)(uint8_t c);
static void (*usart2_isr_tx)(void);

//USART1 routines
static uint8_t usart2_set_baud(uint8_t baud);
static uint8_t usart2_set_stop(uint8_t stop);
static uint8_t usart2_set_parity(uint8_t parity);
static uint8_t usart2_set_data(uint8_t data);
static uint8_t usart2_setup(void);
static void    usart2_default_rx_isr(uint8_t byte);
static void    usart2_default_tx_isr(void);
static void    usart2_put(uint8_t c);
static uint8_t usart2_get(void);

/****************************************************************************/
//				USART3
/****************************************************************************/

//ISR function pointers 
static void (*usart3_isr_rx)(uint8_t c);
static void (*usart3_isr_tx)(void);

//USART1 routines
static uint8_t usart3_set_baud(uint8_t baud);
static uint8_t usart3_set_stop(uint8_t stop);
static uint8_t usart3_set_parity(uint8_t parity);
static uint8_t usart3_set_data(uint8_t data);
static uint8_t usart3_setup(void);
static void    usart3_default_rx_isr(uint8_t byte);
static void    usart3_default_tx_isr(void);
static void    usart3_put(uint8_t c);
static uint8_t usart3_get(void);

/****************************************************************************/
//			     SETUP FUNCTIONS
/****************************************************************************/

mcu_error USART_setup(usart_periph peripheral){

	uint8_t error = E_USART_NOERROR;

	switch(peripheral){

		case USART_1:

			//Direct the ISR routines
			USART_setTxISR (peripheral, usart1_default_tx_isr);
			USART_setRxISR (peripheral, usart1_default_rx_isr);

			//Setup the port
			usart1_setup();
			break;

		case USART_2:

			//Direct the ISR routines
			USART_setTxISR (peripheral, usart2_default_tx_isr);
			USART_setRxISR (peripheral, usart2_default_rx_isr);

			//Setup the port
			usart2_setup();
			break;

		case USART_3:

			//Direct the ISR routines
			USART_setTxISR (peripheral, usart3_default_tx_isr);
			USART_setRxISR (peripheral, usart3_default_rx_isr);

			//Setup the port
			usart3_setup();
			break;
		default:
			error =  E_USART_NOPORT;

			if(MCU_debugIsEnabled()){
				MCU_printError(E_USART_NOPORT);
			}
			break;
	}

	return error;
}

mcu_error usart1_setup(void){

	uint8_t error = E_USART_NOERROR;

	//Setup the GPIO pins
	pinSetup(GPIO_UART_RX,PORTA,PIN10);
	pinSetup(GPIO_UART_TX,PORTA,PIN9);

	//Enable USART CLOCK
    rcc_periph_clock_enable(RCC_USART1);

	//Set default settings
	usart1_set_parity(USART_DEFAULT_PARITY);
	usart1_set_data(USART_DEFAULT_DATA);
	usart1_set_stop(USART_DEFAULT_STOP);
	usart1_set_baud(USART_DEFAULT_BAUD);

	//Enable receive 
	USART_CR1(USART1) |= USART_CR1_RE;

	//Enable receive interrupts
	USART_CR1(USART1) |= USART_CR1_RXNEIE;

	USART_DR(USART1)= 0;

	//Enable transmit interrupts
	USART_CR1(USART1) |= USART_CR1_TCIE;

	//Enable transmit 
	USART_CR1(USART1) |= USART_CR1_TE;

	//Enable USART1
	USART_CR1(USART1) |= USART_CR1_UE;

	//Enable interrupts
    nvic_enable_irq(NVIC_USART1_IRQ);

	return error;
}

mcu_error usart2_setup(void){

	uint8_t error = E_USART_NOERROR;

	//Setup the GPIO pins
	pinSetup(GPIO_UART_RX,PORTA,PIN3);
	pinSetup(GPIO_UART_TX,PORTA,PIN2);

	//Enable USART CLOCK
    rcc_periph_clock_enable(RCC_USART2);

	//Set default settings
	usart2_set_parity(USART_DEFAULT_PARITY);
	usart2_set_data(USART_DEFAULT_DATA);
	usart2_set_stop(USART_DEFAULT_STOP);
	usart2_set_baud(USART_DEFAULT_BAUD);

	//Enable receive 
	USART_CR1(USART2) |= USART_CR1_RE;

	//Enable receive interrupts
	USART_CR1(USART2) |= USART_CR1_RXNEIE;

	USART_DR(USART2)= 0;

	//Enable transmit interrupts
	USART_CR1(USART2) |= USART_CR1_TCIE;

	//Enable transmit 
	USART_CR1(USART2) |= USART_CR1_TE;

	//Enable USART1
	USART_CR1(USART2) |= USART_CR1_UE;

	//Enable interrupts
    nvic_enable_irq(NVIC_USART2_IRQ);

	return error;
}

mcu_error usart3_setup(void){

	uint8_t error = E_USART_NOERROR;

	//Setup the GPIO pins
	pinSetup(GPIO_UART_RX,PORTB,PIN11);
	pinSetup(GPIO_UART_TX,PORTB,PIN10);

	//Enable USART CLOCK
    rcc_periph_clock_enable(RCC_USART3);

	//Set default settings
	usart3_set_parity(USART_DEFAULT_PARITY);
	usart3_set_data(USART_DEFAULT_DATA);
	usart3_set_stop(USART_DEFAULT_STOP);
	usart3_set_baud(USART_DEFAULT_BAUD);

	//Enable receive 
	USART_CR1(USART3) |= USART_CR1_RE;

	//Enable receive interrupts
	USART_CR1(USART3) |= USART_CR1_RXNEIE;

	USART_DR(USART3)= 0;

	//Enable transmit interrupts
	USART_CR1(USART3) |= USART_CR1_TCIE;

	//Enable transmit 
	USART_CR1(USART3) |= USART_CR1_TE;

	//Enable USART1
	USART_CR1(USART3) |= USART_CR1_UE;

	//Enable interrupts
    nvic_enable_irq(NVIC_USART3_IRQ);

	return error;
}

/****************************************************************************/
//			     BAUD FUNCTIONS
/****************************************************************************/

mcu_error USART_setBaud(usart_periph peripheral, usart_baud baud){

	uint8_t error = E_USART_NOERROR;

	switch(peripheral){

		case USART_1:

			usart1_set_baud(baud);
			break;

		case USART_2:

			usart2_set_baud(baud);
			break;

		case USART_3:

			usart3_set_baud(baud);
			break;

		default:
			error =  E_USART_NOPORT;

			if(MCU_debugIsEnabled()){
				MCU_printError(E_USART_NOPORT);
			}
			break;
	}
	
	return error;

}

mcu_error usart1_set_baud(usart_baud baud){

	uint8_t error = E_USART_NOERROR;

	switch(baud){

		case USART_BAUD_2400:

            usart_set_baudrate(USART1,2400);
			break;

		case USART_BAUD_4800:

            usart_set_baudrate(USART1,4800);
			break;

		case USART_BAUD_9600:

            usart_set_baudrate(USART1,9600);
			break;

		case USART_BAUD_19200:

            usart_set_baudrate(USART1,19200);
			break;

		case USART_BAUD_38400:

            usart_set_baudrate(USART1,38400);
			break;

		case USART_BAUD_57600:
            usart_set_baudrate(USART1,57600);
			break;

		case USART_BAUD_115200:

            usart_set_baudrate(USART1,115200);
			break;
		default:
			error = E_USART_NOBAUD;

			if(MCU_debugIsEnabled()){
				MCU_printError(E_USART_NOBAUD);
			}
			break;

	}

	return error;
}

mcu_error usart2_set_baud(usart_baud baud){

	uint8_t error = E_USART_NOERROR;
	
	switch(baud){

		case USART_BAUD_2400:

            usart_set_baudrate(USART2,2400);
			break;

		case USART_BAUD_4800:

            usart_set_baudrate(USART2,4800);
			break;

		case USART_BAUD_9600:

            usart_set_baudrate(USART2,9600);
			break;

		case USART_BAUD_19200:

            usart_set_baudrate(USART2,19200);
			break;

		case USART_BAUD_38400:

            usart_set_baudrate(USART2,38400);
			break;

		case USART_BAUD_57600:
            usart_set_baudrate(USART2,57600);
			break;

		case USART_BAUD_115200:

            usart_set_baudrate(USART2,115200);
			break;
		default:
			error = E_USART_NOBAUD;

			if(MCU_debugIsEnabled()){
				MCU_printError(E_USART_NOBAUD);
			}
			break;

	}

	return error;
}

mcu_error usart3_set_baud(usart_baud baud){

	uint8_t error = E_USART_NOERROR;
	
	switch(baud){

		case USART_BAUD_2400:

            usart_set_baudrate(USART3,2400);
			break;

		case USART_BAUD_4800:

            usart_set_baudrate(USART3,4800);
			break;

		case USART_BAUD_9600:

            usart_set_baudrate(USART3,9600);
			break;

		case USART_BAUD_19200:

            usart_set_baudrate(USART3,19200);
			break;

		case USART_BAUD_38400:

            usart_set_baudrate(USART3,38400);
			break;

		case USART_BAUD_57600:
            usart_set_baudrate(USART3,57600);
			break;

		case USART_BAUD_115200:

            usart_set_baudrate(USART3,115200);
			break;
		default:
			error = E_USART_NOBAUD;

			if(MCU_debugIsEnabled()){
				MCU_printError(E_USART_NOBAUD);
			}
			break;

	}

	return error;
}
/****************************************************************************/
//			     STOP FUNCTIONS
/****************************************************************************/

mcu_error USART_setStop(usart_periph peripheral, usart_stop stop){

	uint8_t error = E_USART_NOERROR;

	switch(peripheral){

		case USART_1:

			usart1_set_stop(stop);
			break;

		case USART_2:

			usart2_set_stop(stop);
			break;

		case USART_3:

			usart3_set_stop(stop);
			break;

		default:
			error =  E_USART_NOPORT;

			if(MCU_debugIsEnabled()){
				MCU_printError(E_USART_NOPORT);
			}
			break;
	}
	
	return error;

}

mcu_error usart1_set_stop(usart_stop stop){

	uint8_t error = E_USART_NOERROR;

	switch(stop){

		case USART_STOP_ONE:

            usart_set_stopbits(USART1,USART_STOPBITS_1);
			break;

		case USART_STOP_TWO:

            usart_set_stopbits(USART1,USART_STOPBITS_2);
			break;

		default:
			error = E_USART_NOSTOP;

			if(MCU_debugIsEnabled()){
				MCU_printError(E_USART_NOSTOP);
			}
			break;
	}

	return error;
}

mcu_error usart2_set_stop(usart_stop stop){

	uint8_t error = E_USART_NOERROR;

	switch(stop){

		case USART_STOP_ONE:

            usart_set_stopbits(USART2,USART_STOPBITS_1);
			break;

		case USART_STOP_TWO:

            usart_set_stopbits(USART2,USART_STOPBITS_2);
			break;

		default:
			error = E_USART_NOSTOP;

			if(MCU_debugIsEnabled()){
				MCU_printError(E_USART_NOSTOP);
			}
			break;
	}

	return error;
}

mcu_error usart3_set_stop(usart_stop stop){

	uint8_t error = E_USART_NOERROR;

	switch(stop){

		case USART_STOP_ONE:

            usart_set_stopbits(USART3,USART_STOPBITS_1);
			break;

		case USART_STOP_TWO:

            usart_set_stopbits(USART3,USART_STOPBITS_2);
			break;

		default:
			error = E_USART_NOSTOP;

			if(MCU_debugIsEnabled()){
				MCU_printError(E_USART_NOSTOP);
			}
			break;
	}

	return error;
}

/****************************************************************************/
//			     DATA FUNCTIONS
/****************************************************************************/

mcu_error USART_setData(usart_periph peripheral,usart_data data){

	uint8_t error = E_USART_NOERROR;

	switch(peripheral){
		case USART_1:

			usart1_set_data(data);
			break;

		case USART_2:

			usart2_set_data(data);
			break;

		case USART_3:

			usart3_set_data(data);
			break;

		default:
			error =  E_USART_NOPORT;

			if(MCU_debugIsEnabled()){
				MCU_printError(E_USART_NOPORT);
			}
			break;
	}
	
	return error;
}

mcu_error usart1_set_data(usart_data data){

	uint8_t error = E_USART_NOERROR;

	switch(data){

		case USART_DATA_EIGHT:

            usart_set_databits(USART1,8);
			break;

		case USART_DATA_NINE:

            usart_set_databits(USART1,9);
			break;

		default:
			error = E_USART_NODATA;

			if(MCU_debugIsEnabled()){
				MCU_printError(E_USART_NODATA);
			}
			break;

	}

	return error;
}

mcu_error usart2_set_data(usart_data data){

	uint8_t error = E_USART_NOERROR;

	switch(data){

		case USART_DATA_EIGHT:

            usart_set_databits(USART2,8);
			break;

		case USART_DATA_NINE:

            usart_set_databits(USART2,9);
			break;

		default:
			error = E_USART_NODATA;

			if(MCU_debugIsEnabled()){
				MCU_printError(E_USART_NODATA);
			}
			break;

	}

	return error;
}

mcu_error usart3_set_data(usart_data data){

	uint8_t error = E_USART_NOERROR;

	switch(data){

		case USART_DATA_EIGHT:

            usart_set_databits(USART3,8);
			break;

		case USART_DATA_NINE:

            usart_set_databits(USART3,9);
			break;

		default:
			error = E_USART_NODATA;

			if(MCU_debugIsEnabled()){
				MCU_printError(E_USART_NODATA);
			}
			break;

	}

	return error;
}
/****************************************************************************/
//			     PARITY FUNCTIONS
/****************************************************************************/

mcu_error USART_setParity(usart_periph peripheral, usart_parity parity){

	uint8_t error = E_USART_NOERROR;

	switch(peripheral){

		case USART_1:

			usart1_set_parity(parity);
			break;
            
		case USART_2:

			usart2_set_parity(parity);
			break;
            
		case USART_3:

			usart3_set_parity(parity);
			break;

		default:
			error =  E_USART_NOPORT;

			if(MCU_debugIsEnabled()){
				MCU_printError(E_USART_NOPORT);
			}
			break;
	}
	
	return error;
}

mcu_error usart1_set_parity(usart_parity parity){
	
	uint8_t error = E_USART_NOERROR;
		

	switch(parity){

		case USART_EVEN:

            usart_set_databits(USART1,USART_PARITY_EVEN);
			break;

		case USART_ODD:

            usart_set_databits(USART1,USART_PARITY_ODD);
			break;

		case USART_NONE:

            usart_set_databits(USART1,USART_PARITY_NONE);
			break;

		default:
			error = E_USART_NOPARITY;

			if(MCU_debugIsEnabled()){
				MCU_printError(E_USART_NOPARITY);
			}
			break;
	}

	return error;
}

mcu_error usart2_set_parity(usart_parity parity){
	
	uint8_t error = E_USART_NOERROR;
		

	switch(parity){

		case USART_EVEN:

            usart_set_databits(USART2,USART_PARITY_EVEN);
			break;

		case USART_ODD:

            usart_set_databits(USART2,USART_PARITY_ODD);
			break;

		case USART_NONE:

            usart_set_databits(USART2,USART_PARITY_NONE);
			break;

		default:
			error = E_USART_NOPARITY;

			if(MCU_debugIsEnabled()){
				MCU_printError(E_USART_NOPARITY);
			}
			break;
	}

	return error;
}

mcu_error usart3_set_parity(usart_parity parity){
	
	uint8_t error = E_USART_NOERROR;
		

	switch(parity){

		case USART_EVEN:

            usart_set_databits(USART3,USART_PARITY_EVEN);
			break;

		case USART_ODD:

            usart_set_databits(USART3,USART_PARITY_ODD);
			break;

		case USART_NONE:

            usart_set_databits(USART3,USART_PARITY_NONE);
			break;

		default:
			error = E_USART_NOPARITY;

			if(MCU_debugIsEnabled()){
				MCU_printError(E_USART_NOPARITY);
			}
			break;
	}

	return error;
}

/****************************************************************************/
//			INPUT / OUTPUT FUNCTIONS
/****************************************************************************/

mcu_error USART_put(usart_periph peripheral, uint8_t byte){

	uint8_t error = E_USART_NOERROR;

	switch(peripheral){

		case USART_1:

			usart1_put(byte);
			break;

		case USART_2:

			usart2_put(byte);
			break;

		case USART_3:

			usart3_put(byte);
			break;

		default:
			error =  E_USART_NOPORT;

			if(MCU_debugIsEnabled()){
				MCU_printError(E_USART_NOPORT);
			}
			break;
	}

    return error;

}

v_fp_u8 USART_add_put(usart_periph peripheral){


	switch(peripheral){

		case USART_1:

			return &usart1_put;
			break;

		case USART_2:

			return &usart2_put;
			break;

		case USART_3:

			return &usart3_put;
			break;

		default:

			if(MCU_debugIsEnabled()){
				MCU_printError(E_USART_NOPORT);
			}
            return NULL;
			break;
	}

}

uint8_t USART_get(usart_periph peripheral){

    uint8_t rec;

	switch(peripheral){

		case USART_1:

			rec = usart1_get();
			break;

		case USART_2:

			rec = usart2_get();
			break;

		case USART_3:

			rec = usart3_get();
			break;

		default:

			if(MCU_debugIsEnabled()){
				MCU_printError(E_USART_NOPORT);
			}

			return E_USART_NOPORT;
			break;
	}

    return rec;

}

void usart1_put(uint8_t byte){

	//Wait for a the buffer to be empty to stop lost transmission bytes
	while(!(USART_SR(USART1) & USART_SR_TXE));

	USART_DR(USART1) = byte;

}

void usart2_put(uint8_t byte){

	//Wait for a the buffer to be empty to stop lost transmission bytes
	while(!(USART_SR(USART2) & USART_SR_TXE));

	USART_DR(USART2) = byte;

}

void usart3_put(uint8_t byte){

	//Wait for a the buffer to be empty to stop lost transmission bytes
	while(!(USART_SR(USART3) & USART_SR_TXE));

	USART_DR(USART3) = byte;

}

uint8_t usart1_get(void){

	//Wait for a character to be available to read
	while(!(USART_SR(USART1) & USART_SR_RXNE));

	return USART_DR(USART1);

}

uint8_t usart2_get(void){

	//Wait for a character to be available to read
	while(!(USART_SR(USART2) & USART_SR_RXNE));

	return USART_DR(USART2);

}

uint8_t usart3_get(void){

	//Wait for a character to be available to read
	while(!(USART_SR(USART3) & USART_SR_RXNE));

	return USART_DR(USART3);

}
/****************************************************************************/
//		         TX/RX ISR REDIRECT FUNCTIONS
/****************************************************************************/

mcu_error USART_setRxISR (usart_periph peripheral, void (*new_handler)(uint8_t received)){

	uint8_t error = E_USART_NOERROR;

	switch(peripheral){

		case USART_1:

			usart1_isr_rx = new_handler;
			break;

		case USART_2:

			usart2_isr_rx = new_handler;
			break;
            
		case USART_3:

			usart3_isr_rx = new_handler;
			break;

		default:
			error =  E_USART_NOPORT;

			if(MCU_debugIsEnabled()){
				MCU_printError(E_USART_NOPORT);
			}
			break;
	}
	return error;
}

mcu_error USART_setTxISR (usart_periph peripheral, void (*new_handler)(void)){

	uint8_t error = E_USART_NOERROR;

	switch(peripheral){

		case USART_1:

			usart1_isr_tx = new_handler;
			break;

		case USART_2:

			usart2_isr_tx = new_handler;
			break;

		case USART_3:

			usart3_isr_tx = new_handler;
			break;

		default:
			error =  E_USART_NOPORT;

			if(MCU_debugIsEnabled()){
				MCU_printError(E_USART_NOPORT);
			}
			break;
	}
	return error;
}

mcu_error USART_disableISR (usart_periph peripheral, usart_isr isr_target){

	uint8_t error = E_USART_NOERROR;

	switch(peripheral){

		case USART_1:

			
			if(USART_RX == isr_target){

				//Disable receive interrupts
				USART_CR1(USART1) &= ~USART_CR1_RXNEIE;
			}
			else if(USART_TX == isr_target){

				//Disable transmit interrupts
				USART_CR1(USART1) &= ~USART_CR1_TCIE;
				USART_CR1(USART1) &= ~USART_CR1_TXEIE;

			}
			else{
				error = E_USART_NOINT;
			}

			//Disable interrupts
            nvic_disable_irq(NVIC_USART1_IRQ);
			break;

		case USART_2:

			
			if(USART_RX == isr_target){

				//Disable receive interrupts
				USART_CR1(USART2) &= ~USART_CR1_RXNEIE;
			}
			else if(USART_TX == isr_target){

				//Disable transmit interrupts
				USART_CR1(USART2) &= ~USART_CR1_TCIE;
				USART_CR1(USART2) &= ~USART_CR1_TXEIE;

			}
			else{
				error = E_USART_NOINT;
			}

			//Disable interrupts
            nvic_disable_irq(NVIC_USART2_IRQ);
			break;

		case USART_3:

			
			if(USART_RX == isr_target){

				//Disable receive interrupts
				USART_CR1(USART3) &= ~USART_CR1_RXNEIE;
			}
			else if(USART_TX == isr_target){

				//Disable transmit interrupts
				USART_CR1(USART3) &= ~USART_CR1_TCIE;
				USART_CR1(USART3) &= ~USART_CR1_TXEIE;

			}
			else{
				error = E_USART_NOINT;
			}

			//Disable interrupts
            nvic_disable_irq(NVIC_USART3_IRQ);
			break;

		default:
			error =  E_USART_NOPORT;

			if(MCU_debugIsEnabled()){
				MCU_printError(E_USART_NOPORT);
			}
			break;
	}

	return error;


}

mcu_error USART_enableISR (usart_periph peripheral, usart_isr isr_target){

	uint8_t error = E_USART_NOERROR;

	switch(peripheral){

		case USART_1:

			if(USART_RX == isr_target){

				//Clear the data register to stop inital interrupts
				USART_DR(USART1) = 0;

				//Enable receive interrupts
				USART_CR1(USART1) |= USART_CR1_RXNEIE;

				//Enable interrupts
                nvic_enable_irq(NVIC_USART1_IRQ);
			}
			else if(USART_TX == isr_target){

				//Clear the data register to stop inital interrupts
				USART_DR(USART1) = 0;

				//Enable transmit interrupts
				USART_CR1(USART1) |= USART_CR1_TCIE;

				//Enable interrupts
				nvic_enable_irq(NVIC_USART1_IRQ);

			}
			else{
				error = E_USART_NOINT;
			}
			break;

		case USART_2:

			if(USART_RX == isr_target){

				//Clear the data register to stop inital interrupts
				USART_DR(USART2) = 0;

				//Enable receive interrupts
				USART_CR1(USART2) |= USART_CR1_RXNEIE;

				//Enable interrupts
                nvic_enable_irq(NVIC_USART2_IRQ);
			}
			else if(USART_TX == isr_target){

				//Clear the data register to stop inital interrupts
				USART_DR(USART2) = 0;

				//Enable transmit interrupts
				USART_CR1(USART2) |= USART_CR1_TCIE;

				//Enable interrupts
				nvic_enable_irq(NVIC_USART2_IRQ);

			}
			else{
				error = E_USART_NOINT;
			}
			break;

		case USART_3:

			if(USART_RX == isr_target){

				//Clear the data register to stop inital interrupts
				USART_DR(USART3) = 0;

				//Enable receive interrupts
				USART_CR1(USART3) |= USART_CR1_RXNEIE;

				//Enable interrupts
                nvic_enable_irq(NVIC_USART3_IRQ);
			}
			else if(USART_TX == isr_target){

				//Clear the data register to stop inital interrupts
				USART_DR(USART3) = 0;

				//Enable transmit interrupts
				USART_CR1(USART3) |= USART_CR1_TCIE;

				//Enable interrupts
				nvic_enable_irq(NVIC_USART3_IRQ);

			}
			else{
				error = E_USART_NOINT;
			}
			break;



		default:
			error =  E_USART_NOPORT;

			if(MCU_debugIsEnabled()){
				MCU_printError(E_USART_NOPORT);
			}
			break;
	}
	return error;

}

/****************************************************************************/
//		         	ISRs
/****************************************************************************/

void usart1_isr (void)
{
	//Check if called because of RXNE. 
	if (((USART_CR1(USART1) & USART_CR1_RXNEIE) != 0) && ((USART_SR(USART1) & USART_SR_RXNE) != 0)) {

		usart1_isr_rx(USART_DR(USART1));
	}

	// Check if called because of TXE. 
	if (((USART_CR1(USART1) & USART_CR1_TCIE) != 0) && ((USART_SR(USART1) & USART_SR_TC) != 0)) {

		USART_SR(USART1) &= ~USART_SR_TC;
		usart1_isr_tx();
	}
}

void usart2_isr (void)
{
	//Check if called because of RXNE. 
	if (((USART_CR1(USART2) & USART_CR1_RXNEIE) != 0) && ((USART_SR(USART2) & USART_SR_RXNE) != 0)) {

		usart2_isr_rx(USART_DR(USART2));
	}

	// Check if called because of TXE. 
	if (((USART_CR1(USART2) & USART_CR1_TCIE) != 0) && ((USART_SR(USART2) & USART_SR_TC) != 0)) {

		USART_SR(USART2) &= ~USART_SR_TC;
		usart2_isr_tx();
	}
}

void usart3_isr (void)
{
	//Check if called because of RXNE. 
	if (((USART_CR1(USART3) & USART_CR1_RXNEIE) != 0) && ((USART_SR(USART3) & USART_SR_RXNE) != 0)) {

		usart3_isr_rx(USART_DR(USART3));
	}

	// Check if called because of TXE. 
	if (((USART_CR1(USART3) & USART_CR1_TCIE) != 0) && ((USART_SR(USART3) & USART_SR_TC) != 0)) {

		USART_SR(USART3) &= ~USART_SR_TC;
		usart3_isr_tx();
	}
}

/****************************************************************************/
//		           DEFAULT ISR
/****************************************************************************/

void usart1_default_rx_isr(uint8_t c){

	//This exists to catch un-implemented ISR calls
    c++;

}

void usart1_default_tx_isr(void){

	//This exists to catch un-implemented ISR calls

}

void usart2_default_rx_isr(uint8_t c){

	//This exists to catch un-implemented ISR calls
    c++;

}

void usart2_default_tx_isr(void){

	//This exists to catch un-implemented ISR calls

}

void usart3_default_rx_isr(uint8_t c){

	//This exists to catch un-implemented ISR calls
    c++;

}

void usart3_default_tx_isr(void){

	//This exists to catch un-implemented ISR calls

}

