#include "st7789.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "driverlib/ssi.h"
void     LCD_IO_Init(void) {
    // SPI bus init and LCD RESX pin toggle

}
void LCD_IO_WriteCommand(uint8_t command) {
    // SPI write command
    ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0);
    ROM_SSIDataPut(SSI0_BASE, command);
    while(SSIBusy(SSI0_BASE))
    {
    }
}
void LCD_IO_WriteData(uint8_t data, uint8_t length) {
    // SPI write data
    ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, GPIO_PIN_2);
    ///*
    //int n = length;
    //printf("%d\r\n", n);

    ROM_SSIDataPut(SSI0_BASE, data);
    while(SSIBusy(SSI0_BASE))
    {
    }

    /*
    while(n--)
    {
        ROM_SSIDataPut(SSI0_BASE, *data++);
        while(SSIBusy(SSI0_BASE))
        {
        }
    }
    */
    //*/
   //ROM_SSIDataPut(SSI0_BASE, *data);
    
}
uint16_t LCD_IO_ReadData(void) {
    // SPI read data
    return 0;
}
void     LCD_IO_Delay(uint32_t delay) {
    // Basic MCU delay
    DelayMs(delay);
}