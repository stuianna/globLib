#include <stm32f76xxx_core.h>
#include <FreeRTOS.h>
#include <task.h>
#include <stdarg.h>

#define LED_PORT GPIOB
#define LED_PIN 7
#define LED_PERIOD 200

void task_ledBlink(void *pvParameters);

int main(void){

	GPIO_pinSetup(GPIO_DO,LED_PORT,LED_PIN);

	xTaskCreate(task_ledBlink,		     		// Function that implements the task.
				"LED Blink Green",          	  // Text name for the task.
				128,														// Stack size in words, not bytes.
				NULL,    												// Parameter passed into the task.
				1,															// Priority at which the task is created.
				NULL );													// Task Handle

	vTaskStartScheduler();

	// Should never get here.
	while(1);
}

void task_ledBlink(void *pvParameters){

	(void)pvParameters;

	  while(1){
			GPIO_pinToggle(LED_PORT,LED_PIN);
			vTaskDelay(pdMS_TO_TICKS(LED_PERIOD));
	  }
}
