#include "i2c_wrapper.h"
#include "spi_wrapper.h"
#include "st7789v.h"
#include "it7259_wrapper.h"


void GPIODIntHandler(void)
{
    
}

void tp_init()
{
    //TP_INT:PD7
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    ROM_GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_7);
    ROM_IntMasterEnable();
    ROM_IntEnable(INT_GPIOD);
    ROM_IntPrioritySet(INT_GPIOD, 0x00);
}