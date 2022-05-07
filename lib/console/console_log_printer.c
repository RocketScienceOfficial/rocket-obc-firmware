#include "console_log_printer.h"
#include "logger.h"
#include "console_colors.h"
#include <stdio.h>
#include <stddef.h>

static void logCallback(void **data, size_t size)
{
    const char *level = (const char *)data[1];

    if (level == LOG_LEVEL_DEBUG)
        setConsoleForegroundColor(CYAN);
    else if (level == LOG_LEVEL_ERROR)
        setConsoleForegroundColor(RED);
    else if (level == LOG_LEVEL_RAW)
        setConsoleForegroundColor(WHITE);
    else if (level == LOG_LEVEL_CONSOLE)
        setConsoleForegroundColor(MAGENTA);

    printf("%s", (const char *)data[0]);
}

void attachPrinterToLog()
{
    myLogAddCallback(logCallback);
}