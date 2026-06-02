#include "st7789.h"
#include "inc/hw_memmap.h"
#include "driverlib/ssi.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"

void DelayMs(unsigned long ulCount);

void LCD_IO_Init(void) {
    // 初始化SPI总线
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    ROM_GPIOPinConfigure(GPIO_PA2_SSI0CLK);
    ROM_GPIOPinConfigure(GPIO_PA3_SSI0FSS);
    ROM_GPIOPinConfigure(GPIO_PA4_SSI0RX);
    ROM_GPIOPinConfigure(GPIO_PA5_SSI0TX);
    ROM_GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3 | GPIO_PIN_2);
    // 把SPI时钟从8MHz提高到40MHz，ST7789支持最高60MHz，速度提高5倍
    ROM_SSIConfigSetExpClk(SSI0_BASE, ROM_SysCtlClockGet(), SSI_FRF_MOTO_MODE_3,
                       SSI_MODE_MASTER, 40000000, 8);
    ROM_SSIEnable(SSI0_BASE);

    // 初始化LCD控制引脚：DC=PE2, BL=PE3
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_2 | GPIO_PIN_3);
    ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2 | GPIO_PIN_3, 0); // DC=0, BL=0（关背光）
    LCD_IO_Delay(100);
    ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, GPIO_PIN_3); // 打开背光
}

void LCD_IO_WriteCommand(uint8_t command) {
    // DC=0，写命令
    ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0);
    ROM_SSIDataPut(SSI0_BASE, command);
    while(ROM_SSIBusy(SSI0_BASE))
    {
    }
}

void LCD_IO_WriteData(uint8_t *data, uint8_t length) {
    // DC=1，写数据
    ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, GPIO_PIN_2);

    // 连续写多个字节
    while(length--)
    {
        ROM_SSIDataPut(SSI0_BASE, *data++);
        while(ROM_SSIBusy(SSI0_BASE))
        {
        }
    }
}

uint8_t LCD_IO_ReadData(void) {
    // SPI读功能暂时不用
    uint8_t data;
    while(ROM_SSIBusy(SSI0_BASE))
    {
    }
    ROM_SSIDataGet(SSI0_BASE, &data);
    return data;
}

void LCD_IO_Delay(uint32_t delay) {
    DelayMs(delay);
}
