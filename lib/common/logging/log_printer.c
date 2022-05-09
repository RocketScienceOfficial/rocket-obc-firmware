#include "log_printer.h"
#include "logger.h"
#include "console_colors.h"
#include <stdio.h>

static void logCallback(const char* level, const char* msg)
{
    if (level == LOG_LEVEL_INFO)
        setConsoleForegroundColor(GREEN);
    else if (level == LOG_LEVEL_DEBUG)
        setConsoleForegroundColor(CYAN);
    else if (level == LOG_LEVEL_ERROR)
        setConsoleForegroundColor(RED);

    printf(msg);
}

void attachPrinterToLog()
{
    myLogCreateSink(myLogGetCoreLogger(), &logCallback, MY_LOG_CORE_PATTERN);
}