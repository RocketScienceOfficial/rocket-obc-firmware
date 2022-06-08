#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pinout.h"
#include "logger.h"
#include "log_serial.h"
#include "mg995.h"
#include "console_input.h"
#include "console_commands.h"
#include "remote_commands.h"
#include "default_console_commands.h"
#include "time_tracker.h"
#include "lora.h"
#include "radio_protocol.h"
#include "bmp280.h"

static lora_data_t s_LoraData;
static radio_body_t s_PacketBody;
static unsigned int s_TimerOffset;

void start();
void initialize();
void loop();
void checkCommand();

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
    attachSerialToLog(myLogGetMeasureLogger());
    attachSerialToLog(myLogGetCommandOutputLogger());
    attachSerialToLog(myLogGetRemoteCommandLogger());

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

    loraBegin(&s_LoraData, SX1278_FREQ_HZ);

    MY_LOG_CORE_INFO("Everything is ready!");
}

void loop()
{
    checkCommand();

    if (runEvery(5000, &s_TimerOffset))
    {
        MY_LOG_CORE_INFO("HELLO!");
    }

    if (radioReceivePacket(&s_LoraData, &s_PacketBody))
    {
        MY_LOG_CORE_INFO("Packet received!");

        bmp280_data_t bmp280Data;
        memcpy(&bmp280Data, s_PacketBody.payload, s_PacketBody.payloadSize);

        MY_LOG_MEASURE_RECEIVER_INT("Pressure", bmp280Data.pressure);
        MY_LOG_MEASURE_RECEIVER_FLOAT("Altitude", bmp280GetAltitude(&bmp280Data));
        MY_LOG_MEASURE_RECEIVER_FLOAT("Temperature", bmp280Data.temperature);
        MY_LOG_MEASURE_RECEIVER_INT("RSSI", loraRssi(&s_LoraData));

        free(s_PacketBody.payload);
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