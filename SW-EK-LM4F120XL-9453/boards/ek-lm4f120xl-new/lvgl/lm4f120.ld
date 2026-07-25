/**
 * @file lm4f120.ld
 * @brief Linker script for LM4F120H5QR with LVGL
 *
 * 256KB Flash, 32KB SRAM
 * Symbol names match the original StellarisWare startup_gcc.c conventions.
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

    .data :
    {
        _data = .;
        *(.data*)
        _edata = .;
    } > SRAM AT > FLASH

    .bss :
    {
        _bss = .;
        *(.bss*)
        *(COMMON)
        _ebss = .;
    } > SRAM

    .ARM.exidx :
    {
        *(.ARM.exidx*)
    } > SRAM

    /* Heap and stack symbols for _sbrk.
     * Stack is 4KB (1024 words), so heap must end 5KB from SRAM top
     * to leave margin. */
    _heap_start = .;
    _heap_end = 0x20006800;  /* 4KB stack + 1KB margin at top of 32KB SRAM */
}
