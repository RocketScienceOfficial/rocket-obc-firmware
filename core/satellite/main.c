#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "config.h"
#include "pinout.h"
#include "measurements_manager.h"
#include "common/commands_utils.h"
#include "common/radio_utils.h"
#include "tools/time_tracker.h"
#include "kernel/logging/logger.h"
#include "drivers/console/console_output.h"

#include "drivers/storage/flash_driver.h"
#include "drivers/storage/sd_driver.h"

static RadioUtilPacketData s_Packet;
static TIME s_TimerOffset;

int main()
{
    stdio_init_all();

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
    initializeRadio(&loraPinout);
    initializeMeasurements();

    MY_LOG_CORE_INFO("Everything is ready!");

    while (TRUE)
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

            radioClearPacket(&s_Packet.body);
        }

        if (runEvery(MEASUREMENTS_UPDATE_RATE_MS, &s_TimerOffset))
        {
            MeasurementData measurements = {0};

            takeMeasurements(&measurements);

            RadioBody body = {
                .command = MEASUREMENTS_RADIO_COMMAND_ID,
                .payloadSize = sizeof(measurements),
            };

            BYTE *buffer = (BYTE *)malloc(body.payloadSize);

            memcpy(buffer, &measurements, body.payloadSize);

            body.payload = buffer;

            sendRadioPacket(&body);

            free(buffer);
        }
    }

    return 0;
}