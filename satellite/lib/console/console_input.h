#pragma once

#include <stddef.h>
#include "logger.h"

#define CONSOLE_ARGS_MAX_COUNT 8
#define CONSOLE_INPUT_MAX_LENGTH 128

/**
 * @brief Console input structure. All changes are done internally, so keep it just for reference!
 */
typedef struct ConsoleInput
{
    /**
     * @brief Two-dimensional array of tokens.
     */
    char **tokens;

    /**
     * @brief Number of tokens.
     */
    size_t size;

    /**
     * @brief Current characters
     */
    char _cmd[CONSOLE_INPUT_MAX_LENGTH];

    /**
     * @brief Current characters length
     */
    size_t _cmdSize;
} ConsoleInput;

/**
 * @brief Checks if the input is available.
 *
 * @return ASCII Character if available, 0 otherwise.
 */
int consoleCheckInput();

/**
 * @brief Processess the character.
 *
 * @param c Character to process.
 * @param input_out Console input structure to setup.
 */
void consoleProcessCharacter(int c, ConsoleInput *input_out);

/**
 * @brief Tokenizes the input.
 *
 * @param input Input to tokenize.
 * @param input_out Console input structure to setup.
 */
void consoleTokenizeInput(char *input, ConsoleInput *input_out);

/**
 * @brief Clears the input.
 *
 * @param input Console input structure to clear.
 */
void consoleClearInput(ConsoleInput *input);