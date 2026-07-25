/**
 * @file lv_port_disp.c
 * @brief LVGL display porting layer for ST7789V 240x240 on LM4F120
 */

#include "lv_port_disp.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/rom.h"
#include "driverlib/ssi.h"
#include "driverlib/gpio.h"
#include "st7789v.h"

#define DISP_HOR_RES  240
#define DISP_VER_RES  240

extern volatile uint32_t g_flush_count;

#define BUF_ROW_COUNT 10
static uint8_t disp_buf1[DISP_HOR_RES * BUF_ROW_COUNT * 2];

static lv_display_t * disp_obj;

static void disp_flush_cb(lv_display_t * disp, const lv_area_t * area, uint8_t * px_map)
{
    int32_t w = area->x2 - area->x1 + 1;
    int32_t h = area->y2 - area->y1 + 1;

    st7789_SetWindow((uint16_t)area->x1, (uint16_t)area->y1,
                     (uint16_t)area->x2, (uint16_t)area->y2);

    ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, GPIO_PIN_2);

    /* RGB565: LVGL little-endian [lo,hi], ST7789 wants [hi,lo] over SPI */
    uint32_t pixel_count = (uint32_t)w * h;
    uint8_t * p = px_map;
    while (pixel_count--) {
        while (ROM_SSIBusy(SSI0_BASE));
        ROM_SSIDataPut(SSI0_BASE, p[1]);
        while (ROM_SSIBusy(SSI0_BASE));
        ROM_SSIDataPut(SSI0_BASE, p[0]);
        p += 2;
    }

    g_flush_count++;
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
