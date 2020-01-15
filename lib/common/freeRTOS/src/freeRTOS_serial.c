#include "freeRTOS_serial.h"

TaskHandle_t TXTaskHandle;
QueueHandle_t TXQueue;
SemaphoreHandle_t TXMutex;

static void freeRTOS_serial_TXTask(void *pvParameter);

extern void freeRTOS_serial_initiateTX(uint8_t* buffer, uint16_t length);
extern uint8_t freeRTOS_serial_hardwareSetup();

void freeRTOS_serial_IRQ(void){
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	vTaskNotifyGiveFromISR(TXTaskHandle,&xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

void _putchar(char b){

	// Triggers if data cannot be added to the queue.
	// This could occur if the DMA buffer isn't emptying fast enough or interrupt isn't triggering.
	// The byte through port should be checked here. E.g @ 115000bps = 11500 bytes/s (baud / 10)
	// In reality the byte throughport can be 80% of the maximum.
	configASSERT(xQueueSendToBack(TXQueue,&b,pdMS_TO_TICKS(TX_QUEUE_FULL_BLOCK_MS)) == pdPASS);
}

void print(const char *format, ...){

	// Triggers if unable to take semaphore, another task is hogging the TX print function.
	// This can happen if the other task is waiting for the queue to be empty
	// The byte through port should be checked here. E.g @ 115000bps = 11500 bytes/s (baud / 10)
	// In reality the byte throughport can be 80% of the maximum.
	configASSERT(xSemaphoreTakeRecursive(TXMutex,pdMS_TO_TICKS(TX_MUTEX_BLOCK_MS)) == pdTRUE);

	va_list args;
	va_start(args,format);
	vprintf(format,args);
	va_end(args);
	xSemaphoreGiveRecursive(TXMutex);
}

BaseType_t freeRTOS_serial_setup(UBaseType_t TXpriority, UBaseType_t TXQueueLength){

	BaseType_t taskCreationStatus = pdFAIL;

	if(freeRTOS_serial_hardwareSetup() != pdPASS){
		return pdFAIL;
	}

	taskCreationStatus = xTaskCreate(freeRTOS_serial_TXTask,
			"Serial TX Task",
			TX_TASK_STACK_SIZE,
			NULL,
			TXpriority,
			&TXTaskHandle);
	if(taskCreationStatus != pdPASS){
		return taskCreationStatus;
	}

	TXQueue = xQueueCreate(TXQueueLength,sizeof(uint8_t));
	if(TXQueue == NULL){
		return pdFAIL;
	}

	TXMutex = xSemaphoreCreateMutex();
	if(TXMutex == NULL){
		return pdFAIL;
	}

	// Give a notification so DMA can be started the first time.
	xTaskNotifyGive(TXTaskHandle);

	return pdPASS;

}

void freeRTOS_serial_TXTask(void *pvParameter){

	(void)pvParameter;
	uint8_t buffPos = 0;
	uint8_t buffer[TX_DMA_BUFFER_SIZE];
	uint8_t secondary = 0;

	while(1){

		// Check if the input stream queue is empty.
		while((buffPos < TX_DMA_HALF_BUFFER) && (xQueueReceive(TXQueue,&buffer[buffPos + (TX_DMA_HALF_BUFFER * secondary)],pdMS_TO_TICKS(TX_QUEUE_EMPTY_BLOCK_MS)) == pdPASS)){
			buffPos++;
		}

		// Make sure the DMA has finished flushing the buffer.
		if(ulTaskNotifyTake(pdTRUE,pdMS_TO_TICKS(TX_DMA_EMPTY_BLOCK_MS)) == 0){
			continue;
		}

		if(buffPos){
			freeRTOS_serial_initiateTX(&buffer[TX_DMA_HALF_BUFFER * secondary],buffPos);
			buffPos = 0;
			secondary = !secondary & 0x1;;
		}
	}
}
