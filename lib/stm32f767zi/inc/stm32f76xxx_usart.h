#ifndef LIB_STM32F76XXX_USART_H
#define LIB_STM32F76XXX_USART_H

/**
  @addtogroup STM32F76XXX
  @{
*/

/*! 
    @addtogroup USART
    @brief Module for hardware USART peripheral configuration.
    @details Module for low level USART access. The default setup parameters used when 
    calling #USART_setup() are:

    - 9600bps baud
    - 1 Stop bit
    - 8 bit data frame
    - No parity

    @author Stuart Ianna
    @date November 2019
    @warning Partly tested, see repository readme for details. 
    @todo

  @{
*/

#include "stm32f76xxx_core.h"

#define USART_DEFAULT_BAUD      USART_BAUD_9600 //!<The default baud rate.
#define USART_DEFAULT_STOP      USART_STOP_ONE  //!<The default stop bit number.
#define USART_DEFAULT_PARITY    USART_NONE      //!<The default parity.
#define USART_DEFAULT_DATA      USART_DATA_8    //!<The default number of data bits.

/*! 
    @brief Peripheral ports available on the MCU.
    @details These values are used with function USART_setup()
*/
typedef enum{
    USART_3A,                                   //!<USART 3, RX = PB11, TX = PB10
    USART_3B,                                   //!<USART 3, RX = PC11, TX = PC10
    USART_3C,                                   //!<USART 3, RX = PD09, TX = PD08
}USART_Varient;

/*! 
    @brief Baud rate options for the port.
    @details These value are utilised with function USART_setBaud()
*/
typedef enum{
    USART_BAUD_4800     = 4800,                 //!<Baud rate of 4800
    USART_BAUD_9600     = 9600,                 //!<Baud rate of 9600
    USART_BAUD_19200    = 19200,                //!<Baud rate of 19200
    USART_BAUD_38400    = 38400,                //!<Baud rate of 38400
    USART_BAUD_57600    = 57600,                //!<Baud rate of 57600
    USART_BAUD_115200   = 115200,               //!<Baud rate of 115200
    USART_BAUD_230400   = 230400,               //!<Baud rate of 230400
    USART_BAUD_460800   = 460800,               //!<Baud rate of 460800
    USART_BAUD_921600   = 921600,               //!<Baud rate of 921600
}USART_Baud;

/*! 
    @brief Stop bit options available for the port.
    @details These values are utilised with function USART_setStop()
*/
typedef enum{
    USART_STOP_ONE      = 0x00,                 //!<One stop bit
    USART_STOP_TWO      = 0x02,                 //!<Two stop bits
}USART_Stop;

/*! 
    @brief Parity option available for the port.
    @details 
    These values are utilised with function USART_setParity()
*/
typedef enum{
    USART_EVEN          = 0x00,                 //!<Even parity
    USART_ODD           = 0x01,                 //!<Odd parity
    USART_NONE          = 0x02,                 //!<No parity
}USART_Parity;

/*! 
    @brief ISR values for the port.
    @details These values are utilised with function USART_enableISR() and USART_disableISR()
*/
typedef enum{
    USART_RX            = 0x00,                 //!<Receive interrupts 
    USART_TX            = 0x01,                 //!<Transmit interrupts
}USART_Isr;

/*! 
    @brief Data frame lengths for the port.
    @details These values are utilised with function USART_setData()
*/
typedef enum{
    USART_DATA_8        = 0x00,                 //!<8 Data bits
    USART_DATA_9        = 0x01,                 //!<9 Data bits
    USART_DATA_7        = 0x02,                 //!<7 Data bits
}USART_Data;

/*! 
    @brief Initialize USART port.
    @details This must be called before any other feature is used.

	This sets up the given USART peripheral to the default macro defined settings.
    @param peripheral The USART peripheral to modify.
    @return ERROR or SUCCESS
*/
bool USART_setup(USART_Varient peripheral);
/*! 
    @brief Send a byte on the given peripheral.
    @details 
    @param peripheral The usart number to send on.
    @param byte The byte to send.
    @return None.
*/
void USART_put(USART_TypeDef *peripheral, uint8_t byte);
/*! 
    @brief Get a byte on the given peripheral (blocking).
    @details 
    @param peripheral The usart number to send on.
    @return The byte recieved on the port.
*/
uint8_t USART_get(USART_TypeDef *peripheral);
/*! 
    @brief Set the data frame size for the port.
    @details 
    @param peripheral The USART peripheral to modify.
    @param data Data frame size as define in in #USART_Data.
    @return SUCCESS or ERROR.
*/
bool USART_setData(USART_TypeDef *peripheral, USART_Data data);
/*! 
    @brief Set the parity for the port.
    @details 
    @param peripheral The USART peripheral to modify.
    @param parity Parity option defined in #USART_Parity.
    @return SUCCESS or ERROR.
*/
bool USART_setParity(USART_TypeDef *peripheral, USART_Parity parity);
/*! 
    @brief Set the number of stop bits for the port.
    @details 
    @param peripheral The USART peripheral to modify.
    @param stop Number of stop bits to use defined in #USART_Stop.
    @return SUCCESS or ERROR
*/
bool USART_setStop(USART_TypeDef *peripheral, USART_Stop stop);
/*! 
    @brief Set the baud rate for the port.
    @details 
    @param peripheral The USART peripheral to modify.
    @param baud Baud rate defined by #USART_Baud to be used.
    @return SUCCESS or ERROR.
*/
bool USART_setBaud(USART_TypeDef *peripheral, USART_Baud baud);


/**@}*/
/**@}*/

#endif
