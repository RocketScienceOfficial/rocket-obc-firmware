#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "pico/stdlib.h"
#include "config.h"
#include "radio_controller.h"
#include "measurements_manager.h"
#include "utils/time_tracker.h"
#include "kernel/logging/logger.h"
#include "kernel/logging/recorder.h"
#include "kernel/console/console_output.h"
#include "shared/functions_utils.h"
#include "shared/commands_utils.h"
#include "shared/radio_utils.h"
#include "shared/tick.h"

static RadioUtilPacketData s_Packet;
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

    SX1278Pinout loraPinout = getRadioPinout();

    initializeCommands();
    initializeRadio(&loraPinout);
    initializeMeasurements();

    MY_LOG_CORE_INFO("Everything is ready!");

    while (true)
    {
        if (consoleAvailable())
        {
            checkCommand();
        }

        if (checkRadioPacket(&s_Packet))
        {
            if (s_Packet.body.command == COMMANDS_RADIO_COMMAND_ID)
            {
                radioRemoteCommandCallback(s_Packet.body.payload, s_Packet.body.payloadSize);
            }

            FUNC_CALL(radioClearPacket(&s_Packet.body));
        }

        if (runEvery(MEASUREMENTS_UPDATE_RATE_MS, &s_TimerOffset))
        {
            MeasurementData measurements = {0};

            takeMeasurements(&measurements);

            RadioBody body = {
                .command = MEASUREMENTS_RADIO_COMMAND_ID,
                .payloadSize = sizeof(measurements),
            };

            uint8_t *buffer = malloc(body.payloadSize);

            memcpy(buffer, &measurements, body.payloadSize);

            body.payload = buffer;

            sendRadioPacket(&body);

            free(buffer);
        }

        tick(&s_TickData);
    }

    return 0;
}