/* 
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * This file is part of the TinyUSB stack.
 */

/** \ingroup group_demo
 * \defgroup group_board Boards Abstraction Layer
 *  @{ */

#ifndef _BSP_BOARD_H_
#define _BSP_BOARD_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

//#include "ansi_escape.h"
#include "tusb.h"

// Define the default baudrate
#ifndef CFG_BOARD_UART_BAUDRATE
#define CFG_BOARD_UART_BAUDRATE 115200   ///< Default baud rate
#endif


// Initialize on-board peripherals : led, button, uart and USB
void board_init(void);


#if CFG_TUSB_OS == OPT_OS_NONE
  // Get current milliseconds, must be implemented when no RTOS is used
  uint32_t board_millis(void);

#elif CFG_TUSB_OS == OPT_OS_FREERTOS
  static inline uint32_t board_millis(void)
  {
    return ( ( ((uint64_t) xTaskGetTickCount()) * 1000) / configTICK_RATE_HZ );
  }
#else
  #error "board_millis() is not implemented for this OS"
#endif



// TODO remove
static inline void board_delay(uint32_t ms)
{
  uint32_t start_ms = board_millis();
  while (board_millis() - start_ms < ms)
  {
    #if CFG_TUD_ENABLED
    // take chance to run usb background
    tud_task();
    #endif
  }
}

// stdio getchar() is blocking, this is non-blocking version
int board_getchar(void);

#ifdef __cplusplus
 }
#endif

#endif /* _BSP_BOARD_H_ */

/** @} */
