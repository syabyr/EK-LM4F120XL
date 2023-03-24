//*****************************************************************************
//
// uart_echo.c - Example for reading data from and writing data to the UART in
//               an interrupt driven fashion.
//
// Copyright (c) 2012 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 9453 of the EK-LM4F120XL Firmware Package.
//
//*****************************************************************************

#include <stdio.h>
#include <sys/stat.h>
#include "inc/lm4f120h5qr.h"
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

#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"
#include "task.h"
#include "timers.h"
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

//1000ms
#define SECOND 1000

// static timer & task
StaticTimer_t blinky_tmdef;
TimerHandle_t blinky_tm;



unsigned long SystemCoreClock;
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

//*****************************************************************************
//
// The UART interrupt handler.
//
//*****************************************************************************
void
UARTIntHandler(void)
{
    unsigned long ulStatus;

    //
    // Get the interrrupt status.
    //
    ulStatus = ROM_UARTIntStatus(UART0_BASE, true);

    //
    // Clear the asserted interrupts.
    //
    ROM_UARTIntClear(UART0_BASE, ulStatus);

    //
    // Loop while there are characters in the receive FIFO.
    //
    while(ROM_UARTCharsAvail(UART0_BASE))
    {
        //
        // Read the next character from the UART and write it back to the UART.
        //
        ROM_UARTCharPutNonBlocking(UART0_BASE,
                                   ROM_UARTCharGetNonBlocking(UART0_BASE));
        
        //
        // Blink the LED to show a character transfer is occuring.
        //
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
        
        //
        // Delay for 1 millisecond.  Each SysCtlDelay is about 3 clocks.
        //
        SysCtlDelay(SysCtlClockGet() / (1000 * 3));
        
        //
        // Turn off the LED
        //
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
        
    }
}

//*****************************************************************************
//
// Send a string to the UART.
//
//*****************************************************************************
void
UARTSend(const unsigned char *pucBuffer, unsigned long ulCount)
{
    //
    // Loop while there are more characters to send.
    //
    while(ulCount--)
    {
        //
        // Write the next character to the UART.
        //
        //ROM_UARTCharPutNonBlocking(UART0_BASE, *pucBuffer++);
        ROM_UARTCharPut(UART0_BASE, *pucBuffer++);
    }
}


void led_blinky_cb(TimerHandle_t xTimer)
{
  (void) xTimer;
  static int led_state = 0;
    UARTSend((unsigned char *)"tick tock. \r\n", 14);
    //printf("helloworld.\r\n");
    if(led_state)
    {
        ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 |GPIO_PIN_2 |GPIO_PIN_3, GPIO_PIN_1 |GPIO_PIN_2 |GPIO_PIN_3);
    }else
    {
        ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 |GPIO_PIN_2 |GPIO_PIN_3, 0x0);
    }
  led_state = 1 - led_state; // toggle
}


#ifdef __GNUC__
int _write(int fd, char *ptr, int len)
{
    (void) fd;
  UARTSend((unsigned char *)ptr, len);
  return len;
}
#endif

caddr_t _sbrk(int incr) 
{
    (void) incr;
    return (caddr_t) 0;
}
int _close(int file) 
{ 
    (void) file;
    return -1; 
}
int _fstat(int file, struct stat *st) 
{ 
    (void) file;
    (void) st;
    return 0;
}
int _isatty(int file) 
{ 
    (void) file;
    return 1; 
}
int _lseek(int file, int ptr, int dir) 
{ 
    (void) file;
    (void) ptr;
    (void) dir;
    return 0; 
}
int _read(int file, char *ptr, int len) 
{ 
    (void) file;
    (void) ptr;
    (void) len;
    return 0;
}

//*****************************************************************************
//
// This example demonstrates how to send a string of data to the UART.
//
//*****************************************************************************

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
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
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
    // Enable the peripherals used by this example.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Enable processor interrupts.
    //
    ROM_IntMasterEnable();

    //
    // Set GPIO A0 and A1 as UART pins.
    //
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Configure the UART for 115,200, 8-N-1 operation.
    //
    ROM_UARTConfigSetExpClk(UART0_BASE, ROM_SysCtlClockGet(), 115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));

    //
    // Enable the UART interrupt.
    //
    ROM_IntEnable(INT_UART0);
    ROM_UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);

    //
    // Prompt for text to be entered.
    //
    UARTSend((unsigned char *)"\033[2JEnter text: \r\n", 18);


    SystemCoreClock = ROM_SysCtlClockGet();


    blinky_tm = xTimerCreateStatic(NULL, pdMS_TO_TICKS(SECOND), true, NULL, led_blinky_cb, &blinky_tmdef);
    xTimerStart(blinky_tm, 0);
    vTaskStartScheduler();

    return 0;


    printf("helloworld.\r\n");
    unsigned long freq=ROM_SysCtlClockGet();
    //printf("freq:%ldHz\r\n",freq);
    //printf("freq:%sHz\r\n","hello");

    //
    // Loop forever echoing data through the UART.
    //
    while(1)
    {

        printf("helloworld.\r\n");
        //SysCtlDelay(2000000);
        //printf("\r\n");
        //printf("freq:%ldHz\r\n\r\n",freq);
        printf("freq:%ldHz\r\n",freq);
        //printf("\r\n");
        //printf("helloworld.\r\n");
        //printf("freq:%dHz\r\n\r\n",100);
        SysCtlDelay(2000000);
    }
}
