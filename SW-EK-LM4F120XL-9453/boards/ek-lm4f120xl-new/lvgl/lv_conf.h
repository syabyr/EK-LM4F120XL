/**
 * @file lv_conf.h
 * @brief LVGL configuration for LM4F120H5QR + ST7789 (240x240 RGB565)
 *
 * Target: ARM Cortex-M4F, 80MHz, 256KB Flash, 32KB SRAM
 * Display: ST7789V 240x240 SPI, RGB565
 * Touch: IT7269 I2C capacitive touch
 */

#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>

/*====================
   COLOR SETTINGS
 *====================*/
#define LV_COLOR_DEPTH          16
#define LV_COLOR_CHROMA_KEY     lv_color_hex(0x00ff00)

/*=========================
   STDLIB & MEMORY
 *=========================*/
#define LV_USE_STDLIB_MALLOC    LV_STDLIB_BUILTIN
#define LV_USE_STDLIB_STRING    LV_STDLIB_BUILTIN
#define LV_USE_STDLIB_SPRINTF   LV_STDLIB_BUILTIN

#define LV_MEM_SIZE             (16U * 1024U)   /* 16KB LVGL heap */
#define LV_MEM_ADR              0
#define LV_MEM_BUF_MAX_NUM      16

/*====================
   HAL SETTINGS
 *====================*/
#define LV_DISP_DEF_REFR_PERIOD 33              /* ~30fps */
#define LV_INDEV_DEF_READ_PERIOD 30             /* 30ms touch poll */
#define LV_TICK_PERIOD_MS       1

/*====================
   OS
 *====================*/
#define LV_USE_OS               LV_OS_NONE

/*====================
   LOGGING
 *====================*/
#define LV_USE_LOG              0
/* LV_LOG_LEVEL is set by lv_conf_internal.h when LV_USE_LOG is 0 */
#define LV_LOG_PRINTF           0

/*====================
   ASSERTS
 *====================*/
#define LV_USE_ASSERT_NULL      0
#define LV_USE_ASSERT_MALLOC    0
#define LV_USE_ASSERT_STYLE     0
#define LV_USE_ASSERT_MEM_INTEGRITY 0
#define LV_USE_ASSERT_OBJ       0

/*========================
   RENDERING
 *========================*/
#define LV_USE_DRAW_SW          1
#define LV_DRAW_SW_DRAW_UNIT_CNT 1
#define LV_DRAW_SW_COMPLEX      1

#define LV_DRAW_SW_SUPPORT_RGB565   1
#define LV_DRAW_SW_SUPPORT_RGB565A8 1
#define LV_DRAW_SW_SUPPORT_RGB888   0
#define LV_DRAW_SW_SUPPORT_ARGB8888 0
#define LV_DRAW_SW_SUPPORT_L8       0
#define LV_DRAW_SW_SUPPORT_AL88     0

#define LV_USE_DRAW_VGLITE     0
#define LV_USE_DRAW_PXP        0
#define LV_USE_DRAW_DAVE2D     0
#define LV_USE_DRAW_SDL        0

/*=======================
   FEATURE CONFIGURATION
 *=======================*/
#define LV_USE_ANIMATION       1
#define LV_USE_FLEX            1
#define LV_USE_GRID            0

/*==================
   WIDGET USAGE
 *==================*/
#define LV_USE_ARC             1
#define LV_USE_BAR             1
#define LV_USE_BTN             1
#define LV_USE_BTNMATRIX       1
#define LV_USE_CANVAS          0
#define LV_USE_CHECKBOX        0
#define LV_USE_DROPDOWN        0
#define LV_USE_IMAGE           1
#define LV_USE_LABEL           1
#define LV_USE_LINE            0
#define LV_USE_MENU            0
#define LV_USE_MSGBOX          0
#define LV_USE_ROLLER          0
#define LV_USE_SCALE           0
#define LV_USE_SLIDER          1
#define LV_USE_SWITCH          1
#define LV_USE_SPAN            0
#define LV_USE_SPINBOX         0
#define LV_USE_TABVIEW         0
#define LV_USE_TABLE           0
#define LV_USE_TEXTAREA        0
#define LV_USE_TILEVIEW        0
#define LV_USE_LED             0
#define LV_USE_LIST            0
#define LV_USE_CHART           0
#define LV_USE_CALENDAR        0
#define LV_USE_KEYBOARD        0
#define LV_USE_METER           0
#define LV_USE_COLORWHEEL      0
#define LV_USE_IMGBTN          0
#define LV_USE_WINDOW          0
#define LV_USE_OBJ_ID          0
#define LV_USE_OBJ_ID_BUILTIN  0
#define LV_USE_OBJ_TREE        0

/*==================
   THEMES
 *==================*/
#define LV_USE_THEME_DEFAULT   1
#define LV_USE_THEME_SIMPLE    0
#define LV_THEME_DEFAULT_DARK  0
#define LV_THEME_DEFAULT_GROW  1
#define LV_THEME_DEFAULT_TRANSITION_TIME 80

/*==================
   FONT USAGE
 *==================*/
#define LV_FONT_MONTSERRAT_8    0
#define LV_FONT_MONTSERRAT_10   0
#define LV_FONT_MONTSERRAT_12   0
#define LV_FONT_MONTSERRAT_14   1
#define LV_FONT_MONTSERRAT_16   1
#define LV_FONT_MONTSERRAT_18   0
#define LV_FONT_MONTSERRAT_20   1
#define LV_FONT_MONTSERRAT_22   0
#define LV_FONT_MONTSERRAT_24   0
#define LV_FONT_MONTSERRAT_26   0
#define LV_FONT_MONTSERRAT_28   0
#define LV_FONT_MONTSERRAT_30   0
#define LV_FONT_MONTSERRAT_32   0
#define LV_FONT_MONTSERRAT_34   0
#define LV_FONT_MONTSERRAT_36   0
#define LV_FONT_MONTSERRAT_38   0
#define LV_FONT_MONTSERRAT_40   0
#define LV_FONT_MONTSERRAT_42   0
#define LV_FONT_MONTSERRAT_44   0
#define LV_FONT_MONTSERRAT_46   0
#define LV_FONT_MONTSERRAT_48   0

#define LV_FONT_DEFAULT         &lv_font_montserrat_14
#define LV_FONT_FMT_TXT_LARGE   0
#define LV_USE_FONT_COMPRESSED   0
#define LV_USE_FONT_SUBPX       0
#define LV_USE_BIDI             0
#define LV_USE_ARABIC_PERSIAN_CHARS 0

/*==================
   TEXT
 *==================*/
#define LV_TXT_ENC             LV_TXT_ENC_UTF8
#define LV_TXT_BREAK_CHARS     " ,.;:-_"
#define LV_TXT_LINE_BREAK_LONG_LEN 0

/*==================
   LAYOUTS
 *==================*/
/* Note: LV_LAYOUT_FLEX and LV_LAYOUT_GRID are enum values defined by LVGL
   when LV_USE_FLEX / LV_USE_GRID are enabled. Do not #define them here. */

/*==================
   SNAPSHOTS
 *==================*/
#define LV_USE_SNAPSHOT        0

/*==================
   SYSTEM MONITOR
 *==================*/
#define LV_USE_SYSMON          0

/*==================
   OTHERS
 *==================*/
#define LV_USE_IME_PINYIN      0
#define LV_USE_IME_LANG_CN     0
#define LV_USE_OBSERVER        0
#define LV_USE_FILE_EXPLORER   0
#define LV_USE_NAVKEY          0

/*==================
   EXAMPLES & DEMOS
 *==================*/
#define LV_BUILD_EXAMPLES      0
#define LV_BUILD_DEMOS         0
#define LV_USE_DEMO_WIDGETS    0
#define LV_USE_DEMO_BENCHMARK  0
#define LV_USE_DEMO_STRESS     0
#define LV_USE_DEMO_MUSIC      0
#define LV_USE_DEMO_MULTILANG  0
#define LV_USE_DEMO_RENDER     0
#define LV_USE_DEMO_FLEX_LOGIN 0

/*==================
   FILE SYSTEM
 *==================*/
#define LV_USE_FS_STDIO        0
#define LV_USE_FS_POSIX        0
#define LV_USE_FS_FATFS        0
#define LV_USE_FS_LITTLEFS     0
#define LV_USE_FS_ARDUINO_ESP  0
#define LV_USE_FS_WIN32        0
#define LV_USE_FS_MEMFS        0

/*==================
   IMAGE DECODERS
 *==================*/
#define LV_USE_LODEPNG        0
#define LV_USE_LIBPNG         0
#define LV_USE_BMP            0
#define LV_USE_TJPGD          0
#define LV_USE_LIBJPEG_TURBO  0
#define LV_USE_GIF            0
#define LV_USE_QRCODE         0
#define LV_USE_FREETYPE       0
#define LV_USE_TINY_TTF       0
#define LV_USE_RLE            0

/*==================
   DEVICE DRIVERS
 *==================*/
#define LV_USE_SDL            0
#define LV_USE_X11            0
#define LV_USE_WAYLAND        0
#define LV_USE_WINDOWS        0
#define LV_USE_EVDEV          0
#define LV_USE_LIBINPUT       0
#define LV_USE_NUTTX          0
#define LV_USE_LINUX_DRM      0
#define LV_USE_OPENGLES       0
#define LV_USE_DISPMANX       0

/*==================
   PROFILER
 *==================*/
#define LV_USE_PROFILER       0

/*==================
   DEBUG
 *==================*/
#define LV_USE_REFR_DEBUG     0
#define LV_USE_MONKEY         0

/*==================
   VECTOR GRAPHICS
 *==================*/
#define LV_USE_VECTOR_GRAPHIC 0
#define LV_USE_THORVG         0

/*==================
   3D
 *==================*/
#define LV_USE_3D             0

/*==================
   NVIC
 *==================*/
#define LV_USE_NEMA_GFX       0

/*==================
   VGLITE
 *==================*/
#define LV_USE_VG_LITE        0

#endif /* LV_CONF_H */
