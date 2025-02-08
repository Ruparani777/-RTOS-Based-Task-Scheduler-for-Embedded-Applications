/* RTOS-Based Task Scheduler - FreeRTOS on Windows */

#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

// Task Handles
TaskHandle_t ledTaskHandle;
TaskHandle_t sensorTaskHandle;
TaskHandle_t loggerTaskHandle;

// Queue Handle for communication
QueueHandle_t sensorQueue;

// Simulated LED Task
void vLEDBlinkTask(void *pvParameters) {
    while (1) {
        printf("[LED Task] LED Blink Simulation\n");
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay 1 sec
    }
}

// Simulated Sensor Task
void vSensorTask(void *pvParameters) {
    int sensorData = 0;
    while (1) {
        sensorData = (rand() % 100); // Simulating sensor data
        printf("[Sensor Task] Sensor Value: %d\n", sensorData);
        xQueueSend(sensorQueue, &sensorData, portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(2000)); // Delay 2 sec
    }
}

// Logger Task
void vLoggerTask(void *pvParameters) {
    int receivedData;
    while (1) {
        if (xQueueReceive(sensorQueue, &receivedData, portMAX_DELAY)) {
            printf("[Logger Task] Logging Sensor Value: %d\n", receivedData);
        }
    }
}

// System Initialization
void systemInit() {
    // Create queue for sensor data
    sensorQueue = xQueueCreate(5, sizeof(int));
    
    // Create tasks
    xTaskCreate(vLEDBlinkTask, "LED Task", 1000, NULL, 1, &ledTaskHandle);
    xTaskCreate(vSensorTask, "Sensor Task", 1000, NULL, 2, &sensorTaskHandle);
    xTaskCreate(vLoggerTask, "Logger Task", 1000, NULL, 3, &loggerTaskHandle);
}

int main(void) {
    // Initialize system
    systemInit();
    
    // Start Scheduler
    vTaskStartScheduler();
    
    while (1);
    return 0;
}
