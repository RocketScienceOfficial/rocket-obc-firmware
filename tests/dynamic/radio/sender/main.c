#include "pico/stdlib.h"
#include "pinout.h"
#include "lora.h"
#include "radio_protocol.h"
#include "test_framework.h"
#include "logger.h"
#include "log_serial.h"
#include "time_tracker.h"
#include <string.h>
#include "radio_config.h"

static lora_data_t s_LoraData;
static unsigned int s_TimerOffset = 0;

void start();
void initialize();
void loop();

int main()
{
    start();

    initialize();

    while (1)
    {
        loop();
    }

    return 0;
}

void start()
{
    stdio_init_all();
    sleep_ms(5000);
}

void initialize()
{
    attachSerialToLog(myLogGetCoreLogger());

    MY_LOG_CORE_INFO("Initializing...");

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

   // MY_ASSERT(loraBegin(&s_LoraData, RADIO_FREQUENCY_HZ));

    loraSetSpreadingFactor(&s_LoraData, RADIO_SPREADING_FACTOR);
    loraSetSignalBandwidth(&s_LoraData, RADIO_SIGNAL_BANDWIDTH);

    MY_LOG_CORE_INFO("Everything is ready!");
}

void loop()
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
}