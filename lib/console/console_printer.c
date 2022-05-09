#include "console_printer.h"
#include "console_input.h"
#include "logger.h"
#include "console_colors.h"
#include <stdio.h>

static void logCallback(const char *level, const char *msg)
{
    setConsoleForegroundColor(MAGENTA);

    printf(msg);
}

void attachPrinterToConsoleLog()
{
    myLogCreateSink(myLogGetConsoleLogger(), &logCallback, MY_LOG_CONSOLE_PATTERN);
}