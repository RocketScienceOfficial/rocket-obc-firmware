#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "pico/stdlib.h"
#include "pinout_config.h"
#include "logger.h"
#include "console_log_printer.h"
#include "time_tracker.h"
#include "console_colors.h"
#include "my_assert.h"
#include "sd.h"

void start();
void initialize();
void loop();

int main()
{
    start();

    // sdInit();
    // sdFlush(LOG_FILENAME);

    // return 0;

    initialize();

    while (1)
    {
        loop();
    }

    return 0;
}

void start()
{
    stdio_init_all();
    sleep_ms(5000);
}

void initialize()
{
    myLogInit();

    attachPrinterToLog();

    myLogInfo("Initializing...");

    resetColorsAndEffects();

    sdInit();
    sdInitFile(LOG_FILENAME);
    sdFlush(LOG_FILENAME);
    sdAttachToLogger();

    sdBegin(LOG_FILENAME);

    myLogInfo("Everything is ready!");

    sdEnd(LOG_FILENAME);
}

void loop()
{
    sdBegin(LOG_FILENAME);

    myLogInfo("HELLO");

    sdEnd(LOG_FILENAME);

    sleep_ms(1000);
}