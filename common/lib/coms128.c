/*!
    @file coms128.c
    @brief Source file for coms128.h
    @author Stuart Ianna
    @version 1.0
    @date June 2018
    @copyright GNU GPLv3
    @warning None
    @bug 

    @details

    @par Verified Compilers
    - arm-none-eabi-gcc (15:4.9.3+svn231177-1) 4.9.3 20150529 (prerelease)
*/

#include "coms128.h"

void com128_dataSent(void);
void com128_dataOut(uint8_t byte);

//FIFO8 object and memory buffer
static FIFO8 tx;
static uint8_t tx_buf[TX_SIZE];

//Connects all objects together, see module examples
ret_point COMOUT_con128(void (*put)(uint8_t)){

    FIFO8_init(&tx,FIFO8_AUTO,tx_buf,TX_SIZE,put);
    STRING11_setOutput(&com128_dataOut);
    return &com128_dataSent;
}

//This is called now by USART.txISR
void com128_dataSent(void){

    FIFO8_get(&tx);

}

//This is called by STRING11 output calls
void com128_dataOut(uint8_t byte){

    FIFO8_put(&tx,byte);

}
