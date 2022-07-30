#include "radio_controller.h"
#include "logger.h"
#include "radio.h"
#include "radio_protocol.h"
#include "radio_utils.h"
#include "pinout.h"
#include "config.h"
#include <string.h>

static lora_data_t s_LoraData;
static radio_body_t s_PacketBody;
static int s_RadioPacketValidation;

void initializeRadio()
{
    if (ENABLE_RADIO)
    {
        lora_pinout_t loraPinout = {
            .spi = SX1278_SPI,
            .sck = SX1278_SCK_GPIO,
            .miso = SX1278_MISO_GPIO,
            .mosi = SX1278_MOSI_GPIO,
            .cs = SX1278_CS_GPIO,
            .ss = SX1278_SS_GPIO,
            .reset = SX1278_RESET_GPIO,
            .dio0 = SX1278_DIO0_GPIO};

        loraInit(&s_LoraData, &loraPinout);

        s_LoraData.pinout = loraPinout;
        s_LoraData.txPower = RADIO_DBM;

        loraBegin(&s_LoraData, RADIO_FREQUENCY_HZ);

        loraSetSpreadingFactor(&s_LoraData, RADIO_SPREADING_FACTOR);
        loraSetSignalBandwidth(&s_LoraData, RADIO_SIGNAL_BANDWIDTH);
    }
}

int checkRadioPacket(measurement_data_t *data_out, int *signalStrength)
{
    if (ENABLE_RADIO)
    {
        if (radioCheckPacket(&s_LoraData))
        {
            if (radioReceivePacket(&s_LoraData, &s_PacketBody, &s_RadioPacketValidation))
            {
                MY_LOG_CORE_INFO("Packet received!");

                if (s_RadioPacketValidation)
                {
                    MY_LOG_CORE_INFO("Packet is valid!");

                    memcpy(data_out, s_PacketBody.payload, s_PacketBody.payloadSize);

                    radioClearPacket(&s_PacketBody);
                }
                else
                {
                    MY_LOG_CORE_ERROR("Validation failed!");
                }

                *signalStrength = loraRssi(&s_LoraData);

                MY_LOG_CORE_INFO("Packet processed!");

                return s_RadioPacketValidation;
            }
        }
    }

    return 0;
}