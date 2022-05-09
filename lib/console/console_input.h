#pragma once

#include <stddef.h>
#include "logger.h"

#define CONSOLE_ARGS_MAX_COUNT 8
#define CONSOLE_INPUT_MAX_LENGTH 128

void consoleStart();
void consoleSetUser(const char *user);
void consoleProcessCharacter(int c, char ***tokens_out_ptr, size_t *size_out);
void consoleTokenizeInput(char *input, char ***tokens_out_ptr, size_t *size_out);
void consoleInputAttachToLogger(logger_data_t *otherLoggers, size_t otherLoggersCount);
void consoleInputAttachToPrinter(logger_data_t *otherLoggers, size_t otherLoggersCount);
void consoleCheckInput(char ***tokens_out_ptr, size_t *size_out);
logger_data_t *myLogGetConsoleLogger();

#define MY_LOG_CONSOLE_PATTERN "%c"
#define MY_LOG_CONSOLE(msg, ...) myLog(myLogGetConsoleLogger(), "", msg, ##__VA_ARGS__)