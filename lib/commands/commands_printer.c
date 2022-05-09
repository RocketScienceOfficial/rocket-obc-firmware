#include "commands_printer.h"
#include "console_commands.h"
#include "logger.h"
#include "console_colors.h"
#include <stdio.h>

static void logCallback(const char *level, const char *msg)
{
    setConsoleForegroundColor(WHITE);

    printf(msg);
}

void attachPrinterToCommandsLog()
{
    myLogCreateSink(myLogGetCommandLogger(), &logCallback, MY_LOG_COMMAND_PATTERN);
}