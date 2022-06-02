#include "console_input.h"
#include "pico/stdlib.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static char s_Cmd[CONSOLE_INPUT_MAX_LENGTH];
static size_t s_Size;
static const char *s_User;

void consoleSetUser(const char *user)
{
    s_User = user;
}

void consoleProcessCharacter(int c, char ***tokens_out_ptr, size_t *size_out)
{
    if (!isprint(c) && !isspace(c))
    {
        return;
    }

    if (c == '\r')
    {
        if (strnlen(s_Cmd, sizeof(s_Cmd)) == 0)
        {
            return;
        }

        consoleTokenizeInput(s_Cmd, tokens_out_ptr, size_out);

        s_Size = 0;
        memset(s_Cmd, 0, sizeof(s_Cmd));
    }
    else
    {
        if (s_Size < sizeof(s_Cmd) - 1)
        {
            s_Cmd[s_Size] = c;
            s_Size++;
        }
    }
}

void consoleTokenizeInput(char *input, char ***tokens_out_ptr, size_t *size_out)
{
    *tokens_out_ptr = (char **)malloc(CONSOLE_ARGS_MAX_COUNT * sizeof(char *));
    *size_out = 0;

    char *cmdn = strtok(input, " ");

    while (cmdn != NULL)
    {
        char *arg = malloc((strlen(cmdn) + 1) * sizeof(char));

        strcpy(arg, cmdn);

        (*tokens_out_ptr)[*size_out] = arg;

        cmdn = strtok(NULL, " ");
        (*size_out)++;
    }
}

void consoleCheckInput(char ***tokens_out_ptr, size_t *size_out)
{
    int ch = getchar_timeout_us(100);

    if (ch != PICO_ERROR_TIMEOUT)
    {
        consoleProcessCharacter(ch, tokens_out_ptr, size_out);
    }
}