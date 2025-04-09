
#include <stdio.h>
#include <stdint.h>

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
#include "driverlib/ssi.h"

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
        ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
        
        //
        // Delay for 1 millisecond.  Each SysCtlDelay is about 3 clocks.
        //
        ROM_SysCtlDelay(SysCtlClockGet() / (1000 * 3));
        
        //
        // Turn off the LED
        //
        ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
        
    }
}



void DelayMs(unsigned long ulCount)
{
    unsigned long freq=ROM_SysCtlClockGet();
    unsigned long mscount=freq/3000;
    ROM_SysCtlDelay(mscount*ulCount);
}

uint8_t ReadWhileWrite(uint8_t data)
{
    uint8_t data2;
    
    ROM_SSIDataPut(SSI0_BASE, data);
    while(SSIBusy(SSI0_BASE))
    {
    }
    ROM_SSIDataGet(SSI0_BASE, &data2);
    
    return data2;
}

void PS2_ShortPoll(void)
{
    ROM_GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, 0);
    DelayMs(1);
    ReadWhileWrite(0x01);
    ReadWhileWrite(0x42);
    ReadWhileWrite(0x00);
    ReadWhileWrite(0x00);
    ReadWhileWrite(0x00);
    ReadWhileWrite(0x00);
    ReadWhileWrite(0x00);
    ReadWhileWrite(0x00);
    ReadWhileWrite(0x00);
    ROM_GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, GPIO_PIN_2);
    DelayMs(1);
}

void PS2_EnterConfig(void)
{
    ROM_GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, 0);
    DelayMs(1);
    ReadWhileWrite(0x01);
    ReadWhileWrite(0x43);
    ReadWhileWrite(0x00);
    ReadWhileWrite(0x01);
    ReadWhileWrite(0x00);
    ReadWhileWrite(0x00);
    ReadWhileWrite(0x00);
    ReadWhileWrite(0x00);
    ReadWhileWrite(0x00);
    ROM_GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, GPIO_PIN_2);
    DelayMs(1);
}

void PS2_TurnOnAnalogMode(void)
{
    ROM_GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, 0);
    DelayMs(1);
    ReadWhileWrite(0x01);
    ReadWhileWrite(0x44);
    ReadWhileWrite(0x00);
    ReadWhileWrite(0x01);
    ReadWhileWrite(0x03);
    ReadWhileWrite(0x00);
    ReadWhileWrite(0x00);
    ReadWhileWrite(0x00);
    ReadWhileWrite(0x00);
    ROM_GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, GPIO_PIN_2);
    DelayMs(1);
}

void PS2_VibrationMode(void)
{
    ROM_GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, 0);
    DelayMs(1);
    ReadWhileWrite(0x01);
    ReadWhileWrite(0x4D);
    ReadWhileWrite(0x00);
    ReadWhileWrite(0x00);
    ReadWhileWrite(0x01);
    ReadWhileWrite(0x00);
    ReadWhileWrite(0x00);
    ReadWhileWrite(0x00);
    ReadWhileWrite(0x00);
    ROM_GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, GPIO_PIN_2);
    DelayMs(1);
}

void PS2_ExitConfig(void)
{
    ROM_GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, 0);
    DelayMs(1);
    ReadWhileWrite(0x01);
    ReadWhileWrite(0x43);
    ReadWhileWrite(0x00);
    ReadWhileWrite(0x00);
    ReadWhileWrite(0x5A);
    ReadWhileWrite(0x5A);
    ReadWhileWrite(0x5A);
    ReadWhileWrite(0x5A);
    ReadWhileWrite(0x5A);
    ROM_GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, GPIO_PIN_2);
    DelayMs(1);
}

void PS2_SetInit(void)
{
    PS2_ShortPoll();
    PS2_ShortPoll();
    PS2_ShortPoll();
    PS2_EnterConfig();
    PS2_TurnOnAnalogMode();
    PS2_VibrationMode();
    PS2_ExitConfig();
    DelayMs(1);
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
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
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


    printf("helloworld.\r\n");
    unsigned long freq=ROM_SysCtlClockGet();
    //printf("freq:%ldHz\r\n",freq);
    //printf("freq:%sHz\r\n","hello");
    float test=124575567687655654.84454383838383838383;

    //cs init
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_2);

    // spi init
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinConfigure(GPIO_PA2_SSI0CLK);
    GPIOPinConfigure(GPIO_PA3_SSI0FSS);
    GPIOPinConfigure(GPIO_PA4_SSI0RX);
    GPIOPinConfigure(GPIO_PA5_SSI0TX);
    GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3 |
                   GPIO_PIN_2);
    SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_3,
                       SSI_MODE_MASTER, 1000000, 8);
    SSIEnable(SSI0_BASE);
    #define NUM_SSI_DATA 8
    unsigned long ulindex;
    unsigned long ulDataTx[NUM_SSI_DATA];
    unsigned long ulDataRx[NUM_SSI_DATA];

    PS2_SetInit();
    //
    // Loop forever echoing data through the UART.
    //
    while(1)
    {

        printf("start1.\r\n");
        ROM_GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, 0);
        while(SSIDataGetNonBlocking(SSI0_BASE, &ulDataRx[0]))
        {
        }
        printf("start2.\r\n");
        int i;


        //////////////////
        DelayMs(1);
        ROM_SSIDataPut(SSI0_BASE, 0x01);
        while(SSIBusy(SSI0_BASE))
        {
        }
        ROM_SSIDataGet(SSI0_BASE, &ulDataRx[0]);
        ulDataRx[0] &= 0x00FF;
        printf("0x%02x \r\n",ulDataRx[0]);


        ROM_SSIDataPut(SSI0_BASE, 0x42);

        while(SSIBusy(SSI0_BASE))
        {
        }
        ROM_SSIDataGet(SSI0_BASE, &ulDataRx[0]);
        ulDataRx[0] &= 0x00FF;
        printf("0x%02x \r\n",ulDataRx[0]);


        //read data
        //int i;
        for(i = 0; i < 7; i++)
        {
            ROM_SSIDataPut(SSI0_BASE, 0x0);
            ROM_SSIDataGet(SSI0_BASE, &ulDataRx[0]);
            ulDataRx[0] &= 0x00FF;
            printf("0x%02x ",ulDataRx[0]);
        }
        while(SSIBusy(SSI0_BASE))
        {
        }
        printf("\r\n");
        

        ROM_GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, GPIO_PIN_2);
        //SysCtlDelay(2000000);
        //printf("\r\n");
        //printf("freq:%ldHz\r\n\r\n",freq);
        printf("freq:%ldHz\r\n",freq);
        //printf("float:%f\r\n",test);
        //printf("\r\n");
        //printf("helloworld.\r\n");
        //printf("freq:%dHz\r\n\r\n",100);
        //ROM_SysCtlDelay(80000000);
        DelayMs(500);
    }
}
