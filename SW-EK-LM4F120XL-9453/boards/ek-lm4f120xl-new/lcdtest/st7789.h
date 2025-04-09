#ifndef _ST7789_H_
#define _ST7789_H_
#include <stdint.h>
#include <stdlib.h>
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
void     LCD_IO_Init(void);                                  // SPI bus init and LCD RESX pin toggle
void     LCD_IO_WriteCommand(uint8_t command);               // SPI write command
void     LCD_IO_WriteData(uint8_t data, uint8_t length);    // SPI write data
uint16_t LCD_IO_ReadData(void);                              // SPI read data
void     LCD_IO_Delay(uint32_t delay);                       // Basic MCU delay
extern void DelayMs(unsigned long ulCount);
#endif /* _ST7789_H_ */