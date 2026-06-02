/**
 * @file lm4f120.ld
 * @brief Linker script for LM4F120H5QR with LVGL
 *
 * 256KB Flash, 32KB SRAM
 * Stack increased to 1KB for LVGL usage
 * Heap limit set to allow LVGL's builtin allocator room
 */

MEMORY
{
    FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 0x00040000
    SRAM  (rwx) : ORIGIN = 0x20000000, LENGTH = 0x00008000
}

SECTIONS
{
    .isr_vector :
    {
        KEEP(*(.isr_vector))
    } > FLASH

    .text :
    {
        *(.text*)
        *(.rodata*)
        _etext = .;
    } > FLASH

    /* Provide symbol for .data initialization source in Flash */
    _sidata = LOADADDR(.data);

    .data :
    {
        _sdata = .;
        *(.data*)
        _edata = .;
    } > SRAM AT > FLASH

    .bss :
    {
        _sbss = .;
        *(.bss*)
        *(COMMON)
        _ebss = .;
    } > SRAM

    .ARM.exidx :
    {
        *(.ARM.exidx*)
    } > SRAM

    /* Heap and stack symbols for _sbrk */
    _heap_start = .;
    _heap_end = 0x20007000;  /* Leave 4KB for stack at top of SRAM */
}
