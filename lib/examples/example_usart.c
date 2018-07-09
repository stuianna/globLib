/****************************************************************************/
/*
This program demonstrates USART module functionallity.

INSTRUCTIONS:

Copy this program into src directory, change include "core" library, compile and run.

VERISON 1.0
DATE 5.5.18
AUTHOR Stuart Ianna

PROGRAM ORDER:
	
Setup:
	1. Minimum required hardware is intialised
	2. Optional hardware options are demonstrated
	3. Transmit and recieve ISRs are overriden to custrom functions
	4. GPIO port is setup to digital output for LED (optional)

Begin:	
	1. Interrupts are disabled to demonstrate blocking "basic" function
	2. Message "Go?" printed to terminal
	3. Wait for single key responce from termainal
	4. Interrupts are enabled 

Infinite continuation:
	1. Characters recieved form terminal are return using ISR
	2. On completion of byte transfer the specified LED pin is toggled

VERIFIED COMPILERS
	-arm-none-eabi-gcc (15:4.9.3+svn231177-1) 4.9.3 20150529 (prerelease)

*/
/****************************************************************************/
//Change to include appropriate MCU target
#include "stm32f103c8_core.h"

//Optional macros for LED toggle on USART action
#define LED_PORT PORTC
#define LED_PIN PORT13

//Create an instance for the USART object
USART serial;

//Overriden ISR functions
void dataAvailable(uint8_t byte);
void dataSent(void);

int main(void)
{

	/*********************************************************************/
	//			Minimum Hardware setup
	/*********************************************************************/
	
	//Initialize USART for default parameters
	//This function needs to be called before USART will work
	//	Baud = 		9600
	//	Stop Bits = 	1
	//	Parity = 	None
	//	Data bits = 	8
	//	Interrupts = 	Enabled
	
	//Param 1 = Pointer to USART object
	//Param 2 = USART peripheral to use: 
	//	USART_1
	//	USART_2
	//	USART_3
	//	USART_4
	//	USART_5
	USART_setup(&serial,USART_1); 	

	/*********************************************************************/
	//			OPTIONAL setup parameter
	/*********************************************************************/

	
	//Set the baud rate for the port
	//Param 1 = Pointer to USART object
	//Param 2 = Baud rate
	//	USART_BAUD_4800
	//	USART_BAUD_9600
	//	USART_BAUD_19200
	//	USART_BAUD_38400
	//	USART_BAUD_57600
	//	USART_BAUD_115200
	USART_setBaud(&serial,USART_BAUD_9600);

	//Set the stop bits for the port
	//Param 1 = Pointer to USART object
	//Param 2 = Number of stop bits
	//	USART_STOP_ONE 
	//	USART_STOP_two
	USART_setStop(&serial,USART_STOP_ONE);

	//Set the parity for the port
	//Param 1 = Pointer to USART object
	//Param 2 = Parity
	//	USART_PARITY_NONE
	//	USART_PARITY_ODD
	//	USART_PARITY_EVEN
	USART_setParity(&serial,USART_PARITY_NONE);

	//Set the data bits for the port
	//Param 1 = Pointer to USART object
	//Param 2 = Number of data bits
	//	USART_DATA_EIGHT
	//	USART_DATA_NINE
	USART_setData(&serial,USART_DATA_EIGHT);

	//Disable ISR (Tx & Rx) for port
	//Param 1 = Pointer to USART object
	USART_disableISR(&serial,USART_TX);
	USART_disableISR(&serial,USART_RX);

	//Enable ISR (Tx & Rx) for port
	//Param 1 = Pointer to USART object
	USART_enableISR(&serial,USART_TX);
	USART_enableISR(&serial,USART_RX);

	//Override the Rx ISR, this gets called when a byte is availble to read
	//Param1 = Pointer to the USART object
	//Param2 = Function pointer to new handler
	//	return = void
	//	params = uint8_t
	USART_setRxISR(&serial,&dataAvailable);


	//Override the Tx ISR, this gets called when a byte is finished being transmitted 
	//Param1 = Pointer to the USART object
	//Param2 = Function pointer to new handler
	//	return = void
	//	params = void
	USART_setTxISR(&serial,&dataSent);

	//Enable the debug feature, (disabled by default)
	//Param1 = Pointer to the debug output function
	//	return none
	USART_debugEnable(serial.put);

	//Enable the debug feature
	//	param = none
	//	return = none
	USART_debugDisable();

	/*********************************************************************/
	//		GPIO pin setup (optional for LED toggle)
	/*********************************************************************/
	pinSetup(GPIO_DO,PORTC,PIN13);
	

	/*********************************************************************/
	//			Begin main Demo
	/*********************************************************************/


	//Disable ISR to demonstrate "blocking read"
	USART_disableISR(&serial,USART_RX);
	USART_disableISR(&serial,USART_TX);

	//Send one byte through USART
	//Subsequent bytes don't send until previous transmittion has finished
       	serial.put('\n');
       	serial.put('\r');
	serial.put('G');
	serial.put('o');
	serial.put('?');
       	serial.put('\n');
       	serial.put('\r');

	//Wait for responce from terminal, "get" loses functionallity when interrupts are enabled
	char responce = serial.get();

	//Print the responce
	serial.put(responce);
	serial.put('\n');
       	serial.put('\r');

	//Now enable ISR to demonstrate "Normal" function
	USART_enableISR(&serial,USART_RX);
	USART_enableISR(&serial,USART_TX);

	/*********************************************************************/
	//			 Main Loop
	/*********************************************************************/


	while(1){

		//Nothing to do here, everything handled in ISRs
	}

	return 0;
}

//This is the override function set above for RX ISR
//This is called when a byte has been received and is ready to be read.
void dataAvailable(uint8_t byte){

	//Echo the character back to the terminal
	serial.put(byte);
}

//This is the override function set above for TX ISR
//This is called when a byte placed on the buffer has finished being transmitted
void dataSent(void){

	//Toggle the LED to show functionallity
	pinToggle(PORTC,PIN13);
}

