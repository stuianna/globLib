#ifndef _FREERTOS_SERIAL_H
#define _FREERTOS_SERIAL_H

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>
#include <stdarg.h>
#include "printf.h"

/**
  @addtogroup Common
  @{
*/

/*!
    @addtogroup freeRTOS_serial
    @brief DMA utilising USART serial transmit and receive tasks.
		@details Only TX is supported for now.

    @author Stuart Ianna
    @date January 2020
    @warning If used, this module requires that external functions are declared.
		@todo
			- The optimal default size for the TX stack should be determined.
			- RX should be implemented
			- An event group could be implemented for the TX status (lost packets, usage)

    @par Example external function requirements
  @{
*/

/*!
    @brief The default size of the TX task stack
*/
#ifndef TX_TASK_STACK_SIZE
#define TX_TASK_STACK_SIZE 128
#endif
/*!
    @brief The default size of the TX DMA buffer
*/
#ifndef TX_DMA_BUFFER_SIZE
#define TX_DMA_BUFFER_SIZE 32
#endif
/*!
    @brief Macro calculation for half DMA buffer.
*/
#define TX_DMA_HALF_BUFFER TX_DMA_BUFFER_SIZE >> 1
/*!
    @brief Macro calculation for half DMA buffer.
*/
#define TX_DMA_EMPTY_BLOCK_MS 200
/*!
    @brief How long (in ms) the TX task is blocked waiting for an item to appear on the queue.
*/
#define TX_QUEUE_EMPTY_BLOCK_MS 200
/*!
    @brief How long (in ms) print output function should wait for space to be on the queue.
*/
#define TX_QUEUE_FULL_BLOCK_MS 200
/*!
    @brief How long a task calling the print function should wait for another task to return the mutex.
*/
#define TX_MUTEX_BLOCK_MS 1000

#ifdef __cplusplus
extern "C" {
#endif

/*!
    @brief Setup the modules required components. To be called before the scheduler is started.
		@details
		@param TXPriority The priority of the TX task function.
		@param TXQueueLength The length of the queue to which the print function outputs formatted characters.
    @return pdPASS if creation was sucessfull or pdFAIL if an error occured, possibly due to insufficient
		heap space.
*/
BaseType_t freeRTOS_serial_setup(UBaseType_t TXPriority, UBaseType_t TXQueueLength);
/*!
    @brief Format a string and add it to the TX queue.
		@details This function takes the same arguments as printf. The calling task becomes the Mutex holder
		for the TX task.
		@param format Formatted printf style string
		@param ... Variable argument list
    @return None
*/
void print(const char *format, ...);
/*!
    @brief IRQ function, to be placed in DMA transfer complete (or equivalent) IRQ handler.
		@details This function sends a notification to the TX task so the DMA buffer can be reloaded.
    @return None
*/
void freeRTOS_serial_IRQ(void);


#ifdef __cplusplus
}
#endif

/**@}*/
/**@}*/
#endif
