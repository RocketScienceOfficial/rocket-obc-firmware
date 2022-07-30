#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "time_tracker.h"
#include "logger.h"
#include "sd_manager.h"
#include "radio_controller.h"
#include "commands_utils.h"
#include "measurements_manager.h"
#include "config.h"

static unsigned int s_TimerOffset;

int main()
{
    stdio_init_all();

    MY_LOG_CORE_INFO("Initializing...");

    initializeSd();
    beginSd();

    initializeCommands();
    initializeRadio();
    initializeMeasurements();

    MY_LOG_CORE_INFO("Everything is ready!");

    endSd();

    while (1)
    {
        if (consoleAvailable())
        {
            beginSd();
            checkCommand();
            endSd();
        }

        if (runEvery(MEASUREMENTS_UPDATE_RATE_MS, &s_TimerOffset))
        {
            beginSd();
            measurement_data_t measurements = {0};

            takeMeasurements(&measurements);
            sendRadio(&measurements);
            endSd();
        }
    }

    return 0;
}