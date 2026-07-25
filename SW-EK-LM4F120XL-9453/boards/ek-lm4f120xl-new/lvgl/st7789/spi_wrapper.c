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
#include "inc/hw_ssi.h"
#include "driverlib/ssi.h"
#include "spi_wrapper.h"

int spi_init(void)
{
    // spi init
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    ROM_GPIOPinConfigure(GPIO_PA2_SSI0CLK);
    ROM_GPIOPinConfigure(GPIO_PA3_SSI0FSS);
    ROM_GPIOPinConfigure(GPIO_PA4_SSI0RX);
    ROM_GPIOPinConfigure(GPIO_PA5_SSI0TX);
    ROM_GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3 |
                   GPIO_PIN_2);
    ROM_SSIConfigSetExpClk(SSI0_BASE, ROM_SysCtlClockGet(), SSI_FRF_MOTO_MODE_3,
                       SSI_MODE_MASTER, 8000000, 8);
    ROM_SSIEnable(SSI0_BASE);
}