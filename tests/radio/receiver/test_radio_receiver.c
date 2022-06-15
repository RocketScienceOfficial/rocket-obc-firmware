#include "test_radio_receiver.h"
#include "pinout.h"
#include "lora.h"
#include "radio_config.h"
#include "radio_protocol.h"
#include "logger.h"
#include "time_tracker.h"

static lora_data_t s_LoraData;
static radio_body_t s_PacketBody;
static int s_RadioPacketValidation;

MY_TEST_INIT_FUNC(RADIO_RECEIVER_NAME)
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

    MY_ASSERT(loraBegin(&s_LoraData, RADIO_FREQUENCY_HZ));

    loraSetSpreadingFactor(&s_LoraData, RADIO_SPREADING_FACTOR);
    loraSetSignalBandwidth(&s_LoraData, RADIO_SIGNAL_BANDWIDTH);

    MY_TEST_END();
}

MY_TEST_FUNC_DYNAMIC(RADIO_RECEIVER_NAME, 1)
{
    if (radioReceivePacket(&s_LoraData, &s_PacketBody, &s_RadioPacketValidation))
    {
        MY_LOG_CORE_INFO("Received packet!");

        if (s_RadioPacketValidation)
        {
            MY_LOG_CORE_INFO("Packet is valid!");
            MY_LOG_CORE_INFO("%s", s_PacketBody.payload);

            radioClearPacket(&s_PacketBody);
        }
        else
        {
            MY_LOG_CORE_INFO("Validation failed!");
        }

        MY_LOG_CORE_INFO("RSSI: %d", loraRssi(&s_LoraData));
        MY_LOG_CORE_INFO("Packet processed!");
    }

    MY_TEST_END();
}