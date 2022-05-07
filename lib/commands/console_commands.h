#pragma once

#include <stddef.h>

#define CONSOLE_MAX_COMMANDS 16
#define CONSOLE_ARGUMENTS_COUNT_ERROR_MSG "Invalid arguments count!"

typedef void (*consoleCommandFunction_t)(char **args, size_t argc);

typedef struct console_command
{
    char *name;
    consoleCommandFunction_t func;
} console_command_t;

void registerCommand(console_command_t *command);
console_command_t *parseCommand(char **tokens, size_t tokensSize, char ***commandArgs_out_ptr, size_t *commandArgsSize_out);
void executeCommand(console_command_t *command, char **commandArgs, size_t argc);
int checkArgsCount(size_t expectedCount, size_t actualCount, char** output_ptr);