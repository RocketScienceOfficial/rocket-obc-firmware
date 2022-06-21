#include "default_console_commands.h"
#include "console_commands.h"
#include "logger.h"
#include "recorder.h"

static console_command_t s_Commands[] = {
    {.name = "hello", .func = helloCommand},
    {.name = "hello_name", .func = helloNameCommand},
};

void helloCommand(char **args, size_t argc)
{
    FUNCTION_PROFILE_BEGIN();

    MY_LOG_COMMAND_OUTPUT("Hello");

    FUNCTION_PROFILE_END();
}

void helloNameCommand(char **args, size_t argc)
{
    FUNCTION_PROFILE_BEGIN();

    char *error;

    if (checkArgsCount(1, argc, &error))
    {
        MY_LOG_COMMAND_OUTPUT("Hello %s", args[0]);
    }
    else
    {
        MY_LOG_COMMAND_OUTPUT(error);
    }

    FUNCTION_PROFILE_END();
}

void registerDefaultConsoleCommands()
{
    FUNCTION_PROFILE_BEGIN();

    size_t count = sizeof(s_Commands) / sizeof(console_command_t);

    for (size_t i = 0; i < count; i++)
    {
        registerCommand(&s_Commands[i]);
    }

    MY_LOG_CORE_INFO("Registered %d console commands", count);

    FUNCTION_PROFILE_END();
}