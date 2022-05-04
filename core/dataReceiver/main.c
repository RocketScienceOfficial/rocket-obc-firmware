#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "pinout_config.h"
#include "logger.h"
#include "console_log_printer.h"
#include "mg995.h"
#include "console_input.h"
#include "console_commands.h"
#include "default_console_commands.h"
#include "time_tracker.h"
#include "console_colors.h"
#include "my_assert.h"

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
    myLogInit();

    consoleInputAttachToLogger();
    attachPrinterToLog();
    consoleInputAttachToPrinter();

    myLogInfo("Initializing...");

    resetColorsAndEffects();

    registerDefaultConsoleCommands();

    mg995_data_t data_1 = {.pin = MG995_PIN_1};
    mg995_data_t data_2 = {.pin = MG995_PIN_2};

    mg995Init(&data_1);
    mg995Init(&data_2);

    consoleStart();

    myLogInfo("Everything is ready!");
}

void loop()
{
    checkCommand();
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