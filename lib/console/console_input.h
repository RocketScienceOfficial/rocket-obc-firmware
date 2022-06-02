#pragma once

#include <stddef.h>
#include "logger.h"

#define CONSOLE_ARGS_MAX_COUNT 8
#define CONSOLE_INPUT_MAX_LENGTH 128

void consoleSetUser(const char *user);
void consoleProcessCharacter(int c, char ***tokens_out_ptr, size_t *size_out);
void consoleTokenizeInput(char *input, char ***tokens_out_ptr, size_t *size_out);
void consoleCheckInput(char ***tokens_out_ptr, size_t *size_out);