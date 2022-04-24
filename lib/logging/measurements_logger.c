#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "measurements_logger.h"
#include "time_tracker.h"

static logMeasureCallback_t logCallback = NULL;

static void __logMeasure(const char *format, va_list args)
{
    int len = snprintf(NULL, 0, MEASURE_FORMAT, format) + 1;
    char *newformat = malloc(len);

    snprintf(newformat, len, MEASURE_FORMAT, format);

    int flen = vprintf(newformat, args) + 1;

    char buffer[128];
    vsnprintf(buffer, flen, newformat, args);
    
    logCallback(buffer);

    free(newformat);
}

void myLogMeasureSetCallback(logMeasureCallback_t callback)
{
    logCallback = callback;
}

void myLogMeasure(const char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    __logMeasure(format, ap);
    va_end(ap);
}

void myLogMeasureEnd()
{
    myLogMeasure("\n");
}