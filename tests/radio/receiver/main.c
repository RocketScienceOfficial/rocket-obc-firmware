#include "pico/stdlib.h"
#include "pinout.h"
#include "lora.h"
#include "my_assert.h"
#include "logger.h"
#include "log_printer.h"
#include "console_colors.h"

static lora_data_t s_LoraData;

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
    resetColorsAndEffects();

    attachPrinterToLog();

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

    MY_ASSERT(loraBegin(&s_LoraData, SX1278_FREQ_HZ));

    MY_LOG_CORE_INFO("Everything is ready!");
}

void loop()
{
    int packetSize = loraParsePacket(&s_LoraData, 0);

    if (packetSize)
    {
        MY_LOG_CORE_INFO("Received packet '");

        while (loraAvailable(&s_LoraData))
        {
            MY_LOG_CORE_INFO("%c", (char)loraRead(&s_LoraData));
        }
    }
}