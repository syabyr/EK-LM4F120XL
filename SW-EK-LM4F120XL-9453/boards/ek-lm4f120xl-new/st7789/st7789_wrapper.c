
#include "i2c_wrapper.h"
#include "spi_wrapper.h"
#include "st7789v.h"
#include "st7789_wrapper.h"


void pin_init(void)
{


    //LCD_DC:PE2 LCD_BL:PE3
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_2|GPIO_PIN_3);
    ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2|GPIO_PIN_3, 0);
}

void st7789_init(void)
{
    i2c_init();
    spi_init();
    pin_init();
    new_init();
}