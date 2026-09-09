#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * CPU / Tick configuration
 *----------------------------------------------------------*/

#define configCPU_CLOCK_HZ                 ( 80000000UL )
#define configTICK_RATE_HZ                 ( 1000UL )

/*-----------------------------------------------------------
 * Scheduler
 *----------------------------------------------------------*/

#define configUSE_PREEMPTION               1
#define configUSE_TIME_SLICING             1
#define configMAX_PRIORITIES               5

/*-----------------------------------------------------------
 * Task configuration
 *----------------------------------------------------------*/

#define configMINIMAL_STACK_SIZE           128

/*-----------------------------------------------------------
 * Dynamic memory
 *----------------------------------------------------------*/

#define configSUPPORT_DYNAMIC_ALLOCATION   1
#define configSUPPORT_STATIC_ALLOCATION    0

#define configTOTAL_HEAP_SIZE              ( 10 * 1024 )

/*-----------------------------------------------------------
 * Tick type
 *----------------------------------------------------------*/

#define configTICK_TYPE_WIDTH_IN_BITS      TICK_TYPE_WIDTH_32_BITS

/*-----------------------------------------------------------
 * Synchronization
 *----------------------------------------------------------*/

#define configUSE_MUTEXES                  1
#define configUSE_COUNTING_SEMAPHORES      1

/*-----------------------------------------------------------
 * Software timers
 *----------------------------------------------------------*/

#define configUSE_TIMERS                   0

/*-----------------------------------------------------------
 * Hooks
 *----------------------------------------------------------*/

#define configUSE_IDLE_HOOK                0
#define configUSE_TICK_HOOK                0

/*-----------------------------------------------------------
 * Interrupt priorities
 *----------------------------------------------------------*/

#define configPRIO_BITS                    3

#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY    7

#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 5

#define configKERNEL_INTERRUPT_PRIORITY \
    ( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << ( 8 - configPRIO_BITS ) )

#define configMAX_SYSCALL_INTERRUPT_PRIORITY \
    ( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << ( 8 - configPRIO_BITS ) )

#endif


#define INCLUDE_vTaskPrioritySet                  1
#define INCLUDE_uxTaskPriorityGet                 1
#define INCLUDE_vTaskDelete                       1
#define INCLUDE_vTaskCleanUpResources             0
#define INCLUDE_vTaskSuspend                      1
#define INCLUDE_vTaskDelayUntil                   1
#define INCLUDE_vTaskDelay                        1
#define INCLUDE_uxTaskGetStackHighWaterMark       1
#define INCLUDE_xTaskGetSchedulerState            1
#define INCLUDE_xTimerGetTimerDaemonTaskHandle    1
#define INCLUDE_xTaskGetIdleTaskHandle            1
#define INCLUDE_xTaskGetHandle                    1
#define INCLUDE_eTaskGetState                     1
#define INCLUDE_xSemaphoreGetMutexHolder          1
#define INCLUDE_xTimerPendFunctionCall            1
#define INCLUDE_xTaskAbortDelay                   1
