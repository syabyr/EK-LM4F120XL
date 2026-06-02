---
name: lvgl-demo-project
description: LVGL demo project for LM4F120+ST7789+IT7269 - working configuration and pitfalls
metadata:
  type: project
---

# LVGL Demo Project for LM4F120 + ST7789 (240x240) + IT7269 Touch

## Project Location
`boards/ek-lm4f120xl-new/lvgl/`

## Key Files
- `lv_conf.h` — LVGL config (RGB565, 16KB heap, minimal widgets)
- `main.c` — Demo UI with buttons, slider, switch, color preview
- `lv_port_disp.c/h` — Display flush callback (ST7789V SPI)
- `lv_port_indev.c/h` — Touch read callback (IT7269 I2C)
- `retargetserial.c` — UART printf + proper `_sbrk` with heap_start/heap_end
- `startup_gcc.c` — Full 155-entry vector table, SysTickIntHandler for lv_tick_inc
- `lm4f120.ld` — Linker script: _data/_bss symbols, 4KB stack, heap_end=0x20006800
- `Makefile` — All LVGL + board driver sources, `make flash` uses lm4flash

## Critical Pitfalls & Solutions

### 1. Stack size MUST be >= 4KB
- LVGL render chain is deep: lv_timer_handler -> render -> draw -> flush
- 2KB stack crashes on complex UI (renders partially then dies)
- 512 words (2KB) = FAIL, 1024 words (4KB) = PASS
- Stack is static in startup_gcc.c: `pulStack[1024]`

### 2. Must call lv_refr_now(NULL) before entering main loop
- Without this, first call to lv_timer_handler() hangs forever
- lv_timer_handler waits for tick, but internal refresh timer logic deadlocks
- Call lv_refr_now(NULL) right after UI creation to force first render

### 3. Manual tick fallback needed
- SysTick ISR may not fire (ROM version mismatch on LM4F120)
- In main loop: check if g_tick_count changed; if not, call lv_tick_inc(1) manually
- Still configure SysTick as backup — it may work on some hardware revisions

### 4. No printf in flush callback
- printf consumes massive stack space (~200+ bytes)
- Calling printf inside flush (which is deep in render chain) causes stack overflow
- Only use simple counter increments in flush

### 5. Vector table must be full 155 entries
- LM4F120 has many interrupt vectors; short vector table causes Hard Fault
- Copy the complete vector table from the original st7789/startup_gcc.c

### 6. st7789.h vs st7789v.h conflict
- LCD_IO_ReadData returns uint8_t in st7789.h but uint16_t in st7789v.h
- Only include st7789v.h — it already declares all LCD_IO_* functions

### 7. Linker script symbol names must match startup
- Startup uses _etext, _data, _edata, _bss, _ebss (StellarisWare convention)
- NOT _sidata, _sdata, _sbss (GCC convention) — these won't match

### 8. LVGL default theme requires many widgets enabled
- Even unused widgets: LV_USE_ARC, LV_USE_BTNMATRIX, LV_USE_IMAGE, LV_USE_SPINNER, LV_USE_WIN
- Without them, theme_apply() gets undefined references

## Memory Usage (working config)
- Flash: ~194KB / 256KB (75.8%)
- SRAM: ~28KB / 32KB (87.5%) — includes 16KB LVGL heap + 4KB stack + 4.8KB display buf

## Hardware Pins (same as st7789 project)
- SPI0 (PA2-PA5) -> ST7789 LCD
- I2C0 (PB2/PB3) -> IT7269 touch
- PE2 -> LCD DC, PE3 -> LCD BL
- PD7 -> Touch IRQ, PF2 -> Blue LED
