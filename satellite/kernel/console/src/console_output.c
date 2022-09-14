#include "kernel/console/console_output.h"
#include <stdarg.h>
#include <stdio.h>

void logSerial(const char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    vprintf(format, ap);
    va_end(ap);
}

void logSerialError(const char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    vprintf(format, ap);
    va_end(ap);
}