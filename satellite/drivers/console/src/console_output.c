#include "drivers/console/console_output.h"
#include <stdarg.h>
#include <stdio.h>

VOID consoleLog(const STRING format, ...)
{
    va_list ap;

    va_start(ap, format);
    vprintf(format, ap);
    va_end(ap);

    fflush(stdout);
}

VOID consoleLogError(const STRING format, ...)
{
    va_list ap;

    va_start(ap, format);
    vprintf(format, ap);
    va_end(ap);

    fflush(stdout);
}