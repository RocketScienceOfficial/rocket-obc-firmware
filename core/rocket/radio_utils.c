#include "radio_utils.h"
#include "config.h"
#include "driver_calling.h"
#include <string.h>

static SX127XData s_LoraData;

VOID initRadio(VOID)
{
    SX127XPinout loraPinout = (SX127XPinout){
        .spi = SPI,
        .sck = SCK,
        .miso = MISO,
        .mosi = MOSI,
        .cs = EXTERNAL_CS_PIN,
        .reset = LORA_RESET_PIN,
    };

    DRIVER_CALL(sx127XInit(&s_LoraData, &loraPinout, RADIO_FREQUENCY_HZ));
    DRIVER_CALL(sx127XSetTxPower(&s_LoraData, RADIO_DBM));
    DRIVER_CALL(sx127XSetSpreadingFactor(&s_LoraData, RADIO_SPREADING_FACTOR));
    DRIVER_CALL(sx127XSetSignalBandwidth(&s_LoraData, RADIO_SIGNAL_BANDWIDTH));
}

BOOL checkRadioPacket(RadioUtilPacketData *packet)
{
    SIZE packetSize = 0;

    DRIVER_CALL(sx127XParsePacket(&s_LoraData, 0, &packetSize));

    if (packetSize > 0)
    {
        if (packetSize != sizeof(RadioPacket))
        {
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

        return deserializeRadioPacket(buffer, &packet->body);
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
    }
}

VOID sendMeasurementData(MeasurementData *data)
{
    RadioBody body = {0};

    body.command = MEASUREMENTS_RADIO_COMMAND_ID;
    body.payloadSize = sizeof(*data);

    memcpy(body.payload, data, body.payloadSize);

    sendRadioPacket(&body);
}