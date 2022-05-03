#pragma once

#define CONSOLE_COMMAND_MAX_LENGTH 128

typedef void (*consoleCommandFunction_t)(const char **args, int argc);

typedef struct console_command
{
    char *name;
    consoleCommandFunction_t func;
} console_command_t;

void parseCommand(const char *input, console_command_t *output);
void executeCommand(console_command_t *command, const char **args, int argc);