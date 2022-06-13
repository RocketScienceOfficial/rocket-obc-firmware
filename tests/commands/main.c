#include "pico/stdlib.h"
#include <stdlib.h>
#include "test_framework.h"
#include "logger.h"
#include "log_serial.h"
#include "console_commands.h"
#include "default_console_commands.h"
#include "remote_commands.h"

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
    attachSerialToLog(myLogGetCommandOutputLogger());

    MY_LOG_CORE_INFO("Initializing...");

    registerDefaultConsoleCommands();

    char *tokens[] = {
        "hello",
    };
    size_t tokensSize = 1;
    command_args_t args = {0};
    console_command_t *command = parseCommand(tokens, tokensSize, &args);

    if (command != NULL)
    {
        executeCommand(command, &args);
    }

    commandClearArgs(&args);

    MY_LOG_CORE_INFO("Done!");
}

void loop()
{
    tight_loop_contents();
}