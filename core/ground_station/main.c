#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pinout.h"
#include "core/common/measurements_utils.h"
#include "core/common/commands_utils.h"
#include "core/common/radio_utils.h"
#include "drivers/console/console_output.h"
#include "kernel/logging/logger.h"
#include "tools/time_tracker.h"

static RadioUtilPacketData s_Packet;
static TIME s_TimerOffset;

int main()
{
    stdio_init_all();
    sleep_ms(5000);

    myLogCreateConsoleSink(myLogGetCoreLogger(), DEFAULT_LOG_SERIAL_PATTERN);

    MY_LOG_CORE_INFO("Initializing...");

    SX127XPinout loraPinout = (SX127XPinout){
        .spi = SX1278_SPI,
        .sck = SX1278_SCK_GPIO,
        .miso = SX1278_MISO_GPIO,
        .mosi = SX1278_MOSI_GPIO,
        .cs = SX1278_CS_GPIO,
        .reset = SX1278_RESET_GPIO,
    };

    initializeCommands();
    // initializeRadio(&loraPinout);
    initializeMeasurementLogger();

    MY_LOG_CORE_INFO("Everything is ready!");

    while (TRUE)
    {
        if (runEvery(1000, &s_TimerOffset))
        {
            MeasurementData measurement = (MeasurementData){
                .accel_x = 0,
                .accel_y = 0,
                .accel_z = 9.8f,
                .cpuTemp = 20.1f,
            };
            
            logMeasurementData(&measurement);
        }

        // if (consoleAvailable())
        // {
        //     checkCommand();
        // }

        // if (checkRadioPacket(&s_Packet))
        // {
        //     if (s_Packet.body.command == MEASUREMENTS_RADIO_COMMAND_ID)
        //     {
        //         MeasurementData measurement = {0};

        //         memcpy(&measurement, &s_Packet.body.payload, s_Packet.body.payloadSize);

        //         logReceiverData(&measurement);
        //     }
        //     else if (s_Packet.body.command == COMMANDS_RADIO_COMMAND_ID)
        //     {
        //         radioRemoteCommandCallback(s_Packet.body.payload, s_Packet.body.payloadSize);
        //     }

        //     radioClearPacket(&s_Packet.body);
        // }
    }

    return 0;
}