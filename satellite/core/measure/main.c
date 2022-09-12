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
#include "shared/tick.h"

static Timer s_TimerOffset;
static TickData s_TickData;

int main()
{
    stdio_init_all();

    if (DEBUG_MODE)
    {
        myLogCreateConsoleSink(myLogGetCoreLogger(), DEFAULT_LOG_SERIAL_PATTERN);
    }

    myLogCreateFileSink(myLogGetCoreLogger(), DEFAULT_LOG_SERIAL_PATTERN, LOG_FILE_INDEX);
    myLogCreateFileSink(myLogGetRecordLogger(), "%c\n", RECORD_FILE_INDEX);

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

        tick(&s_TickData);
    }

    return 0;
}