#include "drivers/console/console_input.h"
#include "pico/stdlib.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

BOOL consoleCheckInput(INT32 *chr)
{
    *chr = getchar_timeout_us(0);

    return *chr != PICO_ERROR_TIMEOUT;
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
        if (strnlen(input->cmd, sizeof(input->cmd)) == 0)
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
        if (input->cmdSize < sizeof(input->cmd) - 1)
        {
            input->cmd[input->cmdSize++] = c;

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

    STRING cmdn = strtok(input->cmd, " ");

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

    input->cmdSize = 0;
    memset(input->cmd, 0, sizeof(input->cmd));

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