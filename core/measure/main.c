#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pinout.h"
#include "logger.h"
#include "time_tracker.h"
#include "console_input.h"
#include "console_commands.h"
#include "remote_commands.h"
#include "default_console_commands.h"
#include "sd.h"
#include "lora.h"
#include "radio_protocol.h"
#include "bmp280.h"
#include "mpu6050.h"
#include "kalman_filter.h"
#include "log_serial.h"

static lora_data_t s_LoraData;
static unsigned int s_TimerOffset = 0;

void start();
void initialize();
void loop();
void checkCommand();
void takeMeasurements();

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

    registerDefaultConsoleCommands();

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
    s_LoraData.txPower = 20;

    loraSetSpreadingFactor(&s_LoraData, 8);
    loraSetSignalBandwidth(&s_LoraData, 125000);

    loraBegin(&s_LoraData, SX1278_FREQ_HZ);

    sdInit();
    sdInitFile(LOG_CORE_FILENAME);
    sdInitFile(LOG_MEASURE_FILENAME);
    sdFlush(LOG_CORE_FILENAME);
    sdFlush(LOG_MEASURE_FILENAME);
    sdAttachToCoreLogger();
    sdAttachToMeasureLogger();

    sdBegin(LOG_CORE_FILENAME);

    bmp280Init(BMP280_I2C, BMP280_I2C_SDA_PIN, BMP280_I2C_SCL_PIN);

    MY_LOG_CORE_INFO("Everything is ready!");

    sdEnd(LOG_CORE_FILENAME);
}

void loop()
{
    checkCommand();

    if (runEvery(5000, &s_TimerOffset))
    {
        sdBegin(LOG_CORE_FILENAME);
        sdBegin(LOG_MEASURE_FILENAME);

        takeMeasurements();

        sdEnd(LOG_CORE_FILENAME);
        sdEnd(LOG_MEASURE_FILENAME);
    }
}

void checkCommand()
{
    char **tokens;
    size_t tokensSize = 0;
    consoleCheckInput(&tokens, &tokensSize);

    if (tokensSize > 0)
    {
        char **commandArgs;
        size_t commandArgsSize = 0;
        console_command_t *command = parseCommand(tokens, tokensSize, &commandArgs, &commandArgsSize);

        if (command != NULL)
        {
            executeCommand(command, commandArgs, commandArgsSize);
        }

        if (commandArgsSize > 0)
        {
            free(commandArgs);
        }

        for (int i = 0; i < tokensSize; i++)
        {
            free(tokens[i]);
        }

        free(tokens);
    }
}

void takeMeasurements()
{
    bmp280_data_t bmp280Data;
    bmp280Read(&bmp280Data);

    radio_body_t body = {
        .command = 'M',
        .payloadSize = sizeof(bmp280Data),
    };

    char buffer[body.payloadSize];

    memcpy(buffer, &bmp280Data, body.payloadSize);

    body.payload = buffer;

    radioSendPacket(&s_LoraData, &body);

    MY_LOG_MEASURE_INT(bmp280Data.pressure);
    MY_LOG_MEASURE_FLOAT(bmp280Data.temperature);
    MY_LOG_MEASURE_END();
}