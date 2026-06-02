/**
 * @file main.c
 * @brief LVGL demo for LM4F120 + ST7789 240x240 with IT7269 touch
 *
 * Demo features:
 *   - Title label with colored background
 *   - Brightness slider
 *   - LED toggle switch
 *   - Three interactive buttons (Red/Green/Blue)
 *   - Color preview area that responds to button presses
 *   - Status bar at the bottom
 */

#include <stdio.h>
#include <string.h>

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
#include "driverlib/systick.h"
#include "driverlib/uart.h"

#include "lvgl/lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "st7789v.h"
#include "st7789_wrapper.h"
#include "i2c_wrapper.h"
#include "it7259_wrapper.h"
#include "uart_wrapper.h"

/* Declare external fonts */
LV_FONT_DECLARE(lv_font_montserrat_16)
LV_FONT_DECLARE(lv_font_montserrat_20)

/* ---- Forward declarations ---- */
static void create_demo_ui(void);
static void btn_color_event_cb(lv_event_t * e);
static void led_switch_event_cb(lv_event_t * e);
static void slider_event_cb(lv_event_t * e);

/* ---- Global UI objects ---- */
static lv_obj_t * color_preview;
static lv_obj_t * slider_label;
static lv_obj_t * status_label;
static lv_color_t current_color;

/* ---- SysTick ISR for LVGL tick ---- */
void SysTickIntHandler(void)
{
    lv_tick_inc(1);
}

/* ---- Delay function used by ST7789 driver ---- */
void DelayMs(unsigned long ulCount)
{
    unsigned long freq = ROM_SysCtlClockGet();
    unsigned long mscount = freq / 3000;
    ROM_SysCtlDelay(mscount * ulCount);
}

/* ---- Error handler ---- */
#ifdef DEBUG
void __error__(char *pcFilename, unsigned long ulLine)
{
}
#endif

/* ======== UI Callbacks ======== */

/**
 * @brief Color button press callback
 * Sets the color preview area to the button's color
 */
static void btn_color_event_cb(lv_event_t * e)
{
    lv_obj_t * btn = lv_event_get_target(e);
    uint32_t color_val = (uint32_t)(uintptr_t)lv_obj_get_user_data(btn);

    current_color = lv_color_hex(color_val);
    lv_obj_set_style_bg_color(color_preview, current_color, 0);

    /* Update status text */
    lv_color32_t c32 = lv_color_to_32(current_color, LV_OPA_COVER);
    lv_label_set_text_fmt(status_label,
        "Color: R%02X G%02X B%02X", c32.red, c32.green, c32.blue);

    /* Light the on-board blue LED briefly */
    ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
}

static void btn_color_release_cb(lv_event_t * e)
{
    (void)e;
    ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
}

/**
 * @brief LED toggle switch callback
 */
static void led_switch_event_cb(lv_event_t * e)
{
    lv_obj_t * sw = lv_event_get_target(e);
    if (lv_obj_has_state(sw, LV_STATE_CHECKED)) {
        ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
        lv_label_set_text(status_label, "LED: ON");
    } else {
        ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
        lv_label_set_text(status_label, "LED: OFF");
    }
}

/**
 * @brief Slider value change callback
 */
static void slider_event_cb(lv_event_t * e)
{
    lv_obj_t * slider = lv_event_get_target(e);
    int32_t val = lv_slider_get_value(slider);
    lv_label_set_text_fmt(slider_label, "%ld%%", (long)val);

    /* Align label to the right of the slider area */
    lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_BOTTOM_RIGHT, 0, 0);
}

/* ======== UI Construction ======== */

static void create_demo_ui(void)
{
    lv_obj_t * scr = lv_screen_active();

    /* Clean screen background */
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x1A1A2E), 0);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);

    /* ---- Title bar ---- */
    lv_obj_t * title_bg = lv_obj_create(scr);
    lv_obj_set_size(title_bg, 240, 32);
    lv_obj_align(title_bg, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_bg_color(title_bg, lv_color_hex(0x16213E), 0);
    lv_obj_set_style_bg_opa(title_bg, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(title_bg, 0, 0);
    lv_obj_set_style_radius(title_bg, 0, 0);
    lv_obj_set_style_pad_all(title_bg, 2, 0);
    lv_obj_set_scrollbar_mode(title_bg, LV_SCROLLBAR_MODE_OFF);

    lv_obj_t * title = lv_label_create(title_bg);
    lv_label_set_text(title, "LM4F120 + LVGL");
    lv_obj_set_style_text_color(title, lv_color_hex(0xE94560), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_16, 0);
    lv_obj_center(title);

    /* ---- Color preview area ---- */
    color_preview = lv_obj_create(scr);
    lv_obj_set_size(color_preview, 100, 100);
    lv_obj_align(color_preview, LV_ALIGN_TOP_LEFT, 8, 40);
    current_color = lv_color_hex(0x555555);
    lv_obj_set_style_bg_color(color_preview, current_color, 0);
    lv_obj_set_style_bg_opa(color_preview, LV_OPA_COVER, 0);
    lv_obj_set_style_border_color(color_preview, lv_color_hex(0x0F3460), 0);
    lv_obj_set_style_border_width(color_preview, 2, 0);
    lv_obj_set_style_radius(color_preview, 8, 0);
    lv_obj_set_style_pad_all(color_preview, 0, 0);
    lv_obj_set_scrollbar_mode(color_preview, LV_SCROLLBAR_MODE_OFF);

    /* ---- Color buttons (right side) ---- */
    lv_obj_t * btn_red = lv_btn_create(scr);
    lv_obj_set_size(btn_red, 110, 28);
    lv_obj_align(btn_red, LV_ALIGN_TOP_RIGHT, -8, 40);
    lv_obj_set_style_bg_color(btn_red, lv_color_hex(0xE94560), 0);
    lv_obj_set_style_bg_opa(btn_red, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(btn_red, 6, 0);
    lv_obj_add_event_cb(btn_red, btn_color_event_cb, LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(btn_red, btn_color_release_cb, LV_EVENT_RELEASED, NULL);
    lv_obj_set_user_data(btn_red, (void *)(uintptr_t)0xE94560);
    lv_obj_t * lbl_red = lv_label_create(btn_red);
    lv_label_set_text(lbl_red, "Red");
    lv_obj_center(lbl_red);

    lv_obj_t * btn_green = lv_btn_create(scr);
    lv_obj_set_size(btn_green, 110, 28);
    lv_obj_align(btn_green, LV_ALIGN_TOP_RIGHT, -8, 72);
    lv_obj_set_style_bg_color(btn_green, lv_color_hex(0x0F9D58), 0);
    lv_obj_set_style_bg_opa(btn_green, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(btn_green, 6, 0);
    lv_obj_add_event_cb(btn_green, btn_color_event_cb, LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(btn_green, btn_color_release_cb, LV_EVENT_RELEASED, NULL);
    lv_obj_set_user_data(btn_green, (void *)(uintptr_t)0x0F9D58);
    lv_obj_t * lbl_green = lv_label_create(btn_green);
    lv_label_set_text(lbl_green, "Green");
    lv_obj_center(lbl_green);

    lv_obj_t * btn_blue = lv_btn_create(scr);
    lv_obj_set_size(btn_blue, 110, 28);
    lv_obj_align(btn_blue, LV_ALIGN_TOP_RIGHT, -8, 104);
    lv_obj_set_style_bg_color(btn_blue, lv_color_hex(0x4285F4), 0);
    lv_obj_set_style_bg_opa(btn_blue, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(btn_blue, 6, 0);
    lv_obj_add_event_cb(btn_blue, btn_color_event_cb, LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(btn_blue, btn_color_release_cb, LV_EVENT_RELEASED, NULL);
    lv_obj_set_user_data(btn_blue, (void *)(uintptr_t)0x4285F4);
    lv_obj_t * lbl_blue = lv_label_create(btn_blue);
    lv_label_set_text(lbl_blue, "Blue");
    lv_obj_center(lbl_blue);

    /* ---- LED Switch ---- */
    lv_obj_t * sw_label = lv_label_create(scr);
    lv_label_set_text(sw_label, "LED");
    lv_obj_set_style_text_color(sw_label, lv_color_hex(0xCCCCCC), 0);
    lv_obj_align(sw_label, LV_ALIGN_TOP_LEFT, 8, 148);

    lv_obj_t * sw = lv_switch_create(scr);
    lv_obj_set_size(sw, 50, 26);
    lv_obj_align_to(sw, sw_label, LV_ALIGN_OUT_RIGHT_MID, 8, 0);
    lv_obj_add_event_cb(sw, led_switch_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    /* ---- Brightness Slider ---- */
    lv_obj_t * slider_title = lv_label_create(scr);
    lv_label_set_text(slider_title, "Brightness");
    lv_obj_set_style_text_color(slider_title, lv_color_hex(0xCCCCCC), 0);
    lv_obj_align(slider_title, LV_ALIGN_TOP_LEFT, 8, 185);

    lv_obj_t * slider = lv_slider_create(scr);
    lv_obj_set_width(slider, 200);
    lv_obj_align(slider, LV_ALIGN_TOP_LEFT, 8, 205);
    lv_slider_set_range(slider, 0, 100);
    lv_slider_set_value(slider, 80, LV_ANIM_OFF);
    lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    slider_label = lv_label_create(scr);
    lv_label_set_text(slider_label, "80%");
    lv_obj_set_style_text_color(slider_label, lv_color_hex(0xAAAAAA), 0);
    lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_BOTTOM_RIGHT, 0, 0);

    /* ---- Bottom status bar ---- */
    lv_obj_t * status_bg = lv_obj_create(scr);
    lv_obj_set_size(status_bg, 240, 24);
    lv_obj_align(status_bg, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_color(status_bg, lv_color_hex(0x0F3460), 0);
    lv_obj_set_style_bg_opa(status_bg, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(status_bg, 0, 0);
    lv_obj_set_style_radius(status_bg, 0, 0);
    lv_obj_set_style_pad_all(status_bg, 2, 0);
    lv_obj_set_scrollbar_mode(status_bg, LV_SCROLLBAR_MODE_OFF);

    status_label = lv_label_create(status_bg);
    lv_label_set_text(status_label, "Ready");
    lv_obj_set_style_text_color(status_label, lv_color_hex(0xE94560), 0);
    lv_obj_set_style_text_font(status_label, &lv_font_montserrat_14, 0);
    lv_obj_center(status_label);
}

/* ======== Main ======== */

int main(void)
{
    /* Enable FPU */
    ROM_FPUEnable();
    ROM_FPULazyStackingEnable();

    /* Set system clock to 80 MHz */
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);

    /* Enable GPIO for on-board LED (PF2) */
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);
    ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);

    /* Enable processor interrupts */
    ROM_IntMasterEnable();

    /* Initialize UART for debug output */
    uart_init();
    printf("\r\n=== LM4F120 LVGL Demo ===\r\n");
    printf("System Clock: %ld Hz\r\n", ROM_SysCtlClockGet());

    /* Initialize LCD */
    printf("Initializing LCD...\r\n");
    LCD_IO_Init();
    new_init();
    st7789_Clear(ST7789_BLACK);
    printf("LCD initialized (240x240 RGB565)\r\n");

    /* Initialize I2C and touch */
    printf("Initializing I2C + Touch...\r\n");
    i2c_init();
    tp_init();
    printf("Touch initialized (IT7269)\r\n");

    /* Initialize LVGL */
    printf("Initializing LVGL...\r\n");
    lv_init();

    /* Initialize display and input porting layers */
    lv_port_disp_init();
    lv_port_indev_init();

    /* Set up SysTick for 1ms tick (80MHz / 1000 = 80000) */
    ROM_SysTickPeriodSet(80000 - 1);
    ROM_SysTickIntEnable();
    ROM_SysTickEnable();

    /* Create the demo UI */
    create_demo_ui();
    printf("UI created. Running main loop.\r\n");

    /* Main loop */
    while (1) {
        lv_timer_handler();
    }
}
