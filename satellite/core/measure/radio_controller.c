#include "radio_controller.h"
#include "pinout.h"
#include "config.h"
#include "logging/logger.h"
#include "myhardware/radio.h"
#include "myhardware/radio_protocol.h"
#include "shared/radio_utils.h"
#include "shared/hardware_utils.h"
#include <string.h>
#include <stdlib.h>

static LoRaData s_LoraData;
static RadioBody s_PacketBody;
static bool s_RadioPacketAvailable;
static bool s_RadioPacketValidation;

void initializeRadio()
{
    if (ENABLE_RADIO)
    {
        LoRaPinout loraPinout = {
            .spi = SX1278_SPI,
            .sck = SX1278_SCK_GPIO,
            .miso = SX1278_MISO_GPIO,
            .mosi = SX1278_MOSI_GPIO,
            .cs = SX1278_CS_GPIO,
            .ss = SX1278_SS_GPIO,
            .reset = SX1278_RESET_GPIO,
            .dio0 = SX1278_DIO0_GPIO,
        };

        loraInit(&s_LoraData, &loraPinout);
        loraBegin(&s_LoraData, RADIO_FREQUENCY_HZ);
        loraSetTxPower(&s_LoraData, RADIO_DBM);
        loraSetSpreadingFactor(&s_LoraData, RADIO_SPREADING_FACTOR);
        loraSetSignalBandwidth(&s_LoraData, RADIO_SIGNAL_BANDWIDTH);

        initRadioCommand(&s_LoraData);
    }
}

void sendRadio(MeasurementData *data)
{
    if (ENABLE_RADIO)
    {
        RadioBody body = {
            .command = MEASUREMENTS_RADIO_COMMAND_ID,
            .payloadSize = sizeof(*data),
        };

        uint8_t *buffer = malloc(body.payloadSize);

        memcpy(buffer, data, body.payloadSize);

        body.payload = buffer;

        HW_CALL(radioSendPacket(&s_LoraData, &body));

        free(buffer);
    }
}

bool checkRadioPacket()
{
    if (ENABLE_RADIO)
    {
        HW_CALL(radioReceivePacket(&s_LoraData, &s_RadioPacketAvailable, &s_PacketBody, &s_RadioPacketValidation));

        if (s_RadioPacketAvailable)
        {
            MY_LOG_CORE_INFO("Packet received!");

            if (s_RadioPacketValidation)
            {
                MY_LOG_CORE_INFO("Packet is valid!");

                if (s_PacketBody.command == COMMANDS_RADIO_COMMAND_ID)
                {
                }

                HW_CALL(radioClearPacket(&s_PacketBody));
            }
            else
            {
                MY_LOG_CORE_ERROR("Validation failed!");
            }

            MY_LOG_CORE_INFO("Packet processed!");

            return s_RadioPacketValidation;
        }
    }

    return false;
}