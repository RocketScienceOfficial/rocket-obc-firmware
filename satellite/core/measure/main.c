#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "time_tracker.h"
#include "logger.h"
#include "recorder.h"
#include "log_serial.h"
#include "radio_controller.h"
#include "commands_utils.h"
#include "measurements_manager.h"
#include "config.h"

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