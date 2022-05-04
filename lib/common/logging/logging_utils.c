#include "logging_utils.h"
#include <stdlib.h>
#include <stdio.h>

char *vlogBase(const char *format, va_list args)
{
    size_t len = vsnprintf(NULL, 0, format, args) + 1;
    char *newformat = malloc(len);

    vsnprintf(newformat, len, format, args);

    return newformat;
}

char *logBase(const char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    char *message = vlogBase(format, ap);
    va_end(ap);

    return message;
}