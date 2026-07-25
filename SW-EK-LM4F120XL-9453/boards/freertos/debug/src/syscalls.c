//*****************************************************************************
//
// syscalls.c - Stub implementations for newlib system calls.
//
// On bare-metal targets, newlib requires these POSIX system call stubs.
// They are never called in this project, but providing them eliminates
// linker warnings from -specs=nosys.specs.
//
//*****************************************************************************

#include <errno.h>
#include <sys/stat.h>

int _close(int fd)
{
    (void)fd;
    errno = ENOSYS;
    return -1;
}

int _lseek(int fd, int offset, int whence)
{
    (void)fd;
    (void)offset;
    (void)whence;
    errno = ENOSYS;
    return -1;
}

int _read(int fd, char *buf, int len)
{
    (void)fd;
    (void)buf;
    (void)len;
    errno = ENOSYS;
    return -1;
}

int _write(int fd, const char *buf, int len)
{
    (void)fd;
    (void)buf;
    (void)len;
    errno = ENOSYS;
    return -1;
}

int _fstat(int fd, struct stat *st)
{
    (void)fd;
    st->st_mode = S_IFCHR;
    return 0;
}

int _isatty(int fd)
{
    (void)fd;
    return 1;
}
