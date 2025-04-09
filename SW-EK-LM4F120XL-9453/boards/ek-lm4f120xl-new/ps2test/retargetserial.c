#include "retargetserial.h"

//*****************************************************************************
//
// Send a string to the UART.
//
//*****************************************************************************
void
UARTSend(const unsigned char *pucBuffer, unsigned long ulCount)
{
    //
    // Loop while there are more characters to send.
    //
    while(ulCount--)
    {
        //
        // Write the next character to the UART.
        //
        //ROM_UARTCharPutNonBlocking(UART0_BASE, *pucBuffer++);
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

caddr_t _sbrk(int incr)
{
    return (caddr_t) 0;
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