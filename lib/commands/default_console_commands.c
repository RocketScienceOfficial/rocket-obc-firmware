#include "default_console_commands.h"
#include "console_commands.h"
#include "logger.h"

console_command_t commands[] = {
    {.name = "hello", .func = helloCommand},
    {.name = "hello_name", .func = helloNameCommand},
};

void helloCommand(char **args, size_t argc)
{
    myLogRaw("Hello");
}

void helloNameCommand(char **args, size_t argc)
{
    char *error;

    if (checkArgsCount(1, argc, &error))
    {
        myLogRaw("Hello %s", args[0]);
    }
    else
    {
        myLogRaw(error);
    }
}

void registerDefaultConsoleCommands()
{
    size_t count = sizeof(commands) / sizeof(console_command_t);

    for (size_t i = 0; i < count; i++)
    {
        registerCommand(&commands[i]);
    }
}