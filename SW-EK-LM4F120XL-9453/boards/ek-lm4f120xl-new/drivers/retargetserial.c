#include <stdio.h>
#include <sys/stat.h>
#include "inc/hw_memmap.h"
#include "driverlib/rom.h"

#ifdef __GNUC__
int _write(int fd, char *ptr, int len)
{
    int count=len;
    while(count--)
    {
        ROM_UARTCharPut(UART0_BASE, *ptr++);
    }
    return len;
}
#endif

caddr_t _sbrk(int incr)
{
    return (caddr_t) 0;
}

int _fstat(int file, struct stat *st)
{
    return 0;
}

int _read(int file, char *ptr, int len)
{
    return 0;
}