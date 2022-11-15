#include "drivers/console/console_input.h"
#include "pico/stdlib.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

INT32 consoleCheckInput()
{
    INT32 ch = getchar_timeout_us(0);

    return ch != PICO_ERROR_TIMEOUT ? ch : 0;
}

FUNCRESULT consoleInputProcessCharacter(INT32 c, ConsoleInput *input, ConsoleTokens *tokens)
{
    if (!input || !tokens)
    {
        return ERR_INVALIDARG;
    }

    if (!isprint(c) && !isspace(c))
    {
        return SUC_OK;
    }

    if (c == '\r')
    {
        if (strnlen(input->_cmd, sizeof(input->_cmd)) == 0)
        {
            return SUC_OK;
        }
        else
        {
            return consoleTokenizeInput(input, tokens);
        }
    }
    else
    {
        if (input->_cmdSize < sizeof(input->_cmd) - 1)
        {
            input->_cmd[input->_cmdSize++] = c;

            return SUC_OK;
        }
        else
        {
            return ERR_FAIL;
        }
    }
}

FUNCRESULT consoleTokenizeInput(ConsoleInput *input, ConsoleTokens *tokens)
{
    if (!input || !tokens)
    {
        return ERR_INVALIDARG;
    }

    tokens->tokens = (STRING *)malloc(CONSOLE_ARGS_MAX_COUNT * sizeof(STRING));
    tokens->size = 0;

    STRING cmdn = strtok(input->_cmd, " ");

    while (cmdn != NULL)
    {
        STRING arg = (STRING)malloc((strlen(cmdn) + 1) * sizeof(CHAR));

        strcpy(arg, cmdn);

        tokens->tokens[tokens->size] = arg;

        cmdn = strtok(NULL, " ");
        tokens->size++;
    }

    return SUC_OK;
}

FUNCRESULT consoleInputClear(ConsoleInput *input, ConsoleTokens *tokens)
{
    if (!input || !tokens)
    {
        return ERR_INVALIDARG;
    }

    input->_cmdSize = 0;
    memset(input->_cmd, 0, sizeof(input->_cmd));

    if (tokens->size > 0)
    {
        for (SIZE i = 0; i < tokens->size; i++)
        {
            free(tokens->tokens[i]);
        }

        free(tokens->tokens);

        tokens->size = 0;
    }

    return SUC_OK;
}