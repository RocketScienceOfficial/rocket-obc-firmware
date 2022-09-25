#include "kernel/commands/default_commands.h"
#include "kernel/commands/commands.h"
#include "kernel/logging/logger.h"
#include "kernel/logging/recorder.h"

static CommandData s_Commands[] = {
    {.name = "hello", .func = &helloCommand},
    {.name = "hello_name", .func = &helloNameCommand},
};

void helloCommand(STRING *args, SIZE argc)
{
    FUNCTION_PROFILE_BEGIN();

    MY_LOG_CORE_INFO("Hello");

    FUNCTION_PROFILE_END();
}

void helloNameCommand(STRING *args, SIZE argc)
{
    FUNCTION_PROFILE_BEGIN();

    STRING error;

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

SIZE defaultCommandsGetCount()
{
    return sizeof(s_Commands) / sizeof(CommandData);
}

BOOL registerDefaultConsoleCommands()
{
    FUNCTION_PROFILE_BEGIN();

    SIZE count = sizeof(s_Commands) / sizeof(CommandData);

    for (SIZE i = 0; i < count; i++)
    {
        if (!registerCommand(&s_Commands[i]))
        {
            return FALSE;
        }
    }

    MY_LOG_CORE_INFO("Registered %d console commands", count);

    FUNCTION_PROFILE_END();

    return TRUE;
}