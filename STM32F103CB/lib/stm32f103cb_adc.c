/*!
    @file stm32f103cb_adc.c
    @brief Source file for stm32f103cb ADC 
    @author Stuart Ianna
    @version 0.1
    @date June 2018
    @copyright GNU GPLv3
    @warning None
    @bug 

    @details

    @par Compilers
    - arm-none-eabi-gcc (15:4.9.3+svn231177-1) 4.9.3 20150529 (prerelease)
*/

#include "stm32f103cb_adc.h"

void adc_init(void);

/****************************************************************************/
//			                SETUP FUNCTIONS
/****************************************************************************/

void adc_init(void){

    rcc_periph_clock_enable(RCC_ADC1);
    adc_power_off(ADC1);

    //No clock division
    rcc_set_adcpre(RCC_CFGR_ADCPRE_PCLK2_DIV2);
    adc_set_dual_mode(ADC_CR1_DUALMOD_IND);
    adc_disable_scan_mode(ADC1);

    //One sample then stop
    adc_set_single_conversion_mode(ADC1);

    //Software trigger
    adc_enable_external_trigger_regular(ADC1, ADC_CR2_EXTSEL_SWSTART);

    //Calibrate ADC
    adc_power_on(ADC1);
    adc_reset_calibration(ADC1);

    adc_calibrate(ADC1);
    return;

}

mcu_error ADC_setup(gpio_port port, gpio_pin pin){

    adc_init();
    return ADC_addChannel(port,pin);
}

uint16_t ADC_addChannel(gpio_port port, gpio_pin pin){

    switch(port){

        case PORTA:

        switch(pin){

                case PIN0:

                    pinSetup(GPIO_ADC,PORTA,PIN0);
                    break;

                case PIN1:

                    pinSetup(GPIO_ADC,PORTA,PIN1);
                    break;

                case PIN2:

                    pinSetup(GPIO_ADC,PORTA,PIN2);
                    break;

                case PIN3:

                    pinSetup(GPIO_ADC,PORTA,PIN3);
                    break;

                case PIN4:

                    pinSetup(GPIO_ADC,PORTA,PIN4);
                    break;

                case PIN5:

                    pinSetup(GPIO_ADC,PORTA,PIN5);
                    break;

                case PIN6:

                    pinSetup(GPIO_ADC,PORTA,PIN6);
                    break;

                case PIN7:

                    pinSetup(GPIO_ADC,PORTA,PIN7);
                    break;

                default:

                    //All other pins have no Adc
                    if(MCU_debugIsEnabled()){

                        MCU_printError(E_ADC_PIN);
                    }
                    break;
        }
            break;

        case PORTB:

            switch(pin){

                case PIN0:

                    pinSetup(GPIO_ADC,PORTB,PIN0);
                    break;

                case PIN1:

                    pinSetup(GPIO_ADC,PORTB,PIN1);
                    break;

                default:

                    //All other pins have no Adc
                    if(MCU_debugIsEnabled()){

                        MCU_printError(E_ADC_PIN);
                    }
                    return E_ADC_PIN;

                    break;

            }
            
            break;

        case PORTC:
            
            //No pins on this port have an ADC
			if(MCU_debugIsEnabled()){
				MCU_printError(E_ADC_PORT);
			}
            return E_ADC_PORT;

            break;

        default:

            //No pins on this port have an ADC
			if(MCU_debugIsEnabled()){
				MCU_printError(E_ADC_PORT);
			}
            return E_ADC_PORT;
            break;

    }

    return E_ADC_NOERROR;

}
/****************************************************************************/
//				               Sample functions 
/****************************************************************************/

uint16_t ADC_sample(gpio_port port, gpio_pin pin){

    //Map pin to corrent ADC channel

    switch(port){

        case PORTA:

        switch(pin){

                case PIN0:

                    ADC_SQR3(ADC1) = 0x0;
                    break;

                case PIN1:

                    ADC_SQR3(ADC1) = 0x1;
                    break;

                case PIN2:

                    ADC_SQR3(ADC1) = 0x2;
                    break;

                case PIN3:

                    ADC_SQR3(ADC1) = 0x3;
                    break;

                case PIN4:

                    ADC_SQR3(ADC1) = 0x4;
                    break;

                case PIN5:

                    ADC_SQR3(ADC1) = 0x5;
                    break;

                case PIN6:

                    ADC_SQR3(ADC1) = 0x6;
                    break;

                case PIN7:

                    ADC_SQR3(ADC1) = 0x7;
                    break;

                default:

                    //All other pins have no Adc
                    return 0;
                    break;
        }
            break;

        case PORTB:

            switch(pin){

                case PIN0:

                    ADC_SQR3(ADC1) = 0x8;
                    break;

                case PIN1:

                    ADC_SQR3(ADC1) = 0x9;
                    break;

                default:

                    return 0;

                    break;

            }
            
            break;

        case PORTC:
            
            return 0;

            break;

        default:

            return 0;
            break;

    }

    //Set single conversion
    ADC_SQR1(ADC1) = 0;

    //Convert
    adc_start_conversion_regular(ADC1);

    //Wait for end of conversion
    while (!adc_eoc(ADC1));
    return  adc_read_regular(ADC1);
}


float ADC_volts(gpio_port port, gpio_pin pin){

    return ((float)ADC_sample(port,pin)/4096.0) * ADC_VOLT_REF;

}
