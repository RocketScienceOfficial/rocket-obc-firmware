#include "log_printer.h"
#include "logger.h"
#include "measurements_logger.h"
#include <stdio.h>
#include <stddef.h>

static void logCallback(void **data, size_t size)
{
    printf((const char *)data[0]);
}

void attachPrinterToMeasureLog()
{
    myLogMeasureAddCallback(logCallback);
}

void attachPrinterToLog()
{
    myLogAddCallback(logCallback);
}