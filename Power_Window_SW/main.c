#include <stdint.h>
#include <stdbool.h>

#include "FreeRTOS.h"
#include "task.h"

#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_gpio.h"
#include "inc/tm4c123gh6pm.h"
#include "sysctlconfig.h"

#define MAX_BRIGHTNESS 10u

typedef enum Brightness
{
    OFF = 0u,
    ON = 9u,
} Brightness_t;

static uint8_t u8_LedBrigtness[MAX_BRIGHTNESS] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
static uint8_t u8_BIdx = 0;
static uint8_t BTicks = 0;
uint32_t u32_ReadVal = 0;

static Brightness_t eBrightness = OFF;
/* Both state is SW1 Released */
static uint8_t PrevSwState = 1u;
static uint8_t u8_CurrentState = 1u;
void Task_10ms(void *pvParameters)
{
    (void)pvParameters;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(10);

    for (;;)
    {
        u32_ReadVal = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);
        u8_CurrentState = (uint8_t)((u32_ReadVal >> 4u) & 0x01u);

        if ((PrevSwState == 1u) && (u8_CurrentState == 0u))
        {
            vTaskDelay(pdMS_TO_TICKS(20)); // Sw Debounce
            BTicks = (uint8_t)u8_LedBrigtness[((++u8_BIdx) % MAX_BRIGHTNESS)];
            eBrightness = (Brightness_t)BTicks;
        }

        PrevSwState = u8_CurrentState;

        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}

void Task_5ms(void *pvParameters)
{
    (void)pvParameters;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(5);
    /*Monitor the Switch1 */

    for (;;)
    {

        switch (eBrightness)
        {
        case ON:
        {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
            break;
        }
        case OFF:
        {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0u);
            break;
        }
        default:
        {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
            vTaskDelay(pdMS_TO_TICKS(BTicks));
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
            vTaskDelay(pdMS_TO_TICKS(MAX_BRIGHTNESS - BTicks));
            break;
        }
        }

        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}

/**
 * main.c
 */
int main(void)
{
    // Configure system clock to 80 MHz using PLL
   //SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    SysCtlClockConfig(MOSC,1u,80u);

    // Enable GPIO Port F Peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));

    // Configure LED pins (PF1 = Red, PF2 = Blue, PF3 = Green) as Outputs
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

    // Configure SW1 (PF4) as Input with Internal Pull-Up Resistor
    GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_DIR_MODE_IN);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    // Create FreeRTOS Tasks with adequate stack depth (256 words = 1 KB)
    xTaskCreate(Task_5ms, "SwitchTask", 256, NULL, 3, NULL); // Higher priority
    xTaskCreate(Task_10ms, "LEDTask", 256, NULL, 2, NULL);

    // Start FreeRTOS Scheduler
    vTaskStartScheduler();

    while (1)
    {
        // Safety loop: should never be reached
    }
}
