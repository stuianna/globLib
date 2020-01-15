#include <stm32f76xxx_core.h>
#include <FreeRTOS.h>
#include <task.h>

#define LED_PORT_GREEN   GPIOB
#define LED_PIN_GREEN    7
#define LED_PORT    		GPIOB
#define LED_PIN     		0

void delayMs(uint16_t ms);
void task_ledBlinkGreen(void *pvParameters);
void task_ledBlinkRed(void *pvParameters);

int main(void){

	GPIO_pinSetup(GPIO_DO,LED_PORT_GREEN,LED_PIN_GREEN);
	GPIO_pinSetup(GPIO_DO,LED_PORT,LED_PIN);

	 xTaskCreate(
				task_ledBlinkRed,    					// Function that implements the task.
				"LED Blink",          				// Text name for the task.
				configMINIMAL_STACK_SIZE,			// Stack size in words, not bytes.
				NULL,    											// Parameter passed into the task.
				tskIDLE_PRIORITY+1,						// Priority at which the task is created.
				NULL );												// Task Handle

	 xTaskCreate(task_ledBlinkGreen,     // Function that implements the task.
				"LED Blink Green",          	  // Text name for the task.
				configMINIMAL_STACK_SIZE,			// Stack size in words, not bytes.
				NULL,    											// Parameter passed into the task.
				tskIDLE_PRIORITY+1,						// Priority at which the task is created.
				NULL );												// Task Handle

	vTaskStartScheduler();

	// Should never get here.
	while(1);

}


void task_ledBlinkGreen(void *pvParameters){

	(void)pvParameters;

	  while(1){
			GPIO_pinToggle(LED_PORT_GREEN,LED_PIN_GREEN);
			vTaskDelay( pdMS_TO_TICKS( 500) );
	  }
}

void task_ledBlinkRed(void *pvParameters){

	(void)pvParameters;

	  while(1){
			GPIO_pinToggle(LED_PORT,LED_PIN);
			vTaskDelay( pdMS_TO_TICKS( 200) );
	  }
}
