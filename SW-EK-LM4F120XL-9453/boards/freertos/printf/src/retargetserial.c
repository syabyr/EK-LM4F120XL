#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <reent.h>

#include "driverlib/rom.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/uart.h"

#include "FreeRTOS.h"
#include "task.h"

#ifdef __GNUC__
int _write(int fd, char *ptr, int len)
{
    (void) fd;
    int ulCount = len;
    char *pucBuffer = ptr;
    while(ulCount--)
    {
        ROM_UARTCharPut(UART0_BASE, *pucBuffer++);
    }
    return len;
}
#endif

extern char __heap_start;
extern char __heap_end;

caddr_t _sbrk(int incr)
{
    static char *heap_ptr = 0;
    char *prev;

    if (heap_ptr == 0)
        heap_ptr = &__heap_start;

    prev = heap_ptr;
    if (heap_ptr + incr > &__heap_end)
    {
        errno = ENOMEM;
        return (caddr_t) -1;
    }
    heap_ptr += incr;
    return (caddr_t) prev;
}
int _close(int file)
{
    (void) file;
    return -1;
}
int _fstat(int file, struct stat *st)
{
    (void) file;
    (void) st;
    return 0;
}
int _isatty(int file)
{
    (void) file;
    return 1;
}
int _lseek(int file, int ptr, int dir)
{
    (void) file;
    (void) ptr;
    (void) dir;
    return 0;
}
int _read(int file, char *ptr, int len)
{
    (void) file;
    (void) ptr;
    (void) len;
    return 0;
}

/* Thread-safe newlib malloc hooks using FreeRTOS scheduler.
 * These override the weak defaults in newlib that do nothing. */
void __malloc_lock(struct _reent *reent)
{
    (void) reent;
    vTaskSuspendAll();
}

void __malloc_unlock(struct _reent *reent)
{
    (void) reent;
    xTaskResumeAll();
}