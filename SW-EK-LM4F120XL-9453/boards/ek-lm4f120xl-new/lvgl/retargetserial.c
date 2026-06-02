/**
 * @file retargetserial.c
 * @brief printf retarget to UART0 and proper _sbrk for newlib/LVGL
 *
 * _sbrk uses linker symbols _heap_start and _heap_end from lm4f120.ld
 * to properly manage heap memory for LVGL's builtin allocator.
 */

#include "retargetserial.h"
#include <sys/stat.h>

extern uint32_t _heap_start;
extern uint32_t _heap_end;

//*****************************************************************************
//
// Send a string to the UART.
//
//*****************************************************************************
void
UARTSend(const unsigned char *pucBuffer, unsigned long ulCount)
{
    while(ulCount--)
    {
        ROM_UARTCharPut(UART0_BASE, *pucBuffer++);
    }
}

#ifdef __GNUC__
int _write(int fd, char *ptr, int len)
{
    UARTSend((unsigned char *)ptr, len);
    return len;
}
#endif

/**
 * @brief Proper _sbrk implementation using linker-defined heap bounds
 *
 * The original st7789 project's _sbrk always returned 0x20007000,
 * which is broken for any code that uses malloc/realloc.
 * This version tracks the heap pointer properly.
 */
caddr_t _sbrk(int incr)
{
    static uint8_t *heap_ptr = NULL;
    uint8_t *prev_ptr;

    if (heap_ptr == NULL)
    {
        heap_ptr = (uint8_t *)&_heap_start;
    }

    prev_ptr = heap_ptr;

    /* Check if we have enough space */
    if (heap_ptr + incr > (uint8_t *)&_heap_end)
    {
        return (caddr_t)-1;  /* Out of memory */
    }

    heap_ptr += incr;
    return (caddr_t)prev_ptr;
}

int _close(int file)
{
    return -1;
}

int _fstat(int file, struct stat *st)
{
    return 0;
}

int _isatty(int file)
{
    return 1;
}

int _lseek(int file, int ptr, int dir)
{
    return 0;
}

int _read(int file, char *ptr, int len)
{
    return 0;
}
