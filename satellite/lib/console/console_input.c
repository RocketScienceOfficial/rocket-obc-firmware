#include "console_input.h"
#include "pico/stdlib.h"
#include "logger.h"
#include "recorder.h"
#include "error_handling.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int consoleCheckInput()
{
    int ch = getchar_timeout_us(0);

    return ch != PICO_ERROR_TIMEOUT ? ch : 0;
}

void consoleProcessCharacter(int c, ConsoleInput *input_out)
{
    FUNCTION_PROFILE_BEGIN();

    if (!input_out)
    {
        MY_LOG_CORE_ERROR("Invalid input");
        FUNCTION_PROFILE_END();

        return;
    }

    if (!isprint(c) && !isspace(c))
    {
        FUNCTION_PROFILE_END();

        return;
    }

    MY_LOG_CORE_INFO("Processing character: %c", c);

    if (c == '\r')
    {
        if (strnlen(input_out->_cmd, sizeof(input_out->_cmd)) == 0)
        {
            return;
        }

        MY_LOG_CORE_INFO("Return pressed!");

        consoleTokenizeInput(input_out->_cmd, input_out);

        input_out->_cmdSize = 0;
        memset(input_out->_cmd, 0, sizeof(input_out->_cmd));
    }
    else
    {
        if (input_out->_cmdSize < sizeof(input_out->_cmd) - 1)
        {
            input_out->_cmd[input_out->_cmdSize++] = c;
        }
    }

    FUNCTION_PROFILE_END();
}

void consoleTokenizeInput(char *input, ConsoleInput *input_out)
{
    FUNCTION_PROFILE_BEGIN();

    if (!input || !input_out)
    {
        MY_LOG_CORE_ERROR("Invalid input");
        FUNCTION_PROFILE_END();

        return;
    }

    MY_LOG_CORE_INFO("Tokenizing input...");

    input_out->tokens = (char **)malloc(CONSOLE_ARGS_MAX_COUNT * sizeof(char *));
    input_out->size = 0;

    char *cmdn = strtok(input, " ");

    while (cmdn != NULL)
    {
        char *arg = (char *)malloc((strlen(cmdn) + 1) * sizeof(char));

        strcpy(arg, cmdn);

        input_out->tokens[input_out->size] = arg;

        cmdn = strtok(NULL, " ");
        input_out->size++;
    }

    MY_LOG_CORE_INFO("Input tokenized! Length: %d", input_out->size);

    FUNCTION_PROFILE_END();
}

void consoleClearInput(ConsoleInput *input)
{
    FUNCTION_PROFILE_BEGIN();

    if (!input)
    {
        MY_LOG_CORE_ERROR("Invalid input");
        FUNCTION_PROFILE_END();

        return;
    }

    if (input->size > 0)
    {
        for (size_t i = 0; i < input->size; i++)
        {
            free(input->tokens[i]);
        }

        free(input->tokens);

        input->size = 0;
    }

    FUNCTION_PROFILE_END();
}