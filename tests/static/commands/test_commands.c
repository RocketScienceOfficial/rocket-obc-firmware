#include "test_commands.h"
#include "console_commands.h"
#include "default_console_commands.h"

MY_TEST_INIT_FUNC(COMMANDS_TEST_NAME)
{
    registerDefaultConsoleCommands();

    MY_TEST_END();
}

MY_TEST_FUNC(COMMANDS_TEST_NAME, 1)
{
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

    MY_TEST_END();
}