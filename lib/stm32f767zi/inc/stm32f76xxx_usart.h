#ifndef LIB_STM32F76XXX_USART_HPP
#define LIB_STM32F76XXX_USART_HPP

/**
  @addtogroup STM32F76XXX
  @{
*/

/*! 
    @addtogroup USART
    @details Module for low level USART access.
    @author Stuart Ianna
    @date November 2019
    @warning Partly tested, see repository readme for details. 
    @todo

  @{
*/

#include "stm32f76xxx_core.h"

#define USART_DEFAULT_BAUD      USART_BAUD_9600
#define USART_DEFAULT_STOP      USART_STOP_ONE
#define USART_DEFAULT_PARITY    USART_NONE
#define USART_DEFAULT_DATA      USART_DATA_8

typedef enum{
    USART_3A,                                   //!<USART 3, RX = PB11, TX = PB10
    USART_3B,                                   //!<USART 3, RX = PC11, TX = PC10
    USART_3C,                                   //!<USART 3, RX = PD09, TX = PD08
}USART_Varient;

typedef enum{
    USART_BAUD_9600     = 9600,                 //!<Baud rate of 9600
    USART_BAUD_19200    = 19200,                //!<Baud rate of 9600
    USART_BAUD_38400    = 38400,                //!<Baud rate of 9600
    USART_BAUD_57600    = 57600,                //!<Baud rate of 9600
    USART_BAUD_115200   = 115200,               //!<Baud rate of 9600
    USART_BAUD_230400   = 230400,               //!<Baud rate of 9600
    USART_BAUD_460800   = 460800,               //!<Baud rate of 9600
    USART_BAUD_921600   = 921600,               //!<Baud rate of 9600
}USART_Baud;

typedef enum{
    USART_STOP_ONE      = 0x00,                 //!<One stop bit
    USART_STOP_TWO      = 0x02,                 //!<Two stop bits
}USART_Stop;

typedef enum{
    USART_EVEN          = 0x00,                 //!<Even parity
    USART_ODD           = 0x01,                 //!<Odd parity
    USART_NONE          = 0x02,                 //!<No parity
}USART_Parity;

typedef enum{
    USART_RX            = 0x00,                 //!<Receive interrupts 
    USART_TX            = 0x01,                 //!<Transmit interrupts
}USART_Isr;

typedef enum{
    USART_DATA_8        = 0x00,                 //!<8 Data bits
    USART_DATA_9        = 0x01,                 //!<9 Data bits
    USART_DATA_7        = 0x02,                 //!<7 Data bits
}USART_Data;

bool USART_setup(USART_Varient peripheral);
void USART_put(USART_TypeDef *peripheral, uint8_t byte);
uint8_t USART_get(USART_TypeDef *peripheral);
bool USART_setData(USART_TypeDef *peripheral, USART_Data data);
bool USART_setParity(USART_TypeDef *peripheral, USART_Parity parity);
bool USART_setStop(USART_TypeDef *peripheral, USART_Stop stop);
bool USART_setBaud(USART_TypeDef *peripheral, USART_Baud baud);


/**@}*/
/**@}*/

#endif
