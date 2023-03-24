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
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bsp/board.h"


#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"
#include "task.h"
#include "timers.h"


//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF PROTYPES
//--------------------------------------------------------------------+

/* Blink pattern
 * - 250 ms  : device not mounted
 * - 1000 ms : device mounted
 * - 2500 ms : device is suspended
 */
enum  {
  BLINK_NOT_MOUNTED = 250,
  BLINK_MOUNTED = 1000,
  BLINK_SUSPENDED = 2500,
};

// static timer & task
StaticTimer_t blinky_tmdef;
StaticTimer_t blinky_tmdef2;


TimerHandle_t blinky_tm;
TimerHandle_t blinky_tm2;

void led_blinky_cb(TimerHandle_t xTimer);
void led2_blinky_cb(TimerHandle_t xTimer);


//--------------------------------------------------------------------+
// Main
//--------------------------------------------------------------------+

int main(void)
{
  board_init();

  // soft timer for blinky
  blinky_tm = xTimerCreateStatic(NULL, pdMS_TO_TICKS(BLINK_NOT_MOUNTED), true, NULL, led_blinky_cb, &blinky_tmdef);

  blinky_tm2 = xTimerCreateStatic(NULL, pdMS_TO_TICKS(BLINK_SUSPENDED), true, NULL, led2_blinky_cb, &blinky_tmdef2);


  xTimerStart(blinky_tm, 0);
  xTimerStart(blinky_tm2, 0);



  vTaskStartScheduler();


  return 0;
}



//--------------------------------------------------------------------+
// BLINKING TASK
//--------------------------------------------------------------------+
void led_blinky_cb(TimerHandle_t xTimer)
{
  (void) xTimer;
  static bool led_state = false;

  board_led_write(led_state);
  led_state = 1 - led_state; // toggle
}

void led2_blinky_cb(TimerHandle_t xTimer)
{
  (void) xTimer;
  static bool led_state = false;

  board_led2_write(led_state);
  led_state = 1 - led_state; // toggle
}