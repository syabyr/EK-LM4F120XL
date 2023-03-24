#include "inc/lm4f120h5qr.h"
#include <stdio.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"
#include "task.h"
#include "timers.h"


/* Blink pattern
 * - 250 ms  : device not mounted
 * - 1000 ms : device mounted
 * - 2500 ms : device is suspended
 */
enum  {
  BLINK_NOT_MOUNTED = 250,
  BLINK_MOUNTED = 1000,
  BLINK_SUSPENDED = 2500,
};


// static timer & task
StaticTimer_t blinky_tmdef;
TimerHandle_t blinky_tm;


void led_blinky_cb(TimerHandle_t xTimer)
{
  (void) xTimer;
  static int led_state = 0;

    if(led_state)
    {
        ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 |GPIO_PIN_2 |GPIO_PIN_3, GPIO_PIN_1 |GPIO_PIN_2 |GPIO_PIN_3);
    }else
    {
        ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 |GPIO_PIN_2 |GPIO_PIN_3, 0x0);
    }
  led_state = 1 - led_state; // toggle
}

unsigned long SystemCoreClock;

int main(void)
{
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 |GPIO_PIN_2 |GPIO_PIN_3);

    SystemCoreClock = ROM_SysCtlClockGet();

    blinky_tm = xTimerCreateStatic(NULL, pdMS_TO_TICKS(BLINK_NOT_MOUNTED), true, NULL, led_blinky_cb, &blinky_tmdef);
    xTimerStart(blinky_tm, 0);
    vTaskStartScheduler();

    return 0;
}
