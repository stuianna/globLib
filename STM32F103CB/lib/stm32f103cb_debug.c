/*!**************************************************************************
    @file stm32f103cb_debug.c
    @brief Source file for stm32f103cb DEBUG 
    @author Stuart Ianna
    @version 0.1
    @date May 2018
    @copyright GNU GPLv3
    @warning None
    @bug 

    @details


    @par Compilers
    - arm-none-eabi-gcc (15:4.9.3+svn231177-1) 4.9.3 20150529 (prerelease)
******************************************************************************/

#include "stm32f103cb_debug.h"

//Debug function pointer, this should automatically be assigned to NULL, as it is file-static
static void (*debug_out)(uint8_t byte);

static const struct error_desc{

    uint8_t code;
    char *message;
}errordesc[] = {

    { E_MCU_UNDEFINED,      "Error MCU: Unknown error code\n\r"                 },

    { E_GPIO_NOERROR,       "Error GPIO: No Error\n\r"                          },
    { E_GPIO_PORT,          "Error GPIO: Port doesn't exist\n\r"                },
    { E_GPIO_PIN,           "Error GPIO: Pin doesnt' exist\n\r"                 },
    { E_GPIO_ISR,           "Error GPIO: ISR EXTI line in use\n\r"              },
    { E_GPIO_TRIGGER,       "Error GPIO: ISR trigger doesn't exist\n\r"         },

    { E_USART_NOERROR,      "Error USART: No Error\n\r"                         },
    { E_USART_NOPORT,       "Error USART: Port doesn't exist\n\r"               },
    { E_USART_NOBAUD,       "Error USART: Baud rate not available\n\r"          },
    { E_USART_NOSTOP,       "Error USART: Stop bits don't exist\n\r"            },
    { E_USART_NODATA,       "Error USART: Data frame not available\n\r"         },
    { E_USART_NOPARITY,     "Error USART: Parity option not available\n\r"      },
    { E_USART_NOINT,        "Error USART: Interrupt option doesn't exist\n\r"   },

    { E_I2C_NOERROR,        "Error I2C: No error\n\r"                           },
    { E_I2C_PORT,           "Error I2C: Port doesn't exist\n\r"                 },
    { E_I2C_WRITE,          "Error I2C: Write transmission timeout\n\r"         },
    { E_I2C_READ,           "Error I2C: Read transmission timeout\n\r"          },
    { E_I2C_START,          "Error I2C: Start transmission timeout\n\r"         },
    { E_I2C_STOP,           "Error I2C: Stop transmission timeout\n\r"          },

    { E_CLOCK_NOERROR,      "Error CLOCK: No error\n\r"                         },
    { E_CLOCK_NOSPEED,      "Error CLOCK: Clock speed doesn't exist\n\r"        },

    { E_SYSTICK_NOERROR,    "Error SYSTICK: No error\n\r"                       },
    { E_SYSTICK_TOOLONG,    "Error SYSTICK: Timeour value is too long\n\r"      },
    
    { E_TIMER_NOERROR,      "Error TIMER: No error\n\r"                         },
    { E_TIMER_NOTIMER,      "Error TIMER: Timer doesn't exist\n\r"              },
    { E_TIMER_NOCHANNEL,    "Error TIMER: Channel doesn't exist\n\r"            },
    { E_TIMER_PERIOD,       "Error TIMER: Period long/frequency high\n\r"       },
    { E_TIMER_PULSE,        "Error TIMER: Pulse too long\n\r"                   },

    { E_ADC_NOERROR,        "Error ADC: No error\n\r"                           },
    { E_ADC_PORT,           "Error ADC: Port doesn't exist with ADC\n\r"        },
    { E_ADC_PIN,            "Error ADC: No ADC on pin\n\r"                      },

    { E_IWDG_NOERROR,       "Error IWDG: No error\n\r"                          },
    { E_IWDG_PERIOD,        "Error IWDG: Period too long\n\r"                   },

    { E_FLASH_NOERROR,      "Error FLASH: No error\n\r"                         },
    { E_FLASH_PAGE,         "Error FLASH: Page number out of bounds\n\r"        },

    { E_RTC_NOERROR,        "Error RTC: No Error \n\r"                          },
    { E_RTC_PRELOAD,        "Error RTC: Invalid preload value \n\r"             },

    { E_SPI_NOERROR,        "Error SPI: No Error \n\r"                          },
    { E_SPI_PORT,           "Error SPI: Port doesn't exist \n\r"                }
};


void MCU_debugEnable(void (*output_handle)(uint8_t)){

    //Set that static function pointer to the output function, so error messages have a place to go
	debug_out = output_handle;
}

void MCU_debugDisable(void){

    //Set the function pointer to null, so no errors can be outputted
    //This also means error enable can be tested using the NULL value
	debug_out = NULL;
}
uint8_t MCU_debugIsEnabled(void){

    if(debug_out == NULL){

        return 0;

    }

    return 1;
}
//Print the error character by character to the error_out function pointer
void MCU_printError(mcu_error errorNum){

    if(debug_out == NULL){

        return;
    }

    uint8_t index;
    uint8_t pos = 0;

    //Find the position of the error code in errodesc
    for(index = 0; index < (sizeof(errordesc) / sizeof(errordesc[0])); index++){

        if(errorNum == errordesc[index].code){

            break;
        }
    }

    //Set index to default if unknown error if can't be found
    if(index == (sizeof(errordesc) / sizeof(errordesc[0]))){

        index = 0;
    }

    //Print the error using the message value
   while(errordesc[index].message[pos] != '\0'){

       debug_out(errordesc[index].message[pos]);
       pos++;
   }
}

