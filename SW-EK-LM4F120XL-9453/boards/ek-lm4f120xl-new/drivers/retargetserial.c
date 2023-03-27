#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "driverlib/rom.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/uart.h"

#ifdef __GNUC__
int _write(int fd, char *ptr, int len)
{
    int ulCount = len;
    char *pucBuffer = ptr;
    while(ulCount--)
    {
        ROM_UARTCharPut(UART0_BASE, *pucBuffer++);
    }
    return len;
}
#endif

caddr_t _sbrk(int incr)
{
    return (caddr_t) 0x20007000;
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