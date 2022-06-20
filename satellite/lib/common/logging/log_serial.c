#include "log_serial.h"
#include <stdio.h>

static void logCallback(const char* level, const char* msg)
{
    printf(msg);
}

void attachSerialToLog(logger_data_t* logger)
{
    myLogCreateSink(logger, &logCallback, LOG_SERIAL_JSON_PATTERN);
}