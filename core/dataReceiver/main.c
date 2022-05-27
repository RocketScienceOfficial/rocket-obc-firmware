#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "pinout_config.h"
#include "logger.h"
#include "log_printer.h"
#include "commands_printer.h"
#include "console_printer.h"
#include "mg995.h"
#include "console_input.h"
#include "console_commands.h"
#include "default_console_commands.h"
#include "time_tracker.h"
#include "console_colors.h"
#include "my_assert.h"
#include "lora.h"

static lora_data_t s_LoraData;
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
    resetColorsAndEffects();

    attachPrinterToConsoleLog();
    consoleSetUser("receiver");
    consoleStart();

    consoleInputAttachToLogger(myLogGetCommandLogger(), 1);
    attachPrinterToLog();
    attachPrinterToCommandsLog();
    consoleInputAttachToPrinter(myLogGetCommandLogger(), 1);

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

    // mg995_data_t data_1 = {.pin = MG995_PIN_1};
    // mg995_data_t data_2 = {.pin = MG995_PIN_2};

    // mg995Init(&data_1);
    // mg995Init(&data_2);

    MY_LOG_CORE_INFO("Everything is ready!");
}

void loop()
{
    checkCommand();

    if (runEvery(5000, &s_TimerOffset))
    {
        MY_LOG_CORE_INFO("Sending packet...");

        loraBeginPacket(&s_LoraData, 0);
        loraWrite_str(&s_LoraData, "Hello World!");
        loraEndPacket(&s_LoraData, 0);
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