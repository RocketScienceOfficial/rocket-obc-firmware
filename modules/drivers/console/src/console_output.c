#include "drivers/console/console_output.h"
#include <stdarg.h>
#include <stdio.h>

FUNCRESULT consoleLog(const STRING format, ...)
{
    va_list ap;

    va_start(ap, format);
    vprintf(format, ap);
    va_end(ap);

    fflush(stdout);

    return SUC_OK;
}

FUNCRESULT consoleLogError(const STRING format, ...)
{
    va_list ap;

    va_start(ap, format);
    vprintf(format, ap);
    va_end(ap);

    fflush(stdout);

    return SUC_OK;
}