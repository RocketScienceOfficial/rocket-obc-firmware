#include "console/console_input.h"
#include "pico/stdlib.h"
#include "logging/logger.h"
#include "logging/recorder.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int consoleCheckInput()
{
    int ch = getchar_timeout_us(0);

    return ch != PICO_ERROR_TIMEOUT ? ch : 0;
}

bool consoleProcessCharacter(int c, ConsoleInput *input, ConsoleTokens *tokens)
{
    FUNCTION_PROFILE_BEGIN();

    if (!input || !tokens)
    {
        MY_LOG_CORE_ERROR("Invalid input");
        FUNCTION_PROFILE_END();

        return false;
    }

    if (!isprint(c) && !isspace(c))
    {
        FUNCTION_PROFILE_END();

        return false;
    }

    MY_LOG_CORE_INFO("Processing character: %c", c);

    if (c == '\r')
    {
        if (strnlen(input->_cmd, sizeof(input->_cmd)) == 0)
        {
            return true;
        }

        MY_LOG_CORE_INFO("Return pressed!");

        consoleTokenizeInput(input, tokens);
    }
    else
    {
        if (input->_cmdSize < sizeof(input->_cmd) - 1)
        {
            input->_cmd[input->_cmdSize++] = c;
        }
        else
        {
            MY_LOG_CORE_ERROR("Command too long!");
        }
    }

    FUNCTION_PROFILE_END();

    return true;
}

bool consoleTokenizeInput(ConsoleInput *input, ConsoleTokens *tokens)
{
    FUNCTION_PROFILE_BEGIN();

    if (!input || !tokens)
    {
        MY_LOG_CORE_ERROR("Invalid input");
        FUNCTION_PROFILE_END();

        return false;
    }

    MY_LOG_CORE_INFO("Tokenizing input...");

    tokens->tokens = (char **)malloc(CONSOLE_ARGS_MAX_COUNT * sizeof(char *));
    tokens->size = 0;

    char *cmdn = strtok(input->_cmd, " ");

    while (cmdn != NULL)
    {
        char *arg = (char *)malloc((strlen(cmdn) + 1) * sizeof(char));

        strcpy(arg, cmdn);

        tokens->tokens[tokens->size] = arg;

        cmdn = strtok(NULL, " ");
        tokens->size++;
    }

    MY_LOG_CORE_INFO("Input tokenized! Length: %d", tokens->size);

    FUNCTION_PROFILE_END();

    return true;
}

bool consoleClear(ConsoleInput *input, ConsoleTokens *tokens)
{
    FUNCTION_PROFILE_BEGIN();

    if (!input || !tokens)
    {
        MY_LOG_CORE_ERROR("Invalid input");
        FUNCTION_PROFILE_END();

        return false;
    }

    input->_cmdSize = 0;
    memset(input->_cmd, 0, sizeof(input->_cmd));

    if (tokens->size > 0)
    {
        for (size_t i = 0; i < tokens->size; i++)
        {
            free(tokens->tokens[i]);
        }

        free(tokens->tokens);

        tokens->size = 0;
    }

    FUNCTION_PROFILE_END();

    return true;
}