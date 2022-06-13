#pragma once

#include <stddef.h>
#include "logger.h"

#define CONSOLE_ARGS_MAX_COUNT 8
#define CONSOLE_INPUT_MAX_LENGTH 128

typedef struct console_input 
{
    char **tokens;
    size_t size;
} console_input_t;

void consoleCheckInput(console_input_t *input_out);
void consoleProcessCharacter(int c, console_input_t *input_out);
void consoleTokenizeInput(char *input, console_input_t *input_out);
void consoleClearInput(console_input_t *input);