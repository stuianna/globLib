/*!**************************************************************************
    @file stm32f103cb_spi.c
    @brief Source file for stm32f103cb SPI
    @author Stuart Ianna
    @version 0.1
    @date July 2018
    @copyright GNU GPLv3
    @warning None
    @bug 

    @details

    @par Compilers
    - arm-none-eabi-gcc (15:4.9.3+svn231177-1) 4.9.3 20150529 (prerelease)
******************************************************************************/

#include "stm32f103cb_spi.h"

//Used when calculating the serial clock divisor
void spi_setSpeed(spi_periph periph, uint16_t divisor);

void spi1_put(uint16_t data);
void spi2_put(uint16_t data);
uint16_t spi1_get(void);
uint16_t spi2_get(void);

//ISR function pointers
static void (*spi1_tx_handler)(void);
static void (*spi1_rx_handler)(uint16_t);
static void (*spi2_tx_handler)(void);
static void (*spi2_rx_handler)(uint16_t);

mcu_error SPI_setup(spi_periph periph){

    switch(periph){

        case SPI_1:

            //SPI clock
            rcc_periph_clock_enable(RCC_SPI1);

            //GPIO pins
            pinSetup(GPIO_SPI_OUT,PORTA,PIN5);
            pinSetup(GPIO_SPI_OUT,PORTA,PIN7);
            pinSetup(GPIO_SPI_IN,PORTA,PIN6);

            spi_reset(SPI1);
            SPI_setSpeed(periph,SPI_DEFAULT_SPEED);
            SPI_setClockPolarity(periph,SPI_DEFAULT_CLOCK);
            SPI_setPhase(periph,SPI_DEFAULT_PHASE);
            SPI_setDataFrame(periph,SPI_DEFAULT_DATA);
            SPI_setEndian(periph,SPI_DEFAULT_ENDIAN);
            spi_enable_software_slave_management(SPI1);
            spi_set_nss_high(SPI1);
            spi_set_master_mode(SPI1);
            spi_enable(SPI1);
            break;

        case SPI_2:

            //SPI clock
            rcc_periph_clock_enable(RCC_SPI2);

            //GPIO pins
            pinSetup(GPIO_SPI_OUT,PORTB,PIN15);     
            pinSetup(GPIO_SPI_OUT,PORTB,PIN13);
            pinSetup(GPIO_SPI_IN,PORTB,PIN14);

            spi_reset(SPI2);
            SPI_setSpeed(periph,SPI_DEFAULT_SPEED);
            SPI_setClockPolarity(periph,SPI_DEFAULT_CLOCK);
            SPI_setPhase(periph,SPI_DEFAULT_PHASE);
            SPI_setDataFrame(periph,SPI_DEFAULT_DATA);
            SPI_setEndian(periph,SPI_DEFAULT_ENDIAN);
            spi_enable_software_slave_management(SPI2);
            spi_set_nss_high(SPI2);
            spi_set_master_mode(SPI2);
            spi_enable(SPI2);
            break;

        default:

            if(MCU_debugIsEnabled()){

                MCU_printError(E_SPI_PORT);
            }
            return E_SPI_PORT;
            break;
    }

    return E_SPI_NOERROR;

}
mcu_error SPI_enableTxISR(spi_periph periph, void(*handler)(void)){

    switch(periph){

        case SPI_1:

            nvic_disable_irq(NVIC_SPI1_IRQ);
            spi1_tx_handler = handler;
            spi_enable_tx_buffer_empty_interrupt(SPI1);
            nvic_enable_irq(NVIC_SPI1_IRQ);
            break;

        case SPI_2:

            nvic_disable_irq(NVIC_SPI2_IRQ);
            spi2_tx_handler = handler;
            spi_enable_tx_buffer_empty_interrupt(SPI2);
            nvic_enable_irq(NVIC_SPI2_IRQ);
            break;

        default:

            if(MCU_debugIsEnabled()){

                MCU_printError(E_SPI_PORT);
            }
            return E_SPI_PORT;
            break;
    }
    return E_SPI_NOERROR;
}

mcu_error SPI_enableRxISR(spi_periph periph, void(*handler)(uint16_t)){

    switch(periph){

        case SPI_1:

            nvic_disable_irq(NVIC_SPI1_IRQ);
            spi1_rx_handler = handler;
            spi_enable_rx_buffer_not_empty_interrupt(SPI1);
            nvic_enable_irq(NVIC_SPI1_IRQ);
            break;

        case SPI_2:

            nvic_disable_irq(NVIC_SPI2_IRQ);
            spi2_rx_handler = handler;
            spi_enable_rx_buffer_not_empty_interrupt(SPI2);
            nvic_enable_irq(NVIC_SPI2_IRQ);
            break;

        default:

            if(MCU_debugIsEnabled()){

                MCU_printError(E_SPI_PORT);
            }
            return E_SPI_PORT;
            break;
    }

    return E_SPI_NOERROR;
}

mcu_error SPI_disableTxISR(spi_periph periph){

    switch(periph){

        case SPI_1:

            spi_disable_tx_buffer_empty_interrupt(SPI1);

            //Disable nvic if no interrupts enabled
            if(!(SPI_CR2(SPI1) & SPI_CR2_RXNEIE)){

                nvic_disable_irq(NVIC_SPI1_IRQ);
            }
            break;

        case SPI_2:

            spi_disable_tx_buffer_empty_interrupt(SPI2);

            //Disable nvic if no interrupts enabled
            if(!(SPI_CR2(SPI2) & SPI_CR2_RXNEIE)){

                nvic_disable_irq(NVIC_SPI2_IRQ);
            }
            break;

        default:

            if(MCU_debugIsEnabled()){

                MCU_printError(E_SPI_PORT);
            }
            return E_SPI_PORT;
            break;
    }

    return E_SPI_NOERROR;
}

mcu_error SPI_disableRxISR(spi_periph periph){

    switch(periph){

        case SPI_1:

            spi_disable_rx_buffer_not_empty_interrupt(SPI1);

            //Disable nvic if no interrupts enabled
            if(!(SPI_CR2(SPI1) & SPI_CR2_TXEIE)){

                nvic_disable_irq(NVIC_SPI1_IRQ);
            }
            break;

        case SPI_2:

            spi_disable_rx_buffer_not_empty_interrupt(SPI2);
            //Disable nvic if no interrupts enabled
            if(!(SPI_CR2(SPI1) & SPI_CR2_TXEIE)){

                nvic_disable_irq(NVIC_SPI2_IRQ);
            }
            break;

        default:

            if(MCU_debugIsEnabled()){

                MCU_printError(E_SPI_PORT);
            }
            return E_SPI_PORT;
            break;
    }

    return E_SPI_NOERROR;
}

void spi1_isr(void){

    if(SPI_SR(SPI1) & SPI_SR_TXE){

        spi1_tx_handler();
    }
    if(SPI_SR(SPI1) & SPI_SR_RXNE){

        spi1_rx_handler(SPI_DR(SPI1));
    }
}

void spi2_isr(void){

    if(SPI_SR(SPI2) & SPI_SR_TXE){

        spi2_tx_handler();
    }
    if(SPI_SR(SPI2) & SPI_SR_RXNE){

        spi2_rx_handler(SPI_DR(SPI2));
    }


}

mcu_error SPI_setSpeed(spi_periph periph, spi_speed speed){

    
    switch(periph){

        case SPI_1:

            spi_setSpeed(periph,CLOCK_getSpeed()/speed);
            break;

        case SPI_2:

            spi_setSpeed(periph,CLOCK_getSpeedAlternate()/speed);
            break;

        default:

            if(MCU_debugIsEnabled()){

                MCU_printError(E_SPI_PORT);
            }
            return E_SPI_PORT;
            break;
    }

    return E_SPI_NOERROR;

}

void spi_setSpeed(spi_periph periph, uint16_t divisor){

    uint8_t scalar;

    if(divisor > 128){
        
        scalar = SPI_CR1_BR_FPCLK_DIV_256;
    }
    else if(divisor > 64){
        
        scalar = SPI_CR1_BR_FPCLK_DIV_128;
    }
    else if(divisor > 32){
        
        scalar = SPI_CR1_BR_FPCLK_DIV_64;
    }
    else if(divisor > 16){
        
        scalar = SPI_CR1_BR_FPCLK_DIV_32;
    }
    else if(divisor > 8){
        
        scalar = SPI_CR1_BR_FPCLK_DIV_16;
    }
    else if(divisor > 4){
        
        scalar = SPI_CR1_BR_FPCLK_DIV_8;
    }
    else if(divisor > 2){
        
        scalar = SPI_CR1_BR_FPCLK_DIV_4;
    }
    else{

        scalar = SPI_CR1_BR_FPCLK_DIV_2;
    }

    switch(periph){

        case SPI_1:

            spi_set_baudrate_prescaler(SPI1,scalar);
            break;

        case SPI_2:

            spi_set_baudrate_prescaler(SPI2,scalar);
            break;
    }

    return;
}

mcu_error SPI_setClockPolarity(spi_periph periph,spi_clock clock){

    switch(periph){

        case SPI_1:

            if(clock == SPI_CLOCK_HIGH){

                spi_set_clock_polarity_1(SPI1);

            }
            else{

                spi_set_clock_polarity_0(SPI1);
            }

            break;

        case SPI_2:

            if(clock == SPI_CLOCK_HIGH){

                spi_set_clock_polarity_1(SPI2);

            }
            else{

                spi_set_clock_polarity_0(SPI2);
            }
            break;

        default:

            if(MCU_debugIsEnabled()){

                MCU_printError(E_SPI_PORT);
            }
            return E_SPI_PORT;
            break;
    }

    return E_SPI_NOERROR;
}

mcu_error SPI_setDataFrame(spi_periph periph,spi_data data){

    switch(periph){

        case SPI_1:

            if(data == SPI_DATA_8){

                spi_set_dff_8bit(SPI1);

            }
            else{

                spi_set_dff_16bit(SPI1);
            }
            break;

        case SPI_2:

            if(data == SPI_DATA_8){

                spi_set_dff_8bit(SPI2);

            }
            else{

                spi_set_dff_16bit(SPI2);
            }
            break;

        default:

            if(MCU_debugIsEnabled()){

                MCU_printError(E_SPI_PORT);
            }
            return E_SPI_PORT;
            break;
    }

    return E_SPI_NOERROR;
}

mcu_error SPI_setEndian(spi_periph periph,spi_endian endian){

    switch(periph){

        case SPI_1:

            if(endian == SPI_LSB){

                spi_send_lsb_first(SPI1);
            }
            else{

                spi_send_msb_first(SPI1);
            }
            break;

        case SPI_2:

            if(endian == SPI_LSB){

                spi_send_lsb_first(SPI2);
            }
            else{

                spi_send_msb_first(SPI2);
            }
            break;

        default:

            if(MCU_debugIsEnabled()){

                MCU_printError(E_SPI_PORT);
            }
            return E_SPI_PORT;
            break;
    }

    return E_SPI_NOERROR;
}

mcu_error SPI_setPhase(spi_periph periph,spi_phase phase){

    switch(periph){

        case SPI_1:

            if(phase == SPI_PHASE_FIRST){

                spi_set_clock_phase_0(SPI1);
            }
            else{

                spi_set_clock_phase_1(SPI1);
            }
            break;

        case SPI_2:

            if(phase == SPI_PHASE_FIRST){

                spi_set_clock_phase_0(SPI2);
            }
            else{

                spi_set_clock_phase_1(SPI2);
            }
            break;
        default:

            if(MCU_debugIsEnabled()){

                MCU_printError(E_SPI_PORT);
            }
            return E_SPI_PORT;
            break;
    }

    return E_SPI_NOERROR;
}

spi_u16_fp_v SPI_getGetAdd(spi_periph periph){

    switch(periph){

        case SPI_1:

            return &spi1_get;
            break;

        case SPI_2:

            return &spi2_get;
            break;
    }

    return NULL;

}

spi_v_fp_u16 SPI_getPutAdd(spi_periph periph){

    switch(periph){

        case SPI_1:

            return &spi1_put;
            break;

        case SPI_2:

            return &spi2_put;
            break;
    }

    return NULL;
}

mcu_error SPI_put(spi_periph periph, uint16_t data){

    switch(periph){

        case SPI_1:
            
            spi1_put(data);
            break;

        case SPI_2:

            spi2_put(data);
            break;

        default:

            if(MCU_debugIsEnabled()){

                MCU_printError(E_SPI_PORT);
            }
            return E_SPI_PORT;
            break;
    }

    return E_SPI_NOERROR;

}

uint16_t SPI_get(spi_periph periph){

    switch(periph){

        case SPI_1:

            return spi1_get();
            break;

        case SPI_2:

            return spi2_get();
            break;
        default:

            return 0;
            break;
    }
    return 0;
}

void spi1_put(uint16_t data){

    //Wait for data to be sent
    SPI_DR(SPI1) = data;
    while(SPI_SR(SPI1) & SPI_SR_BSY);

}

void spi2_put(uint16_t data){

    //Wait for data to be sent
    SPI_DR(SPI2) = data;
    while(SPI_SR(SPI2) & SPI_SR_BSY);
}

uint16_t spi1_get(){

    //Wait for something to arrive
    while(SPI_SR(SPI1) & SPI_SR_BSY);
    return SPI_DR(SPI1);
}

uint16_t spi2_get(){

    //Wait for something to arrive
    while(SPI_SR(SPI2) & SPI_SR_BSY);
    return SPI_DR(SPI2);
}

void SPI_write(spi_periph periph, gpio_port port, gpio_pin pin, 
        spi_csPol polarity, uint16_t address, uint16_t data){

    if(polarity == SPI_CS_HIGH){

        pinLow(port,pin);
        pinHigh(port,pin);
    }
    else{

        pinHigh(port,pin);
        pinLow(port,pin);
    }
    
    SPI_put(periph,address);
    SPI_put(periph,data);

    if(polarity == SPI_CS_HIGH){

        pinLow(port,pin);
    }
    else{

        pinHigh(port,pin);
    }
    
}

void SPI_repeatWrite(spi_periph periph, gpio_port port, gpio_pin pin, 
        spi_csPol polarity, uint16_t address, uint16_t *data, uint16_t count){

    //More efficient to switch to SPI port here
    if(polarity == SPI_CS_HIGH){

        pinLow(port,pin);
        pinHigh(port,pin);
    }
    else{

        pinHigh(port,pin);
        pinLow(port,pin);
    }
    
    switch(periph){

        case SPI_1:

            spi1_put(address);
            for(uint16_t i = 0; i < count; i++){

                spi1_put(data[i]);
            }
            break;

        case SPI_2:

            spi2_put(address);
            for(uint16_t i = 0; i < count; i++){

                spi2_put(data[i]);
            }
            break;

        default:
            break;
    }

    if(polarity == SPI_CS_HIGH){

        pinLow(port,pin);
    }
    else{

        pinHigh(port,pin);
    }
    
}

uint16_t SPI_read(spi_periph periph, gpio_port port, gpio_pin pin, 
        spi_csPol polarity, uint16_t address){

    uint16_t data = 0;

    //More efficient to switch to SPI port here
    if(polarity == SPI_CS_HIGH){

        pinLow(port,pin);
        pinHigh(port,pin);
    }
    else{

        pinHigh(port,pin);
        pinLow(port,pin);
    }

    switch(periph){

        case SPI_1:

            spi1_put(address);
            spi1_get();         //Dummy read to clear receive buffer
            spi1_put(0xFF);     //Dummy write to get some data back
            data = spi1_get();
            break;

        case SPI_2:

            spi2_put(address);
            spi2_get();         //Dummy read to clear receive buffer
            spi2_put(0xFF);     //Dummy write to get some data back
            data = spi2_get();
            break;

        default:
            break;
    }


    //More efficient to switch to SPI port here
    if(polarity == SPI_CS_HIGH){

        pinLow(port,pin);
    }
    else{

        pinHigh(port,pin);
    }
    
    return data;
}

void SPI_repeatRead(spi_periph periph, gpio_port port, gpio_pin pin, 
        spi_csPol polarity, uint16_t address, uint16_t *data, uint16_t count){

    //More efficient to switch to SPI port here
    if(polarity == SPI_CS_HIGH){

        pinLow(port,pin);
        pinHigh(port,pin);
    }
    else{

        pinHigh(port,pin);
        pinLow(port,pin);
    }
    

    switch(periph){

        case SPI_1:

            spi1_put(address);
            spi1_get();         //Dummy read to clear receive buffer

            for(uint16_t i = 0; i < count; i++){

                spi1_put(0xFF);     //Dummy write to get some data back
                data[i] = spi1_get();
            }

            break;

        case SPI_2:

            spi2_put(address);
            spi2_get();         //Dummy read to clear receive buffer
            for(uint16_t i = 0; i < count; i++){

                spi2_put(0xFF);     //Dummy write to get some data back
                data[i] = spi2_get();
            }
            break;

        default:
            break;
    }

    //More efficient to switch to SPI port here
    if(polarity == SPI_CS_HIGH){

        pinLow(port,pin);
    }
    else{

        pinHigh(port,pin);
    }
    
}

