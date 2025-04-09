#ifndef __I2C_WRAPPER_H__
#define __I2C_WRAPPER_H__

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


#include "inc/hw_i2c.h"
#include "driverlib/i2c.h"
#include <stdint.h>

uint8_t i2cReadByte(uint16_t device_address, uint16_t device_register);
void i2cWriteByte(uint8_t device_address, uint8_t device_register, uint8_t device_data);

void i2cReadBytes(uint16_t device_address,uint16_t device_register, uint8_t *pData, size_t size);
void i2cWriteBytes(uint16_t device_address,uint16_t device_register, uint8_t *pData, size_t size);

void i2cDetect();

void i2c_init(void);





#endif