//*****************************************************************************
//
// touch.c - IT7259 Touch Driver with I2C Scanner
//
// Pinout:
// TP_SCL: PB2 (I2C0 SCL)
// TP_SDA: PB3 (I2C0 SDA)
// TP_INT: PD7 (Interrupt Input)
// UART: PA0 (RX), PA1 (TX) - 115200 8-N-1
//
//*****************************************************************************

#include <stdio.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_i2c.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/i2c.h"
#include "../drivers/retargetserial.h"

//*****************************************************************************
//
// Error handler
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
// Delay function (milliseconds)
//
//*****************************************************************************
void
DelayMs(unsigned long ulCount)
{
    unsigned long ulFreq = ROM_SysCtlClockGet();
    unsigned long ulMsCount = ulFreq / 3000;
    ROM_SysCtlDelay(ulMsCount * ulCount);
}

//*****************************************************************************
//
// I2C Initialize (exactly same as st7789 implementation)
//
//*****************************************************************************
void
i2c_init(void)
{
    // Enable I2C module
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);

    // Reset I2C module
    SysCtlPeripheralReset(SYSCTL_PERIPH_I2C0);

    // Enable GPIO peripheral that contains I2C
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    // Configure the pin muxing for I2C0 functions on port B2 and B3
    GPIOPinConfigure(GPIO_PB2_I2C0SCL);
    GPIOPinConfigure(GPIO_PB3_I2C0SDA);

    // Select the I2C function for these pins
    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);

    // Enable and initialize the I2C0 master module
    I2CMasterInitExpClk(I2C0_MASTER_BASE, SysCtlClockGet(), false);

    // Enable I2C master
    ROM_I2CMasterEnable(I2C0_MASTER_BASE);
}

//*****************************************************************************
//
// I2C Write Byte (exactly same as st7789 implementation)
//
//*****************************************************************************
void
i2cWriteByte(uint8_t device_address, uint8_t device_register, uint8_t device_data)
{
    ROM_I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, device_address, false);
    ROM_I2CMasterDataPut(I2C0_MASTER_BASE, device_register);
    ROM_I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    while(ROM_I2CMasterBusy(I2C0_MASTER_BASE));

    ROM_I2CMasterDataPut(I2C0_MASTER_BASE, device_data);
    ROM_I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
    while(ROM_I2CMasterBusy(I2C0_MASTER_BASE));
}

//*****************************************************************************
//
// I2C Read Byte (exactly same as st7789 implementation)
//
//*****************************************************************************
uint8_t
i2cReadByte(uint16_t device_address, uint16_t device_register)
{
    ROM_I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, device_address, false);
    ROM_I2CMasterDataPut(I2C0_MASTER_BASE, device_register);
    ROM_I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_SINGLE_SEND);
    while(ROM_I2CMasterBusy(I2C0_MASTER_BASE));

    ROM_I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, device_address, true);
    ROM_I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
    while(ROM_I2CMasterBusy(I2C0_MASTER_BASE));

    return(ROM_I2CMasterDataGet(I2C0_MASTER_BASE));
}

//*****************************************************************************
//
// I2C Detect (exactly same as st7789 implementation)
//
//*****************************************************************************
void
i2cDetect()
{
    printf("\r\n");
    printf("=========================================\r\n");
    printf("          I2C Bus Scan Result            \r\n");
    printf("=========================================\r\n");
    printf(" 7  6  5  4  3  2  1  0  |  Addr Range  \r\n");
    printf("--------------------------|---------------\r\n");

    int iFound = 0;

    for (int base = 0; base < 128; base += 8) {
        printf(" ");
        for (int offset = 0; offset < 8; offset++) {
            int address = base + offset;
            if (address == 0 || address >= 127) {
                printf(" --");
                continue;
            }

            ROM_I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, address, true);
            ROM_I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_SINGLE_SEND);
            while (ROM_I2CMasterBusy(I2C0_MASTER_BASE));

            if (ROM_I2CMasterErr(I2C0_MASTER_BASE) == I2C_MASTER_ERR_NONE) {
                printf(" %02X", address);
                iFound++;
            } else {
                printf(" --");
            }
        }
        printf("  |  0x%02X - 0x%02X\r\n", base, base + 7);
    }

    printf("=========================================\r\n");
    if(iFound > 0)
    {
        printf("Found %d I2C device(s) on the bus\r\n", iFound);
    }
    else
    {
        printf("No I2C devices found on the bus\r\n");
    }
    printf("=========================================\r\n");
    printf("\r\n");
}

//*****************************************************************************
//
// TP_INT Initialize (PD7)
//
//*****************************************************************************
void
InitTP_INT(void)
{
    // Enable GPIOD peripheral
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

    // Configure PD7 as input with pull-up
    ROM_GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_7);
    ROM_GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_7,
                         GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}

//*****************************************************************************
//
// IT7259 Read Touch Data
//
//*****************************************************************************
void
IT7259_ReadTouch(uint8_t devAddr, uint16_t *pX, uint16_t *pY, uint8_t *pPressed)
{
    // IT7259 typical register map (adjust based on actual datasheet)
    // 0x00: Status register (bit 7: touch detected)
    // 0x01-0x02: X coordinate (12-bit)
    // 0x03-0x04: Y coordinate (12-bit)

    uint8_t status = i2cReadByte(devAddr, 0x00);
    *pPressed = (status & 0x80) ? 1 : 0;

    if(*pPressed)
    {
        uint8_t xh = i2cReadByte(devAddr, 0x01);
        uint8_t xl = i2cReadByte(devAddr, 0x02);
        uint8_t yh = i2cReadByte(devAddr, 0x03);
        uint8_t yl = i2cReadByte(devAddr, 0x04);

        *pX = ((uint16_t)(xh & 0x0F) << 8) | xl;
        *pY = ((uint16_t)(yh & 0x0F) << 8) | yl;
    }
}

//*****************************************************************************
//
// Main function
//
//*****************************************************************************
int
main(void)
{
    unsigned long ulSysClock;

    // Enable lazy stacking for interrupt handlers
    ROM_FPUEnable();
    ROM_FPULazyStackingEnable();

    // Set system clock to 50MHz using PLL (same as fulltest and st7789)
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);

    ulSysClock = ROM_SysCtlClockGet();

    // Enable GPIO port F for LED (PF2)
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);

    // Enable UART0 and GPIOA peripherals
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    // Enable processor interrupts
    ROM_IntMasterEnable();

    // Configure PA0 and PA1 for UART function
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // Configure UART for 115200, 8-N-1 operation
    ROM_UARTConfigSetExpClk(UART0_BASE, ROM_SysCtlClockGet(), 115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));

    // Print startup message
    printf("\r\n");
    printf("=========================================\r\n");
    printf("        I2C Bus Scanner for LM4F120        \r\n");
    printf("=========================================\r\n");
    printf("System Clock: %ld Hz\r\n", ulSysClock);
    printf("I2C SCL: PB2\r\n");
    printf("I2C SDA: PB3\r\n");
    printf("Baud rate: 100kHz\r\n");
    printf("UART: 115200 8-N-1\r\n");
    printf("Touch Chip: IT7259\r\n");
    printf("=========================================\r\n");

    // Initialize I2C (exactly same as st7789)
    i2c_init();
    printf("I2C initialized successfully\r\n");

    // Initialize TP_INT (PD7)
    InitTP_INT();
    printf("TP_INT (PD7) initialized as input with pull-up\r\n");
    printf("=========================================\r\n");

    // Blink LED twice to show program is running
    for(int i = 0; i < 2; i++)
    {
        ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
        DelayMs(200);
        ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
        DelayMs(200);
    }

    // First scan
    i2cDetect();

    // Test IT7259 at common addresses
    uint8_t possibleAddrs[] = {0x46, 0x38, 0x48, 0x15, 0x5A};
    uint8_t devAddr = 0xFF;

    printf("Scanning for IT7259 touch chip...\r\n");
    for(int i = 0; i < sizeof(possibleAddrs); i++)
    {
        uint8_t test = i2cReadByte(possibleAddrs[i], 0x00);
        // If no error, we found the device
        if(ROM_I2CMasterErr(I2C0_MASTER_BASE) == I2C_MASTER_ERR_NONE)
        {
            devAddr = possibleAddrs[i];
            printf("Found IT7259 at address 0x%02X, chip ID: 0x%02X\r\n", devAddr, test);
            break;
        }
    }

    // Main loop
    while(1)
    {
        if(devAddr != 0xFF)
        {
            uint16_t x, y;
            uint8_t pressed;

            IT7259_ReadTouch(devAddr, &x, &y, &pressed);

            if(pressed)
            {
                printf("Touch detected: X=%d, Y=%d\r\n", x, y);
                // Blink LED when touch detected
                ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
                DelayMs(50);
                ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
            }
            else
            {
                printf("No touch\r\n");
            }
        }
        else
        {
            // Re-scan bus every 3 seconds if no device found
            i2cDetect();
        }

        DelayMs(200);
    }
}
