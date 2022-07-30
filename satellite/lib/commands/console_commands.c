#include "console_commands.h"
#include "logger.h"
#include "recorder.h"
#include <stdlib.h>
#include <string.h>

static console_command_t s_Commands[CONSOLE_MAX_COMMANDS];
static size_t s_CommandSize;

void registerCommand(console_command_t *command)
{
    FUNCTION_PROFILE_BEGIN();

    s_Commands[s_CommandSize] = *command;
    s_CommandSize++;

    FUNCTION_PROFILE_END();
}

console_command_t *parseCommand(char **tokens, size_t tokensSize, command_args_t *args_out_ptr)
{
    FUNCTION_PROFILE_BEGIN();

    MY_LOG_CORE_INFO("Parsing command...");

    console_command_t *command = NULL;

    for (size_t i = 0; i < s_CommandSize; i++)
    {
        if (strcmp(s_Commands[i].name, tokens[0]) == 0)
        {
            command = &s_Commands[i];
            break;
        }
    }

    if (command)
    {
        MY_LOG_CORE_INFO("Command found: %s", command->name);

        args_out_ptr->size = tokensSize - 1;

        if (args_out_ptr->size > 0)
        {
            args_out_ptr->args = (char **)malloc(args_out_ptr->size * sizeof(char *));

            for (size_t i = 1; i < tokensSize; i++)
            {
                args_out_ptr->args[i - 1] = (char *)malloc((strlen(tokens[i]) + 1) * sizeof(char));

                strcpy(args_out_ptr->args[i - 1], tokens[i]);
            }
        }

        MY_LOG_CORE_INFO("Command arguments size: %d", args_out_ptr->size);

        FUNCTION_PROFILE_END();

        return command;
    }
    else
    {
        MY_LOG_CORE_ERROR("Command not found!");

        args_out_ptr->size = 0;

        FUNCTION_PROFILE_END();

        return NULL;
    }
}

void executeCommand(console_command_t *command, command_args_t *args)
{
    FUNCTION_PROFILE_BEGIN();

    MY_LOG_CORE_INFO("Executing command: %s", command->name);

    command->func(args->args, args->size);

    MY_LOG_CORE_INFO("Command executed: %s", command->name);

    FUNCTION_PROFILE_END();
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

void commandClearArgs(command_args_t *args)
{
    FUNCTION_PROFILE_BEGIN();

    if (args->size > 0)
    {
        for (size_t i = 0; i < args->size; i++)
        {
            free(args->args[i]);
        }

        free(args->args);
        
        args->size = 0;
    }

    FUNCTION_PROFILE_END();
}

static logger_data_t s_Logger;
static int s_LoggerInitialized;

logger_data_t *myLogGetCommandOutputLogger()
{
    if (!s_LoggerInitialized)
    {
        myLogCreateLogger(&s_Logger, MY_LOG_COMMAND_OUTUT_NAME);

        s_LoggerInitialized = 1;
    }

    return &s_Logger;
}