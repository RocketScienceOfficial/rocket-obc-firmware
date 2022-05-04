#include "console_commands.h"
#include <stdlib.h>
#include <string.h>

static console_command_t *s_commands;
static size_t s_commandSize;

void registerCommand(console_command_t *command)
{
    console_command_t *oldCommands = s_commands;
    int oldSize = s_commandSize;

    s_commands = malloc(sizeof(console_command_t) * (oldSize + 1));

    for (int i = 0; i < oldSize; i++)
    {
        s_commands[i] = oldCommands[i];
    }

    s_commands[oldSize] = *command;
    s_commandSize++;

    free(oldCommands);
}

console_command_t *parseCommand(char **tokens, size_t tokensSize, char ***commandArgs_out_ptr, size_t *commandArgsSize_out)
{
    console_command_t *command = NULL;

    for (size_t i = 0; i < s_commandSize; i++)
    {
        if (strcmp(s_commands[i].name, tokens[0]) == 0)
        {
            command = &s_commands[i];
        }
    }

    if (command)
    {
        *commandArgsSize_out = tokensSize - 1;

        if (*commandArgsSize_out > 0)
        {
            *commandArgs_out_ptr = (char **)malloc((*commandArgsSize_out) * sizeof(char *));

            for (size_t i = 1; i < tokensSize; i++)
            {
                (*commandArgs_out_ptr)[i - 1] = tokens[i];
            }
        }

        return command;
    }
    else
    {
        *commandArgsSize_out = 0;

        return NULL;
    }
}

void executeCommand(console_command_t *command, char **args, size_t argc)
{
    command->func(args, argc);
}

int checkArgsCount(size_t expectedCount, size_t actualCount, char **output_ptr)
{
    if (actualCount != expectedCount)
    {
        *output_ptr = CONSOLE_ARGUMENTS_COUNT_ERROR_MSG;

        return 0;
    }

    return 1;
}