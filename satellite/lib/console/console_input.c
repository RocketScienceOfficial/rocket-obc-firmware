#include "console_input.h"
#include "pico/stdlib.h"
#include "logger.h"
#include "recorder.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static char s_Cmd[CONSOLE_INPUT_MAX_LENGTH];
static size_t s_Size;

int consoleCheckInput()
{
    int ch = getchar_timeout_us(0);

    return ch != PICO_ERROR_TIMEOUT ? ch : 0;
}

void consoleGetInput(int chr, console_input_t *input_out)
{
    FUNCTION_PROFILE_BEGIN();

    MY_LOG_CORE_INFO("Character received: %d", chr);

    consoleProcessCharacter(chr, input_out);

    FUNCTION_PROFILE_END();
}

void consoleProcessCharacter(int c, console_input_t *input_out)
{
    FUNCTION_PROFILE_BEGIN();

    if (!isprint(c) && !isspace(c))
    {
        FUNCTION_PROFILE_END();

        return;
    }

    if (c == '\r')
    {
        if (strnlen(s_Cmd, sizeof(s_Cmd)) == 0)
        {
            return;
        }

        MY_LOG_CORE_INFO("Return pressed!");

        consoleTokenizeInput(s_Cmd, input_out);

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

    FUNCTION_PROFILE_END();
}

void consoleTokenizeInput(char *input, console_input_t *input_out)
{
    FUNCTION_PROFILE_BEGIN();

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

void consoleClearInput(console_input_t *input)
{
    FUNCTION_PROFILE_BEGIN();

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