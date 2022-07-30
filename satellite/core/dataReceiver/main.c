#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "logger.h"
#include "receiver_logger.h"
#include "log_serial.h"
#include "console_commands.h"
#include "remote_commands.h"
#include "radio_controller.h"
#include "commands_utils.h"
#include "console_input.h"

static measurement_data_t s_MeasurementData;
static int s_RadioSignalStrength;

int main()
{
    stdio_init_all();

    attachSerialToLog(myLogGetCoreLogger());
    attachSerialToLog(myLogGetCommandOutputLogger());
    attachSerialToLog(myLogGetRemoteCommandLogger());

    MY_LOG_CORE_INFO("Initializing...");

    initializeCommands();
    initializeRadio();
    initializeReceiverLogger();

    MY_LOG_CORE_INFO("Everything is ready!");

    while (1)
    {
        if (consoleAvailable())
        {
            checkCommand();
        }

        if (checkRadioPacket(&s_MeasurementData, &s_RadioSignalStrength))
        {
            receiver_send_data_t data = {
                .measurement = s_MeasurementData,
                .condition = {
                    .measureRAMUsagePercent = 0,
                    .receiverRAMUsagePercent = 0,
                    .radioSignalStrength = s_RadioSignalStrength,
                }};

            logReceiverData(&data);
        }
    }

    return 0;
}