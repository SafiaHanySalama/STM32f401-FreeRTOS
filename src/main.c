#include "FreeRTOS.h"
#include "task.h"
#include "stm32f4xx_hal.h"
#define LED1_PIN 0
#define LED2_PIN 1

TaskHandle_t xTask2Handle = NULL;
TaskHandle_t xTask1Handle = NULL;

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
    /* If the buffers to be provided to the Idle task are declared inside this
    function then they must be declared static - otherwise they will be allocated on
    the stack and so not exists after this function exits. */
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
    state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

void vTask1Function( void *pvParameters )
{
    for( ;; )
    {
        // Toggle LED1
        HAL_GPIO_TogglePin(GPIOA, LED1_PIN);
        vTaskDelay(pdMS_TO_TICKS(500)); // Delay for 500ms

         // Resume Task 1
        vTaskResume(xTask2Handle);
    }
}

void vTask2Function( void *pvParameters )
{
    for( ;; )
    {
        // Toggle LED2
        HAL_GPIO_TogglePin(GPIOA, LED2_PIN);
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1000ms

        // Suspend Task 2
        if (xTask1Handle != NULL) {
            vTaskSuspend(xTask1Handle);
        }
    }
}


int main( void )
{
    /**
     * Creates a new task with the given parameters.
     *
     * @param pvTaskCode Pointer to the task entry function.
     * @param pcName A descriptive name for the task.
     * @param usStackDepth The size of the task's stack in words.
     * @param pvParameters Pointer to any parameters that the task entry function may require.
     * @param uxPriority The priority at which the task should run.
     * @param pxCreatedTask Pointer to a variable that will receive the task's handle.
     *
     * @return pdPASS if the task was successfully created, otherwise errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY.
     */
    xTaskCreate( vTask1Function, "Task 1", 1000, NULL, 2, &xTask1Handle );
    xTaskCreate( vTask2Function, "Task 2", 1000, NULL, 1, &xTask2Handle );

    vTaskStartScheduler();

    for( ;; );
    return 0;
}
