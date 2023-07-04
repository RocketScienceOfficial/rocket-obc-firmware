#include "core/common/radio_utils.h"
#include "core/common/driver_calling.h"
#include "drivers/console/console_input.h"
#include "services/logging/logger.h"
#include <stdlib.h>
#include <string.h>

static SX127XData s_LoraData;

VOID initializeRadio(SX127XPinout *pinout)
{
    DRIVER_CALL(sx127XInit(&s_LoraData, pinout, RADIO_FREQUENCY_HZ));
    DRIVER_CALL(sx127XSetTxPower(&s_LoraData, RADIO_DBM));
    DRIVER_CALL(sx127XSetSpreadingFactor(&s_LoraData, RADIO_SPREADING_FACTOR));
    DRIVER_CALL(sx127XSetSignalBandwidth(&s_LoraData, RADIO_SIGNAL_BANDWIDTH));

    MY_LOG_CORE_INFO("Radio initialized!");
}

BOOL checkRadioPacket(RadioUtilPacketData *packet)
{
    SIZE packetSize = 0;

    DRIVER_CALL(sx127XParsePacket(&s_LoraData, 0, &packetSize));

    if (packetSize)
    {
        MY_LOG_CORE_INFO("Packet is available!");

        BYTE buffer[packetSize];
        SIZE i = 0;
        BOOL available = FALSE;

        DRIVER_CALL(sx127XAvailable(&s_LoraData, &available));

        while (available)
        {
            DRIVER_CALL(sx127XRead(&s_LoraData, &buffer[i]));
            DRIVER_CALL(sx127XAvailable(&s_LoraData, &available));

            i++;
        }

        packet->body = (RadioBody){0};

        INT32 rssi = 0;

        DRIVER_CALL(sx127XRssi(&s_LoraData, &rssi));

        if (rssi < RADIO_MIN_RSSI)
        {
            DRIVER_CALL(sx127XSetSpreadingFactor(&s_LoraData, RADIO_ERROR_SPREADING_FACTOR));
            DRIVER_CALL(sx127XSetSignalBandwidth(&s_LoraData, RADIO_ERROR_SIGNAL_BANDWIDTH));
        }
        else
        {
            DRIVER_CALL(sx127XSetSpreadingFactor(&s_LoraData, RADIO_SPREADING_FACTOR));
            DRIVER_CALL(sx127XSetSignalBandwidth(&s_LoraData, RADIO_SIGNAL_BANDWIDTH));
        }

        packet->signalStrength = rssi;

        BOOL packetValidation = deserializeRadioPacket(buffer, packetSize, &packet->body);

        if (packetValidation)
        {
            MY_LOG_CORE_INFO("Packet is valid!");
        }
        else
        {
            MY_LOG_CORE_ERROR("Validation failed!");
        }

        MY_LOG_CORE_INFO("Packet processed!");

        return packetValidation;
    }

    return FALSE;
}

VOID sendRadioPacket(RadioBody *body)
{
    BYTE *packetBuffer;
    SIZE packetBufferSize = 0;

    BOOL serializationResult = serializeRadioPacket(body, &packetBuffer, &packetBufferSize);

    if (serializationResult)
    {
        DRIVER_CALL(sx127XWriteBuffer(&s_LoraData, packetBuffer, packetBufferSize));

        free(packetBuffer);

        MY_LOG_CORE_INFO("Packet sent!");
    }
    else
    {
        MY_LOG_CORE_ERROR("Packet serialization failed!");
    }
}