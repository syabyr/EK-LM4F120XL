
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

int i2cdetect()
{
    uint8_t address;
    printf("     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\r\n");
    for (int i = 0; i < 128; i += 16) {
        printf("%02x: ", i);
        for (int j = 0; j < 16; j++) {
            address = i + j;
            I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, address, false);
            I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_SINGLE_SEND);
            while (I2CMasterBusy(I2C0_MASTER_BASE));
            if (I2CMasterErr(I2C0_MASTER_BASE) == I2C_MASTER_ERR_NONE) {
                printf("%02x ", address);
            } else {
                printf("-- ");
            }
            printf("\r\n");
        }
    }
    return 0;
}

int i2cTest()
{
    unsigned long ulDataRx[10];
    int nBytesCount;
    I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, 0x49, false);
    I2CMasterDataPut(I2C0_MASTER_BASE, 0x55);
    I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_SINGLE_SEND);
    printf("brefore busy wait\r\n");
    while (I2CMasterBusy(I2C0_MASTER_BASE));
    printf("after busy wait\r\n");
    I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, 0x46, true);
    I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
    for(nBytesCount = 0;nBytesCount<10;nBytesCount++)
    {
        while (I2CMasterBusy(I2C0_MASTER_BASE));
        ulDataRx[nBytesCount] = I2CMasterDataGet(I2C0_MASTER_BASE);
        printf("%ld\r\n", ulDataRx[nBytesCount]);
    }
    return 0;
}

void initI2C0(void)
{
	//This function is for eewiki and is to be updated to handle any port

	//enable I2C module
	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);

	//reset I2C module
	SysCtlPeripheralReset(SYSCTL_PERIPH_I2C0);

	//enable GPIO peripheral that contains I2C
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

	// Configure the pin muxing for I2C0 functions on port B2 and B3.
	GPIOPinConfigure(GPIO_PB2_I2C0SCL);
	GPIOPinConfigure(GPIO_PB3_I2C0SDA);

	// Select the I2C function for these pins.
	GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
	GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);
    //GPIODirModeSet(GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_3, GPIO_DIR_MODE_HW);
    //GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_3,
    //                 GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

	// Enable and initialize the I2C0 master module.  Use the system clock for
	// the I2C0 module.  The last parameter sets the I2C data transfer rate.
	// If false the data rate is set to 100kbps and if true the data rate will
	// be set to 400kbps.
	I2CMasterInitExpClk(I2C0_MASTER_BASE, SysCtlClockGet(), false);
    ROM_I2CMasterEnable(I2C0_MASTER_BASE);
	//clear I2C FIFOs
	//HWREG(I2C0_MASTER_BASE + I2C_O_FIFOCTL) = 80008000;
}


uint8_t readI2C0(uint16_t device_address, uint16_t device_register)
{
	//specify that we want to communicate to device address with an intended write to bus
	I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, device_address, false);

	//the register to be read
	I2CMasterDataPut(I2C0_MASTER_BASE, device_register);

	//send control byte and register address byte to slave device
	I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_SINGLE_SEND);

	//wait for MCU to complete send transaction
	while(I2CMasterBusy(I2C0_MASTER_BASE));

	//read from the specified slave device
	I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, device_address, true);

	//send control byte and read from the register from the MCU
	I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);

	//wait while checking for MCU to complete the transaction
	while(I2CMasterBusy(I2C0_MASTER_BASE));

	//Get the data from the MCU register and return to caller
	return( I2CMasterDataGet(I2C0_MASTER_BASE));
}

 void ReadBytes(uint16_t device_address,uint16_t device_register, uint8_t *pData, size_t size)
 {
        I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, device_address, false);
        I2CMasterDataPut(I2C0_MASTER_BASE, device_register);
        I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_SINGLE_SEND);
        while(I2CMasterBusy(I2C0_MASTER_BASE));
        I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, device_address, true);
        I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
        while(I2CMasterBusy(I2C0_MASTER_BASE));
        *pData++ = I2CMasterDataGet(I2C0_MASTER_BASE);
        for(size_t i = 1; i < size; i++)
        {
            I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT);
            while(I2CMasterBusy(I2C0_MASTER_BASE));
            *pData++ = I2CMasterDataGet(I2C0_MASTER_BASE);
        }
        I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
        while(I2CMasterBusy(I2C0_MASTER_BASE));
        *pData = I2CMasterDataGet(I2C0_MASTER_BASE);
 }


void i2cwritebyte(uint8_t device_address, uint8_t device_register, uint8_t device_data)
{
    I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, device_address, false);
    I2CMasterDataPut(I2C0_MASTER_BASE, device_register);
    I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    while(I2CMasterBusy(I2C0_MASTER_BASE));
    I2CMasterDataPut(I2C0_MASTER_BASE, device_data);
    I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
    while(I2CMasterBusy(I2C0_MASTER_BASE));
}


void writeI2C0(uint16_t device_address, uint16_t device_register, uint8_t device_data)
{
	//specify that we want to communicate to device address with an intended write to bus
	I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, device_address, false);

	//register to be read
	I2CMasterDataPut(I2C0_MASTER_BASE, device_register);

	//send control byte and register address byte to slave device
	I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_START);

	//wait for MCU to finish transaction
	while(I2CMasterBusy(I2C0_MASTER_BASE));

	I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, device_address, true);

	//specify data to be written to the above mentioned device_register
	I2CMasterDataPut(I2C0_MASTER_BASE, device_data);

	//wait while checking for MCU to complete the transaction
	I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);

	//wait for MCU & device to complete transaction
	while(I2CMasterBusy(I2C0_MASTER_BASE));
}


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
    //float test=124575567687655654.84454383838383838383;


    //gpiob init
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_3 | GPIO_PIN_2);
    printf("init pb ok.\r\n");
    
    /*
    while(true)
    {
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2|GPIO_PIN_3, 0);
        DelayMs(100);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2|GPIO_PIN_3, 0x0c);
        DelayMs(100);
    }
    */
    


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
                       SSI_MODE_MASTER, 8000000, 8);
    SSIEnable(SSI0_BASE);

    initI2C0();
    //while(1)
    {
        i2cwritebyte(0x46,0x20,0x00);
        uint8_t recv[10]={0xff};
        ReadBytes(0x46,0xA0,recv,10);
        int i=0;
        for(i=0;i<10;i++)
        {
            printf("%02x ",recv[i]);
        }
        printf("\r\n");

        for(i=0;i<14;i++)
        {
            uint8_t data = readI2C0(0x46,0xe0+i);
            printf("%02x ",data);
            DelayMs(10);
        }
        printf("\r\n");
        //uint8_t data = readI2C0(0x46,0xD0);
        //printf("%02x \r\n",data);
        
        DelayMs(1000);
    }
    

    // i2c init
    ///*
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
    ROM_SysCtlPeripheralReset(SYSCTL_PERIPH_I2C0);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    ROM_GPIOPinConfigure(GPIO_PB2_I2C0SCL);
    ROM_GPIOPinConfigure(GPIO_PB3_I2C0SDA);
    //GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_2, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    //GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    //GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
    //ROM_GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_3);
    GPIODirModeSet(GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_3, GPIO_DIR_MODE_HW);
    GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_3,
                     GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    ROM_I2CMasterInitExpClk(I2C0_MASTER_BASE, SysCtlClockGet(), false);
    ROM_I2CMasterEnable(I2C0_MASTER_BASE);
    //*/
   


    //TP_INT:PD7
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    ROM_GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_7);
    ROM_IntMasterEnable();
    ROM_IntEnable(INT_GPIOD);
    ROM_IntPrioritySet(INT_GPIOD, 0x00);

    //LCD_DC:PE2 LCD_BL:PE3
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_2|GPIO_PIN_3);
    ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2|GPIO_PIN_3, 0);

    //st7789_Init();
    new_init();
    st7789_Clear(0xf800);

    //i2cdetect();
    //
    // Loop forever echoing data through the UART.
    //
    while(1)
    {
        i2cTest();
        st7789_Clear(ST7789_BLACK);
        DelayMs(500);
        st7789_Clear(ST7789_RED);
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
