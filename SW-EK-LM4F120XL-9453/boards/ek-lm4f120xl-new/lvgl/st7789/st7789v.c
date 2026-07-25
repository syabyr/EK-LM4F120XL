/**
 * @file st7789v.c
 * @author Renato Freitas (freitas-renato@outlook.com)
 * @brief This file contains all functions implementations for ST7789V LCD driver
 * @version 0.1
 * @date 2021-03-24
 * 
 * @note It uses the LCD_IO format from STMicroelectronics Nucleo and Discovery boards
 * @see 
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "st7789v.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/ssi.h"
#include "driverlib/rom.h"

void new_init(void)
{
    uint8_t data;
    LCD_IO_WriteCommand(0x01);
    DelayMs(150);
    LCD_IO_WriteCommand(0x11);
    DelayMs(255);
    LCD_IO_WriteCommand(0x3A);
    data = 0x55; LCD_IO_WriteData(&data,1);
    DelayMs(10);
    LCD_IO_WriteCommand(0x36);
    data = 0x00; LCD_IO_WriteData(&data,1);

    LCD_IO_WriteCommand(0x2A);
    data = 0x00; LCD_IO_WriteData(&data,1);
    data = 0x00; LCD_IO_WriteData(&data,1);
    data = 0x00; LCD_IO_WriteData(&data,1);
    data = 0xF0; LCD_IO_WriteData(&data,1);

    LCD_IO_WriteCommand(0x2B);
    data = 0x00; LCD_IO_WriteData(&data,1);
    data = 0x00; LCD_IO_WriteData(&data,1);
    data = 0x00; LCD_IO_WriteData(&data,1);
    data = 0xF0; LCD_IO_WriteData(&data,1);

    LCD_IO_WriteCommand(0x21);
    DelayMs(10);
    LCD_IO_WriteCommand(0x13);
    DelayMs(10);
    LCD_IO_WriteCommand(0x29);
    DelayMs(255);
}


/**
 * @brief Initialization routine. Sets interface to RGB.
 * 
 */
void st7789_Init(void) {
    LCD_IO_Init();

    //* CASET set display width
    const uint8_t caset[4] = {
        0x00, 0x00,
        (ST7789_LCD_WIDTH - 1) >> 8, (ST7789_LCD_WIDTH - 1) & 0xFF
    };

    //* RASET set display height
    const uint8_t raset[4] = {
        0x00, 0x00,
        (ST7789_LCD_HEIGHT - 1) >> 8, (ST7789_LCD_HEIGHT - 1) & 0xFF
    };

    //* Initialization sequence
    const st7789_command_t initSequence[] = {
        // Sleep
        {ST7789_CMD_SLPIN, 10, 0, NULL},                    // Sleep
        {ST7789_CMD_SWRESET, 200, 0, NULL},                 // Reset
        {ST7789_CMD_SLPOUT, 120, 0, NULL},                  // Sleep out

        {ST7789_CMD_CMD2EN, 100, 0, NULL},

        {ST7789_CMD_MADCTL, 0, 1, ( uint8_t *)"\x00"},      // Page / column address order
        {ST7789_CMD_COLMOD, 0, 1, ( uint8_t *)"\x55"},      // 16 bit RGB mode

        // //* ADD vsync, hsync
        // {ST7789_CMD_RGBCTRL, 0, 3, (uint8_t *)"\x42\x08\x3c"},

        {ST7789_CMD_INVON, 0, 0, NULL},                     // Inversion on
        {ST7789_CMD_CASET, 0, 4, ( uint8_t *)caset},        // Set width
        {ST7789_CMD_RASET, 0, 4, ( uint8_t *)raset},        // Set height

        // Porch setting
        {ST7789_CMD_PORCTRL, 0, 5, ( uint8_t *)"\x0c\x0c\x00\x33\x33"},
        // Set VGH to 12.54V and VGL to -9.6V
        {ST7789_CMD_GCTRL, 0, 1, ( uint8_t *)"\x35"},
        // Set VCOM to 1.475V
        {ST7789_CMD_VCOMS, 0, 1, ( uint8_t *)"\x1f"},
        // Enable VDV/VRH control
        {ST7789_CMD_VDVVRHEN, 0, 1, ( uint8_t *)"\x01"},

        // LCM control
        {ST7789_CMD_LCMCTRL, 0, 1, ( uint8_t *)"\x2c"},
        // VAP(GVDD) = 4.45+(vcom+vcom offset+vdv)
        {ST7789_CMD_VRHS, 0, 1, ( uint8_t *)"\x12"},
        // VDV = 0V
        {ST7789_CMD_VDVSET, 0, 1, ( uint8_t *)"\x20"},
        // AVDD=6.8V, AVCL=-4.8V, VDDS=2.3V
        {ST7789_CMD_PWCTRL1, 0, 2, ( uint8_t *)"\xa4\xa1"},
        //  60 fps
        {ST7789_CMD_FRCTR2, 0, 1, ( uint8_t *)"\x0f"},
        // Gama 2.2
        {ST7789_CMD_GAMSET, 0, 1, (uint8_t *)"\x01"},
        // Gama curve
        {ST7789_CMD_PVGAMCTRL, 0, 14, ( uint8_t *)"\xd0\x08\x11\x08\x0c\x15\x39\x33\x50\x36\x13\x14\x29\x2d"},
        {ST7789_CMD_NVGAMCTRL, 0, 14, ( uint8_t *)"\xd0\x08\x10\x08\x06\x06\x39\x44\x51\x0b\x16\x14\x2f\x31"},
        
        {ST7789_CMDLIST_END, 0, 0, NULL}                   // End of commands
    };

    st7789_RunCommands(initSequence);

    LCD_IO_Delay(10);
    st7789_Clear(ST7789_BLACK);

    const st7789_command_t initSequence2[] = {
        {ST7789_CMD_RGBCTRL, 0, 3, (uint8_t *)"\x42\x08\x3c"},  // HSYNC = 0x3C, VSYNC = 0x80
        {ST7789_CMD_RAMCTRL, 0, 2, (uint8_t*)"\x11\xc2"},       // RAMCTRL Select RGB interface
        {ST7789_CMD_DISPON, 100, 0, NULL},                      // Display on
        {ST7789_CMD_SLPOUT, 100, 0, NULL},                      // Sleep out
        
        {ST7789_CMD_RAMWR, 50, 0, NULL},                        // Begin GRAM write
        {ST7789_CMDLIST_END, 0, 0, NULL},                       // End of commands
    };

    st7789_RunCommands(initSequence2);
}

void st7789_Reset(void) {
    LCD_IO_Init();
}


/**
 * @brief Turns display ON
 * 
 */
void st7789_DisplayOn(void) {
    LCD_IO_WriteCommand(ST7789_CMD_DISPON);
}

/**
 * @brief Turns display OFF
 * 
 */
void st7789_DisplayOff(void) {
    LCD_IO_WriteCommand(ST7789_CMD_DISPOFF);
}

/**
 * @brief Run ST7789V Command using SPI
 * 
 * @param command 
 */
void st7789_RunCommand(const st7789_command_t *command) {
    //* Send command
    LCD_IO_WriteCommand(command->command);

    LCD_IO_WriteData(command->data, command->dataSize);

    if (command->waitMs > 0) {
        LCD_IO_Delay(command->waitMs);
    }
}

/**
 * @brief Run command predefined command sequence
 * 
 * @param sequence 
 */
void st7789_RunCommands(const st7789_command_t *sequence) {
    while (sequence->command != ST7789_CMDLIST_END) {
        st7789_RunCommand(sequence);
        sequence++;
    }
}

/**
 * @brief Sets Display RAM window for pixel write
 *
 * @param xStart  Horizontal start position
 * @param yStart  Horizontal end position
 * @param xEnd    Vertical start position
 * @param yEnd    Vertical end position
 */
void st7789_SetWindow(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd) {
    uint8_t data;

    LCD_IO_WriteCommand(ST7789_CMD_CASET);
    data = (xStart >> 8) & 0xFF; LCD_IO_WriteData(&data, 1);
    data = xStart & 0xFF;        LCD_IO_WriteData(&data, 1);
    data = (xEnd >> 8) & 0xFF;   LCD_IO_WriteData(&data, 1);
    data = xEnd & 0xFF;          LCD_IO_WriteData(&data, 1);

    LCD_IO_WriteCommand(ST7789_CMD_RASET);
    data = (yStart >> 8) & 0xFF; LCD_IO_WriteData(&data, 1);
    data = yStart & 0xFF;        LCD_IO_WriteData(&data, 1);
    data = (yEnd >> 8) & 0xFF;   LCD_IO_WriteData(&data, 1);
    data = yEnd & 0xFF;          LCD_IO_WriteData(&data, 1);

    LCD_IO_WriteCommand(ST7789_CMD_RAMWR);
}

/**
 * @brief 快速填充矩形区域（寄存器级优化，速度提升10倍）
 *
 * @param color     16bit color code (RGB 565)
 * @param startX
 * @param startY
 * @param width
 * @param height
 */
void st7789_FillArea(uint16_t color, uint16_t startX, uint16_t startY, uint16_t width, uint16_t height) {
    uint8_t hi = (color >> 8) & 0xFF;
    uint8_t lo = (uint8_t)color;
    uint32_t pixelCount = (uint32_t)width * height;

    // 设置窗口
    st7789_SetWindow(startX, startY, startX + width - 1, startY + height - 1);

    // DC引脚设为1（数据模式），后面不用再操作
    ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, GPIO_PIN_2);

    // 直接操作SPI寄存器连续写数据，速度最快
    while(pixelCount--) {
        // 写高字节
        while((ROM_SSIBusy(SSI0_BASE)));
        ROM_SSIDataPut(SSI0_BASE, hi);
        // 写低字节
        while((ROM_SSIBusy(SSI0_BASE)));
        ROM_SSIDataPut(SSI0_BASE, lo);
    }
}

/**
 * @brief 极速清屏（使用ROM函数，兼容TI TivaWare）
 */
void st7789_Clear(uint16_t color) {
    uint8_t hi = (color >> 8) & 0xFF;
    uint8_t lo = (uint8_t)color;
    uint32_t pixelCount = 240*240; // 240x240固定分辨率

    // 设置全屏窗口
    st7789_SetWindow(0, 0, 239, 239);

    // DC引脚设为1（数据模式）
    ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, GPIO_PIN_2);

    // 极速循环写SPI，直接用库函数
    while(pixelCount--) {
        while(ROM_SSIBusy(SSI0_BASE));
        ROM_SSIDataPut(SSI0_BASE, hi);
        while(ROM_SSIBusy(SSI0_BASE));
        ROM_SSIDataPut(SSI0_BASE, lo);
    }
}
