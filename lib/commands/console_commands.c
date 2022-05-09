#include "console_commands.h"
#include <stdlib.h>
#include <string.h>

static console_command_t s_Commands[CONSOLE_MAX_COMMANDS];
static size_t s_CommandSize;

void registerCommand(console_command_t *command)
{
    s_Commands[s_CommandSize] = *command;
    s_CommandSize++;
}

console_command_t *parseCommand(char **tokens, size_t tokensSize, char ***commandArgs_out_ptr, size_t *commandArgsSize_out)
{
    console_command_t *command = NULL;

    for (size_t i = 0; i < s_CommandSize; i++)
    {
        if (strcmp(s_Commands[i].name, tokens[0]) == 0)
        {
            command = &s_Commands[i];
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

static logger_data_t s_Logger;
static int s_LoggerInitialized;

logger_data_t *myLogGetCommandLogger()
{
	if (!s_LoggerInitialized)
	{
		myLogCreateLogger(&s_Logger);

		s_LoggerInitialized = 1;
	}

	return &s_Logger;
}