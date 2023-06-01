#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pinout.h"
#include "core/common/radio_utils.h"
#include "core/common/measurements_utils.h"
#include "tools/time_tracker.h"

#define MEASUREMENTS_UPDATE_RATE_MS 10

static RadioUtilPacketData s_Packet;
static TIME s_TimerOffset;

int main()
{
    stdio_init_all();

    SX127XPinout loraPinout = (SX127XPinout){
        .spi = SX1278_SPI,
        .sck = SX1278_SCK_GPIO,
        .miso = SX1278_MISO_GPIO,
        .mosi = SX1278_MOSI_GPIO,
        .cs = SX1278_CS_GPIO,
        .reset = SX1278_RESET_GPIO,
    };

    initializeRadio(&loraPinout);

    while (TRUE)
    {
        if (runEvery(MEASUREMENTS_UPDATE_RATE_MS, &s_TimerOffset))
        {
            MeasurementData measurements = {0};

            RadioBody body = {
                .command = MEASUREMENTS_RADIO_COMMAND_ID,
                .payloadSize = sizeof(measurements),
            };

            BYTE *buffer = (BYTE *)malloc(body.payloadSize);

            memcpy(body.payload, &measurements, body.payloadSize);

            body.payload = buffer;

            sendRadioPacket(&body);

            free(buffer);
        }
    }

    return 0;
}