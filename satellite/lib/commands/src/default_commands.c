#include "commands/default_commands.h"
#include "commands/commands.h"
#include "logging/logger.h"
#include "logging/recorder.h"

static CommandData s_Commands[] = {
    {.name = "hello", .func = &helloCommand},
    {.name = "hello_name", .func = &helloNameCommand},
};

void helloCommand(char **args, size_t argc)
{
    FUNCTION_PROFILE_BEGIN();

    MY_LOG_CORE_INFO("Hello");

    FUNCTION_PROFILE_END();
}

void helloNameCommand(char **args, size_t argc)
{
    FUNCTION_PROFILE_BEGIN();

    char *error;

    if (checkArgsCount(1, argc, &error))
    {
        MY_LOG_CORE_INFO("Hello %s", args[0]);
    }
    else
    {
        MY_LOG_CORE_INFO(error);
    }

    FUNCTION_PROFILE_END();
}

size_t defaultCommandsGetCount()
{
    return sizeof(s_Commands) / sizeof(CommandData);
}

bool registerDefaultConsoleCommands()
{
    FUNCTION_PROFILE_BEGIN();

    size_t count = sizeof(s_Commands) / sizeof(CommandData);

    for (size_t i = 0; i < count; i++)
    {
        if (!registerCommand(&s_Commands[i]))
        {
            return false;
        }
    }

    MY_LOG_CORE_INFO("Registered %d console commands", count);

    FUNCTION_PROFILE_END();

    return true;
}