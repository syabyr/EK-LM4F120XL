/**
 * @file lv_port_disp.c
 * @brief LVGL display porting layer for ST7789V 240x240 on LM4F120
 *
 * Bridges LVGL's flush callback to the ST7789V SPI display driver.
 * Uses PARTIAL render mode with a 20-row buffer to conserve SRAM.
 */

#include "lv_port_disp.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ssi.h"
#include "driverlib/rom.h"
#include "driverlib/ssi.h"
#include "driverlib/gpio.h"
#include "st7789v.h"

#define DISP_HOR_RES  240
#define DISP_VER_RES  240

/* Partial render buffer: 10 rows x 240 cols x 2 bytes/pixel = 4800 bytes */
#define BUF_ROW_COUNT 10
static uint8_t disp_buf1[DISP_HOR_RES * BUF_ROW_COUNT * 2];

static lv_display_t * disp_obj;

/**
 * @brief Flush callback: send pixel data to ST7789V via SPI
 *
 * LVGL renders into px_map, we set the display window and stream pixels out.
 * For ST7789V with SPI, the byte order in RGB565 may need swapping.
 */
static void disp_flush_cb(lv_display_t * disp, const lv_area_t * area, uint8_t * px_map)
{
    int32_t x1 = area->x1;
    int32_t y1 = area->y1;
    int32_t x2 = area->x2;
    int32_t y2 = area->y2;

    int32_t w = x2 - x1 + 1;
    int32_t h = y2 - y1 + 1;

    /* Set the display window for this area */
    st7789_SetWindow((uint16_t)x1, (uint16_t)y1, (uint16_t)x2, (uint16_t)y2);

    /* DC pin high for data mode */
    ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, GPIO_PIN_2);

    /* Stream pixel data: px_map is RGB565 in lv_color16_t format.
     * LVGL stores as little-endian uint16, ST7789 expects big-endian over SPI,
     * so we need to swap bytes. We write high byte first, then low byte. */
    uint32_t pixel_count = (uint32_t)w * h;
    uint8_t * p = px_map;

    while (pixel_count--) {
        /* Swap bytes: LVGL stores [lo, hi], ST7789 wants [hi, lo] over SPI */
        while (ROM_SSIBusy(SSI0_BASE));
        ROM_SSIDataPut(SSI0_BASE, p[1]);  /* high byte first */
        while (ROM_SSIBusy(SSI0_BASE));
        ROM_SSIDataPut(SSI0_BASE, p[0]);  /* low byte second */
        p += 2;
    }

    /* Tell LVGL flushing is done */
    lv_display_flush_ready(disp);
}

void lv_port_disp_init(void)
{
    disp_obj = lv_display_create(DISP_HOR_RES, DISP_VER_RES);
    lv_display_set_color_format(disp_obj, LV_COLOR_FORMAT_RGB565);
    lv_display_set_flush_cb(disp_obj, disp_flush_cb);
    lv_display_set_buffers(disp_obj, disp_buf1, NULL, sizeof(disp_buf1),
                           LV_DISPLAY_RENDER_MODE_PARTIAL);
}
