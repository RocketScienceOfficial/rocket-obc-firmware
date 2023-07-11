#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pinout.h"
#include "core/common/measurements_utils.h"
#include "core/common/radio_utils.h"
#include "drivers/tools/board_control.h"

static RadioUtilPacketData s_Packet;

int main()
{
    boardInit(0);

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
        if (checkRadioPacket(&s_Packet))
        {
            if (s_Packet.body.command == MEASUREMENTS_RADIO_COMMAND_ID)
            {
                MeasurementData measurement = {0};

                memcpy(&measurement, &s_Packet.body.payload, s_Packet.body.payloadSize);

                logMeasurementData(&measurement);
            }
        }
    }

    return 0;
}