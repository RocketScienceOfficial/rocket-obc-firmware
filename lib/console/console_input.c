#include "console_input.h"
#include "pico/stdlib.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define CONSOLE_ENTRY_FORMAT "%s> "
#define CONSOLE_ENTRY_FORMAT_NL "\n%s> "

static char s_Cmd[CONSOLE_INPUT_MAX_LENGTH];
static size_t s_Size;
static const char *s_User;

static void logConsoleEntry_nl()
{
    MY_LOG_CONSOLE(CONSOLE_ENTRY_FORMAT_NL, s_User);
}

static void logConsoleEntry()
{
    MY_LOG_CONSOLE(CONSOLE_ENTRY_FORMAT, s_User);
}

void consoleStart()
{
    logConsoleEntry();
}

void consoleSetUser(const char *user)
{
    s_User = user;
}

void consoleProcessCharacter(int c, char ***tokens_out_ptr, size_t *size_out)
{
    if (!isprint(c) && !isspace(c) && c != '\r' && c != '\b' && c != (char)127)
    {
        return;
    }

    if (c == '\r')
    {
        MY_LOG_CONSOLE("%c", c);

        if (strnlen(s_Cmd, sizeof(s_Cmd)) == 0)
        {
            logConsoleEntry_nl();

            return;
        }

        consoleTokenizeInput(s_Cmd, tokens_out_ptr, size_out);

        s_Size = 0;
        memset(s_Cmd, 0, sizeof(s_Cmd));

        logConsoleEntry_nl();
    }
    else
    {
        if (c == '\b' || c == (char)127)
        {
            if (s_Size > 0)
            {
                MY_LOG_CONSOLE("%c", c);

                s_Size--;
                s_Cmd[s_Size] = '\0';
            }
        }
        else
        {
            MY_LOG_CONSOLE("%c", c);

            if (s_Size < sizeof(s_Cmd) - 1)
            {
                s_Cmd[s_Size] = c;
                s_Size++;
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

static void logCallback(const char *level, const char *msg)
{
    //const char *removeLineAnsi = "\33[2K\r";

    //MY_LOG_CONSOLE(removeLineAnsi);
}

void consoleInputAttachToLogger(logger_data_t *otherLoggers, size_t otherLoggersCount)
{
    myLogCreateSink(myLogGetCoreLogger(), &logCallback, "");

    for (size_t i = 0; i < otherLoggersCount; i++)
    {
        myLogCreateSink(&otherLoggers[i], &logCallback, "");
    }
}

static void logPrinterCallback(const char *level, const char *msg)
{
    logConsoleEntry();
    MY_LOG_CONSOLE(s_Cmd);
}

void consoleInputAttachToPrinter(logger_data_t *otherLoggers, size_t otherLoggersCount)
{
    myLogCreateSink(myLogGetCoreLogger(), &logPrinterCallback, "");

    for (size_t i = 0; i < otherLoggersCount; i++)
    {
        myLogCreateSink(&otherLoggers[i], &logPrinterCallback, "");
    }
}

void consoleCheckInput(char ***tokens_out_ptr, size_t *size_out)
{
    int ch = getchar_timeout_us(0);

    if (ch != PICO_ERROR_TIMEOUT)
    {
        consoleProcessCharacter(ch, tokens_out_ptr, size_out);
    }
}

static logger_data_t s_Logger;
static int s_LoggerInitialized;

logger_data_t *myLogGetConsoleLogger()
{
    if (!s_LoggerInitialized)
    {
        myLogCreateLogger(&s_Logger);

        s_LoggerInitialized = 1;
    }

    return &s_Logger;
}