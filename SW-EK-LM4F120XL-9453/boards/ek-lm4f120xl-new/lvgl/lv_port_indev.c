/**
 * @file lv_port_indev.c
 * @brief LVGL input device porting layer for IT7269 touch on LM4F120
 *
 * Bridges LVGL's indev read callback to the IT7269 I2C touch controller.
 * Uses the existing it7259_wrapper driver for raw touch reads.
 */

#include "lv_port_indev.h"
#include "it7259_wrapper.h"
#include "i2c_wrapper.h"

static lv_indev_t * indev_touch;

/**
 * @brief Touch read callback for LVGL
 *
 * Reads the IT7269 touch controller and reports touch state to LVGL.
 */
static void touch_read_cb(lv_indev_t * indev, lv_indev_data_t * data)
{
    static int32_t last_x = 0;
    static int32_t last_y = 0;

    uint16_t x, y;
    uint8_t pressed;

    IT7269_ReadTouch(&x, &y, &pressed);

    if (pressed) {
        /* Map raw coordinates to 240x240 screen */
        data->point.x = x % 240;
        data->point.y = y % 240;
        data->state = LV_INDEV_STATE_PRESSED;
        last_x = data->point.x;
        last_y = data->point.y;
    } else {
        /* Return last known position on release */
        data->point.x = last_x;
        data->point.y = last_y;
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

void lv_port_indev_init(void)
{
    indev_touch = lv_indev_create();
    lv_indev_set_type(indev_touch, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev_touch, touch_read_cb);
}
