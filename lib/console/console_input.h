#pragma once

#include <stddef.h>

#define CONSOLE_ARGS_MAX_COUNT 8
#define CONSOLE_INPUT_MAX_LENGTH 128

void consoleStart();
void consoleProcessCharacter(int c, char ***tokens_out_ptr, size_t *size_out);
void consoleTokenizeInput(char* input, char*** tokens_out_ptr, size_t* size_out);
void consoleInputAttachToLogger();
void consoleInputAttachToPrinter();
void consoleCheckInput(char ***tokens_out_ptr, size_t *size_out);