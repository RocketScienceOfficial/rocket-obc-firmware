#include "test_radio_sender.h"
#include "pinout.h"
#include "radio.h"
#include "radio_config.h"
#include "radio_protocol.h"
#include "logger.h"
#include "time_tracker.h"

static lora_data_t s_LoraData;
static unsigned int s_TimerOffset = 0;

MY_TEST_INIT_FUNC(RADIO_SENDER_NAME)
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

MY_TEST_FUNC_DYNAMIC(RADIO_SENDER_NAME, 1)
{
    if (runEvery(5000, &s_TimerOffset))
    {
        MY_LOG_CORE_INFO("Sending packet...");

        char text[] = "Hello world!";

        radio_body_t body = {
            .command = 'T',
            .payloadSize = sizeof(text) / sizeof(char),
            .payload = text,
        };

        radioSendPacket(&s_LoraData, &body);
    }

    MY_TEST_END();
}