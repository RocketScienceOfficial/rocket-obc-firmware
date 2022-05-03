#include "console_input.h"
#include "pico/stdlib.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>

void consoleProcessCharacter(int c)
{
    static char cmd[CONSOLE_COMMAND_MAX_LENGTH];
    static size_t size;

    if (!isprint(c) && !isspace(c) && c != '\r' && c != '\b' && c != (char)127)
    {
        return;
    }

    printf("%c", c);
    stdio_flush();

    if (c == '\r')
    {
        printf("%c", '\n');
        stdio_flush();

        if (!strnlen(cmd, sizeof cmd))
        {
            printf("> ");
            stdio_flush();

            return;
        }

        printf("\n> ");
        stdio_flush();
    }
    else
    {
    }
}

void consoleInputAttachToLogger()
{
}

void consoleCheckInput()
{
    int ch = getchar_timeout_us(0);

    if (ch != PICO_ERROR_TIMEOUT)
    {
        consoleProcessCharacter(ch);
    }
}