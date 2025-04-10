
#include <stdio.h>

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "inc/hw_i2c.h"
#include "driverlib/i2c.h"
#include "inc/hw_ssi.h"
#include "driverlib/ssi.h"
#include "st7789v.h"

#include "i2c_wrapper.h"
#include "spi_wrapper.h"
#include "st7789_wrapper.h"
#include "uart_wrapper.h"
//*****************************************************************************
//
//! \addtogroup example_list
//! <h1>UART Echo (uart_echo)</h1>
//!
//! This example application utilizes the UART to echo text.  The first UART
//! (connected to the USB debug virtual serial port on the evaluation board)
//! will be configured in 115,200 baud, 8-n-1 mode.  All characters received on
//! the UART are transmitted back to the UART.
//
//*****************************************************************************


//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
}
#endif




void DelayMs(unsigned long ulCount)
{
    unsigned long freq=ROM_SysCtlClockGet();
    unsigned long mscount=freq/3000;
    ROM_SysCtlDelay(mscount*ulCount);
}


int main(void)
{
    //
    // Enable lazy stacking for interrupt handlers.  This allows floating-point
    // instructions to be used within interrupt handlers, but at the expense of
    // extra stack usage.
    //
    ROM_FPUEnable();
    ROM_FPULazyStackingEnable();

    //
    // Set the clocking to run directly from the crystal.
    //
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);

    //
    // Enable the GPIO port that is used for the on-board LED.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //
    // Enable the GPIO pins for the LED (PF2).  
    //
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);

    

    //
    // Enable processor interrupts.
    //
    ROM_IntMasterEnable();

    uart_init();

    printf("helloworld.\r\n");
    unsigned long freq=ROM_SysCtlClockGet();
    

    st7789_init();

    i2cDetect();

    st7789_Clear(0xf800);

    tp_init();


    //i2cdetect();
    //
    // Loop forever echoing data through the UART.
    //
    check_info();
    while(1)
    {
        //read_cap_sensor_info();
        
        //DelayMs(1000);
        //i2cTest();
        //st7789_Clear(ST7789_BLACK);
        //DelayMs(50);
        //st7789_Clear(ST7789_RED);
    }
    while(1)
    {

        printf("helloworld.\r\n");
        //SysCtlDelay(2000000);
        //printf("\r\n");
        //printf("freq:%ldHz\r\n\r\n",freq);
        printf("freq:%ldHz\r\n",freq);
        //printf("float:%f\r\n",test);
        //printf("\r\n");
        //printf("helloworld.\r\n");
        //printf("freq:%dHz\r\n\r\n",100);
        //ROM_SysCtlDelay(80000000);
        DelayMs(1000);
        st7789_Clear(ST7789_BLACK);
        DelayMs(1000);
        st7789_Clear(ST7789_WHITE);
        DelayMs(1000);
        st7789_Clear(ST7789_RED);
        DelayMs(1000);
        st7789_Clear(ST7789_GREEN);
        DelayMs(1000);
        st7789_Clear(ST7789_BLUE);
        DelayMs(1000);
        st7789_Clear(ST7789_YELLOW);
        DelayMs(1000);
        st7789_Clear(ST7789_CYAN);
        DelayMs(1000);
        st7789_Clear(ST7789_MAGENTA);

    }
}
