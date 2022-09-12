#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "receiver_logger.h"
#include "radio_controller.h"
#include "logging/logger.h"
#include "logging/log_serial.h"
#include "shared/commands_utils.h"
#include "shared/tick.h"

static int s_RadioSignalStrength;
static MeasurementData s_MeasurementData;
static TickData s_TickData;

int main()
{
    stdio_init_all();

    myLogCreateConsoleSink(myLogGetCoreLogger(), DEFAULT_LOG_SERIAL_PATTERN);

    MY_LOG_CORE_INFO("Initializing...");

    initializeCommands();
    initializeRadio();
    initializeReceiverLogger();

    MY_LOG_CORE_INFO("Everything is ready!");

    while (true)
    {
        if (consoleAvailable())
        {
            checkCommand();
        }

        if (checkRadioPacket(&s_MeasurementData, &s_RadioSignalStrength))
        {
            ReceiverSendData data = {
                .measurement = s_MeasurementData,
                .condition = {
                    .measureRAMUsagePercent = 0,
                    .receiverRAMUsagePercent = 0,
                    .radioSignalStrength = s_RadioSignalStrength,
                }};

            logReceiverData(&data);
        }

        tick(&s_TickData);
    }

    return 0;
}