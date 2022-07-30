#pragma once

#include <stddef.h>
#include "logger.h"

#define CONSOLE_MAX_COMMANDS 16
#define CONSOLE_ARGUMENTS_COUNT_ERROR_MSG "Invalid arguments count!"

typedef void (*consoleCommandFunction_t)(char **args, size_t argc);

typedef struct console_command
{
    char *name;
    consoleCommandFunction_t func;
} console_command_t;

typedef struct command_args
{
    char **args;
    size_t size;
} command_args_t;

void registerCommand(console_command_t *command);
console_command_t *parseCommand(char **tokens, size_t tokensSize, command_args_t *args_out_ptr);
void executeCommand(console_command_t *command, command_args_t *args);
int checkArgsCount(size_t expectedCount, size_t actualCount, char **output_ptr);
void commandClearArgs(command_args_t *args);
logger_data_t *myLogGetCommandOutputLogger();

#define MY_LOG_COMMAND_OUTUT_NAME "COMMAND_OUTPUT"
#define MY_LOG_COMMAND_OUTPUT(msg, ...) myLog(myLogGetCommandOutputLogger(), "", msg, ##__VA_ARGS__)