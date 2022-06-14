#include "default_console_commands.h"
#include "console_commands.h"
#include "logger.h"

static console_command_t s_Commands[] = {
    {.name = "hello", .func = helloCommand},
    {.name = "hello_name", .func = helloNameCommand},
};

void helloCommand(char **args, size_t argc)
{
    MY_LOG_COMMAND_OUTPUT("Hello");
}

void helloNameCommand(char **args, size_t argc)
{
    char *error;

    if (checkArgsCount(1, argc, &error))
    {
        MY_LOG_COMMAND_OUTPUT("Hello %s", args[0]);
    }
    else
    {
        MY_LOG_COMMAND_OUTPUT(error);
    }
}

void registerDefaultConsoleCommands()
{
    size_t count = sizeof(s_Commands) / sizeof(console_command_t);

    for (size_t i = 0; i < count; i++)
    {
        registerCommand(&s_Commands[i]);
    }

    MY_LOG_CORE_INFO("Registered %d console commands", count);
}