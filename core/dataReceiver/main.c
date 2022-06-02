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
#include "my_assert.h"
#include "lora.h"
#include "radioProtocol.h"
#include "bmp280.h"

static lora_data_t s_LoraData;
static radio_packet_t s_Packet;
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
    consoleSetUser("receiver");

    attachSerialToLog(myLogGetCoreLogger());
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

    MY_ASSERT(loraBegin(&s_LoraData, SX1278_FREQ_HZ));

    MY_LOG_CORE_INFO("Everything is ready!");
}

void loop()
{
    checkCommand();

    if (runEvery(5000, &s_TimerOffset))
    {
        MY_LOG_CORE_INFO("HELLO!");
    }

    if (receivePacket(&s_LoraData, &s_Packet))
    {
        MY_LOG_CORE_INFO("Packet received!");

        bmp280_data_t bmp280Data;
        memcpy(&bmp280Data, s_Packet.payload, s_Packet.payloadSize);

        MY_LOG_CORE_INFO("Pressure: %d", bmp280Data.pressure);
        MY_LOG_CORE_INFO("Altitude: %f", bmp280GetAltitude(&bmp280Data));
        MY_LOG_CORE_INFO("Temperature: %f", bmp280Data.temperature);
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