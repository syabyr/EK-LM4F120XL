
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

#include "i2c_wrapper.h"
#include "spi_wrapper.h"
#include "st7789_wrapper.h"
#include "uart_wrapper.h"
#include "it7259_wrapper.h"
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
    // Enable processor interrupts.
    //
    ROM_IntMasterEnable();

    uart_init();

    printf("helloworld.\r\n");
    unsigned long freq=ROM_SysCtlClockGet();
    printf("System Clock: %ld Hz\r\n", freq);

    st7789_init();
    i2c_init();
    tp_init();

    printf("IT7269 Touch Driver initialized\r\n");
    printf("I2C Address: 0x%02X\r\n", IT7269_ADDR);

    i2cDetect();

    st7789_Clear(ST7789_BLACK);

    uint16_t x, y;
    uint8_t pressed;
    // uint8_t last_pressed = 0; // 不再使用
    IT7269_Gesture_t gesture;

    printf("\r\n=== IT7269 触摸&手势测试 ===\r\n");
    printf("支持功能：单击、双击、上下左右滑动\r\n");
    printf("================================================================\r\n");

    // 主循环
    while(1)
    {
        // 读取触摸坐标
        IT7269_ReadTouch(&x, &y, &pressed);
        if(pressed)
        {
            ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2); // 点亮蓝色LED
            printf("[触摸] X=%4d, Y=%4d\r\n", x, y);
        }
        else
        {
            ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0); // 熄灭LED
        }

        // 2. 读取手势
        gesture = IT7269_ReadGesture();
        if(gesture.type != 0)
        {
            printf("[手势] ");
            switch(gesture.type)
            {
                case GESTURE_TAP:
                    printf("单击\r\n");
                    break;
                case GESTURE_DOUBLE_TAP:
                    printf("双击\r\n");
                    break;
                case GESTURE_FLICK:
                    printf("滑动，方向：");
                    switch(gesture.direction)
                    {
                        case DIR_UP: printf("上\r\n"); break;
                        case DIR_DOWN: printf("下\r\n"); break;
                        case DIR_LEFT: printf("左\r\n"); break;
                        case DIR_RIGHT: printf("右\r\n"); break;
                        case DIR_UPPER_RIGHT: printf("右上\r\n"); break;
                        case DIR_LOWER_RIGHT: printf("右下\r\n"); break;
                        case DIR_UPPER_LEFT: printf("左上\r\n"); break;
                        case DIR_LOWER_LEFT: printf("左下\r\n"); break;
                        default: printf("未知(0x%02X)\r\n", gesture.direction); break;
                    }
                    break;
                default:
                    printf("未知手势(0x%02X)\r\n", gesture.type);
                    break;
            }
        }

        DelayMs(50); // 50ms扫描一次
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
