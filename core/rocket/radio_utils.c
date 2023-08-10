#include "radio_utils.h"
#include "driver_calling.h"
#include "drivers/console/console_input.h"
#include "services/logging/logger.h"
#include <stdlib.h>
#include <string.h>

#define RADIO_DBM 20
#define RADIO_SPREADING_FACTOR 10
#define RADIO_SIGNAL_BANDWIDTH 125E3
#define RADIO_ERROR_SPREADING_FACTOR 10
#define RADIO_ERROR_SIGNAL_BANDWIDTH 125E3
#define RADIO_FREQUENCY_HZ 433E6
#define RADIO_MIN_RSSI -80

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

    if (packetSize > 0)
    {
        MY_LOG_CORE_INFO("Packet is available!");

        if (packetSize != sizeof(RadioPacket))
        {
            MY_LOG_CORE_ERROR("Packet size is too big!");

            return FALSE;
        }

        BYTE buffer[sizeof(RadioPacket)];
        SIZE i = 0;
        BOOL available = FALSE;

        DRIVER_CALL(sx127XAvailable(&s_LoraData, &available));

        while (available)
        {
            DRIVER_CALL(sx127XRead(&s_LoraData, &buffer[i]));
            DRIVER_CALL(sx127XAvailable(&s_LoraData, &available));

            i++;
        }

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

        packet->body = (RadioBody){0};
        packet->signalStrength = rssi;

        BOOL packetValidation = deserializeRadioPacket(buffer, &packet->body);

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
    BYTE packetBuffer[sizeof(RadioPacket)];
    BOOL serializationResult = serializeRadioPacket(body, packetBuffer);

    if (serializationResult)
    {
        DRIVER_CALL(sx127XWriteBuffer(&s_LoraData, packetBuffer, sizeof(RadioPacket)));

        MY_LOG_CORE_INFO("Packet sent!");
    }
    else
    {
        MY_LOG_CORE_ERROR("Packet serialization failed!");
    }
}