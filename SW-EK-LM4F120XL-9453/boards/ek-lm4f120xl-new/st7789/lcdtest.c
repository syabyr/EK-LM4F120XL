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
#include "it7259_wrapper.h"
#include "uart_wrapper.h"

// 画点函数（优化版，减少函数调用开销）
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color) {
    if (x >= ST7789_LCD_WIDTH-1 || y >= ST7789_LCD_HEIGHT-1) return; // 边界检查，因为要画2x2的点

    uint8_t hi = (color >> 8) & 0xFF;
    uint8_t lo = (uint8_t)color;

    // 直接设置窗口为2x2，连续写4个像素，比调用FillArea快
    st7789_SetWindow(x, y, x+1, y+1);
    LCD_IO_WriteData(&hi, 1); LCD_IO_WriteData(&lo, 1);
    LCD_IO_WriteData(&hi, 1); LCD_IO_WriteData(&lo, 1);
    LCD_IO_WriteData(&hi, 1); LCD_IO_WriteData(&lo, 1);
    LCD_IO_WriteData(&hi, 1); LCD_IO_WriteData(&lo, 1);
}

// Bresenham画线算法
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
    int16_t dx, dy, sx, sy, err, e2;

    dx = abs(x2 - x1);
    dy = abs(y2 - y1);
    sx = (x1 < x2) ? 1 : -1;
    sy = (y1 < y2) ? 1 : -1;
    err = dx - dy;

    while(1) {
        LCD_DrawPoint(x1, y1, color);
        if (x1 == x2 && y1 == y2) break;

        e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

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
    // Enable lazy stacking for interrupt handlers.
    ROM_FPUEnable();
    ROM_FPULazyStackingEnable();

    // Set system clock to 80MHz.
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);

    // Enable GPIO for on-board LED (PF2).
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);
    ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0); // 熄灭LED

    // Enable processor interrupts.
    ROM_IntMasterEnable();

    // Initialize UART.
    uart_init();
    printf("helloworld.\r\n");
    unsigned long freq = ROM_SysCtlClockGet();
    printf("System Clock: %ld Hz\r\n", freq);

    // Initialize LCD.
    printf("正在初始化LCD...\r\n");
    LCD_IO_Init();
    new_init(); // LCD寄存器初始化
    st7789_Clear(ST7789_BLACK); // 清屏为黑色
    printf("LCD初始化完成，屏幕分辨率：%dx%d\r\n", ST7789_LCD_WIDTH, ST7789_LCD_HEIGHT);

    // Initialize I2C (for touch screen).
    printf("正在初始化I2C...\r\n");
    i2c_init();
    printf("I2C初始化完成\r\n");

    // Initialize touch panel.
    printf("正在初始化触摸屏...\r\n");
    tp_init();
    printf("触摸屏初始化完成\r\n");

    printf("\r\n=== 触摸画板功能已启动 ===\r\n");
    printf("- 触摸屏幕即可画白色线条\r\n");
    printf("- 双击屏幕清屏\r\n");
    printf("================================================================\r\n");

    uint8_t pointdata[14];
    uint16_t x, y;
    uint16_t lastX = 0, lastY = 0;
    uint8_t pressed;
    uint8_t lastPressed = 0;

    // 主循环
    while(1)
    {
        // 直接读取14字节触摸数据
        i2cReadBytes(IT7269_ADDR, POINT_BUFFER_INDEX, pointdata, 14);

        // 判断是否有触摸（POINT标志位0x08）
        pressed = (pointdata[0] & POINT_FLAG) ? 1 : 0;

        if(pressed)
        {
            // 解析原始坐标
            uint16_t rawX = ((pointdata[3] & 0x0F) << 8) | pointdata[2];
            uint16_t rawY = ((pointdata[3] & 0xF0) << 4) | pointdata[4];

            // 坐标映射：原始坐标0-240直接映射到屏幕240x240
            // 如果方向不对或者坐标范围不对，在这里调整
            x = rawX % 240;
            y = rawY % 240;

            // 可选：如果X/Y方向反了，打开下面的注释
            // x = 239 - x; // X轴翻转
            // y = 239 - y; // Y轴翻转
            // { uint16_t t = x; x = y; y = t; } // 交换X/Y轴

            ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2); // 点亮蓝色LED

            if(lastPressed)
            {
                // 从上一个点到当前点画线
                LCD_DrawLine(lastX, lastY, x, y, ST7789_WHITE);
            }
            else
            {
                // 刚按下，画一个起点
                LCD_DrawPoint(x, y, ST7789_WHITE);
            }

            printf("[触摸] 原始坐标 X=%4d Y=%4d | 屏幕坐标 X=%3d Y=%3d\r\n", rawX, rawY, x, y);
            lastX = x;
            lastY = y;
            lastPressed = 1;
        }
        else
        {
            ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0); // 熄灭LED
            lastPressed = 0;
        }

        // 手势处理：双击清屏
        if(pointdata[0] & GESTURES_FLAG) // 手势标志0x80
        {
            if(pointdata[1] == GESTURE_DOUBLE_TAP) // 双击手势
            {
                printf("[手势] 双击屏幕 -> 清屏\r\n");
                st7789_Clear(ST7789_BLACK);
                // 改为200ms延时防抖，避免死循环和重复触发
                DelayMs(200);
            }
        }

        DelayMs(20); // 20ms扫描一次，画线更流畅
    }
}
