#include "console_input.h"
#include "logger.h"
#include "pico/stdlib.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static char cmd[CONSOLE_INPUT_MAX_LENGTH];
static size_t size;

void consoleStart()
{
    myLogConsole("> ");
    stdio_flush();
}

void consoleProcessCharacter(int c, char ***tokens_out_ptr, size_t *size_out)
{
    if (!isprint(c) && !isspace(c) && c != '\r' && c != '\b' && c != (char)127)
    {
        return;
    }

    myLogConsole("%c", c);
    stdio_flush();

    if (c == '\r')
    {
        if (strnlen(cmd, sizeof(cmd)) == 0)
        {
            myLogConsole("\n> ");
            stdio_flush();

            return;
        }

        consoleTokenizeInput(cmd, tokens_out_ptr, size_out);

        size = 0;
        memset(cmd, 0, sizeof(cmd));

        myLogConsole("\n> ");
        stdio_flush();
    }
    else
    {
        if (c == '\b' || c == (char)127)
        {
            if (size > 0)
            {
                size--;
                cmd[size] = '\0';
            }
        }
        else
        {
            if (size < sizeof(cmd) - 1)
            {
                cmd[size] = c;
                size++;
            }
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

static void logCallback(void **data, size_t size)
{
    const char *level = (const char *)data[1];

    if (level != LOG_LEVEL_CONSOLE)
    {
        const char *removeLineAnsi = "\33[2K\r";

        myLogConsole(removeLineAnsi);
        stdio_flush();
    }
}

void consoleInputAttachToLogger()
{
    myLogAddCallback(logCallback);
}

static void logPrinterCallback(void **data, size_t size)
{
    const char *level = (const char *)data[1];

    if (level != LOG_LEVEL_CONSOLE)
    {
        myLogConsole("> %s", cmd);
        stdio_flush();
    }
}

void consoleInputAttachToPrinter()
{
    myLogAddCallback(logPrinterCallback);
}

void consoleCheckInput(char ***tokens_out_ptr, size_t *size_out)
{
    int ch = getchar_timeout_us(0);

    if (ch != PICO_ERROR_TIMEOUT)
    {
        consoleProcessCharacter(ch, tokens_out_ptr, size_out);
    }
}