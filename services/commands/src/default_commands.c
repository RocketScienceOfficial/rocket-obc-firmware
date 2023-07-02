#include "services/commands/default_commands.h"
#include "services/commands/commands.h"
#include "services/logging/logger.h"

static CommandData s_Commands[] = {
    {.name = "hello", .func = &helloCommand},
    {.name = "hello_name", .func = &helloNameCommand},
};

VOID helloCommand(STRING *args, SIZE argc)
{
    MY_LOG_CORE_INFO("Hello");
}

VOID helloNameCommand(STRING *args, SIZE argc)
{
    STRING error;

    if (checkArgsCount(1, argc, &error))
    {
        MY_LOG_CORE_INFO("Hello %s", args[0]);
    }
    else
    {
        MY_LOG_CORE_INFO(error);
    }
}

SIZE defaultCommandsGetCount()
{
    return sizeof(s_Commands) / sizeof(CommandData);
}

BOOL registerDefaultConsoleCommands()
{
    SIZE count = sizeof(s_Commands) / sizeof(CommandData);

    for (SIZE i = 0; i < count; i++)
    {
        if (!registerCommand(&s_Commands[i]))
        {
            return FALSE;
        }
    }

    MY_LOG_CORE_INFO("Registered %d console commands", count);

    return TRUE;
}