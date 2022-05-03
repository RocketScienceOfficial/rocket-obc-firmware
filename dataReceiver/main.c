#include <stdio.h>
#include "pico/stdlib.h"
#include "pinout_config.h"
#include "logger.h"
#include "log_printer.h"
#include "mg995.h"
#include "console_input.h"
#include "console_commands.h"
#include "time_tracker.h"

int main()
{
    stdio_init_all();
    sleep_ms(5000);

    myLogInit();

    attachPrinterToLog();

    myLogInfo("Starting...");

    mg995_data_t data_1 = {.pin = MG995_PIN_1};
    mg995_data_t data_2 = {.pin = MG995_PIN_2};

    mg995Init(&data_1);
    mg995Init(&data_2);

    myLogInfo("Everything is ready!");

    int timeOffset = getMsSinceBoot(), currentTime = 0;

    consoleProcessCharacter('\r');

    while (1)
    {
        consoleCheckInput();

        if (currentTime >= 4000)
        {
            myLogInfo("Test");

            currentTime = 0;
            timeOffset = getMsSinceBoot();
        }

        currentTime = getMsSinceBoot() - timeOffset;
    }

    return 0;
}