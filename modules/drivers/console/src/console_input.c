#include "drivers/console/console_input.h"
#include "pico/stdlib.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>

FUNCRESULT consoleCheckInput(BOOL *pAvailable, INT32 *chr)
{
    *chr = getchar_timeout_us(0);
    *pAvailable = *chr != PICO_ERROR_TIMEOUT;

    return SUC_OK;
}

FUNCRESULT consoleInputProcessCharacter(INT32 c, ConsoleInput *input, BOOL *pTokensReady)
{
    if (!input)
    {
        return ERR_INVALIDARG;
    }

    if (!isprint(c) && !isspace(c))
    {
        return SUC_OK;
    }

    *pTokensReady = FALSE;

    if (c == '\r')
    {
        if (strnlen(input->cmd, sizeof(input->cmd)) == 0)
        {
            return SUC_OK;
        }
        else
        {
            if (__consoleTokenizeInput(input))
            {
                *pTokensReady = TRUE;

                memset(input->cmd, 0, sizeof(input->cmd));
                input->cmdSize = 0;

                return SUC_OK;
            }
            else
            {
                return ERR_FAIL;
            }
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

BOOL __consoleTokenizeInput(ConsoleInput *input)
{
    if (!input)
    {
        return FALSE;
    }

    input->tokensSize = 0;

    STRING cmdn = strtok(input->cmd, " ");

    while (cmdn != NULL)
    {
        if (input->tokensSize >= CONSOLE_TOKENS_MAX_COUNT)
        {
            return FALSE;
        }

        strcpy(input->tokens[input->tokensSize++], cmdn);

        cmdn = strtok(NULL, " ");
    }

    return TRUE;
}