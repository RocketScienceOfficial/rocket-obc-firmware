#include "radio_controller.h"
#include "radio.h"
#include "radio_protocol.h"
#include "pinout.h"
#include "radio_utils.h"
#include "config.h"
#include "hardware_utils.h"
#include <stdlib.h>
#include <string.h>

static LoRaData s_LoraData;

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
            .dio0 = SX1278_DIO0_GPIO};

        loraInit(&s_LoraData, &loraPinout);
        loraBegin(&s_LoraData, RADIO_FREQUENCY_HZ);
        loraSetTxPower(&s_LoraData, RADIO_DBM);
        loraSetSpreadingFactor(&s_LoraData, RADIO_SPREADING_FACTOR);
        loraSetSignalBandwidth(&s_LoraData, RADIO_SIGNAL_BANDWIDTH);
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