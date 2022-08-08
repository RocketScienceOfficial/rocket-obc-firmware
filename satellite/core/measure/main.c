#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "radio_controller.h"
#include "measurements_manager.h"
#include "config.h"
#include "utils/time_tracker.h"
#include "logging/logger.h"
#include "logging/recorder.h"
#include "logging/log_serial.h"
#include "shared/commands_utils.h"

static Timer s_TimerOffset;

int main()
{
    stdio_init_all();
    sleep_ms(5000);

    myLogCreateConsoleSink(myLogGetCoreLogger(), DEFAULT_LOG_SERIAL_PATTERN);
    myLogCreateFileSink(myLogGetCoreLogger(), DEFAULT_LOG_SERIAL_PATTERN, "log.txt");
    myLogCreateFileSink(myLogGetRecordLogger(), "%c\n", "record.txt");

    MY_LOG_CORE_INFO("Initializing...");

    initializeCommands();
    initializeRadio();
    initializeMeasurements();

    MY_LOG_CORE_INFO("Everything is ready!");

    while (true)
    {
        if (consoleAvailable())
        {
            checkCommand();
        }

        if (runEvery(MEASUREMENTS_UPDATE_RATE_MS, &s_TimerOffset))
        {
            MeasurementData measurements = {0};

            takeMeasurements(&measurements);
            sendRadio(&measurements);
        }
    }

    return 0;
}