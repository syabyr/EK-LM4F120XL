#include <stdio.h>

#include "inc/lm4f120h5qr.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"


int main(void)
{
    volatile unsigned long ulLoop;

    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    ulLoop = SYSCTL_RCGC2_R;

    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 |GPIO_PIN_2 |GPIO_PIN_3);
    //
    // Loop forever.
    //
    while(1)
    {
        //
        // Turn on the LED.
        ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 |GPIO_PIN_2 |GPIO_PIN_3, GPIO_PIN_1 |GPIO_PIN_2 |GPIO_PIN_3);


        //
        // Delay for a bit.
        //
        for(ulLoop = 0; ulLoop < 200000; ulLoop++)
        {
        }

        //
        // Turn off the LED.
        //
        ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 |GPIO_PIN_2 |GPIO_PIN_3, 0x0);
        // Delay for a bit.
        //
        for(ulLoop = 0; ulLoop < 200000; ulLoop++)
        {
        }
    }
}
