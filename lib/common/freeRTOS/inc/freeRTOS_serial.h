#ifndef _FREERTOS_SERIAL_H
#define _FREERTOS_SERIAL_H

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>
#include <stdarg.h>
#include "printf.h"

#ifndef TX_TASK_STACK_SIZE
#define TX_TASK_STACK_SIZE 128
#endif

#ifndef TX_DMA_BUFFER_SIZE
#define TX_DMA_BUFFER_SIZE 32
#endif

#define TX_DMA_HALF_BUFFER TX_DMA_BUFFER_SIZE >> 1
#define TX_DMA_EMPTY_BLOCK_MS 200
#define TX_QUEUE_EMPTY_BLOCK_MS 200
#define TX_QUEUE_FULL_BLOCK_MS 200
#define TX_MUTEX_BLOCK_MS 1000

#ifdef __cplusplus
extern "C" {
#endif

BaseType_t freeRTOS_serial_setup(UBaseType_t TXPriority, UBaseType_t TXQueueLength);
void print(const char *format, ...);
void freeRTOS_serial_IRQ(void);


#ifdef __cplusplus
}
#endif
#endif
