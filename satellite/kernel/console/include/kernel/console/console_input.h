#pragma once

#include "utils/errors_types.h"
#include <stddef.h>
#include <stdbool.h>

#define CONSOLE_ARGS_MAX_COUNT 8
#define CONSOLE_INPUT_MAX_LENGTH 128

/**
 * @brief Console input structure. All changes are done internally, so keep it just for reference!
 */
typedef struct ConsoleInput
{
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
 * @brief Console tokens structure.
 */
typedef struct ConsoleTokens
{
    /**
     * @brief Two-dimensional array of tokens.
     */
    char **tokens;

    /**
     * @brief Number of tokens.
     */
    size_t size;
} ConsoleTokens;

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
 * @param input Console input structure to setup.
 * @param input Console tokens structure to setup.
 *
 * @return Result code.
 */
FUNCRESULT consoleProcessCharacter(int c, ConsoleInput *input, ConsoleTokens *tokens);

/**
 * @brief Tokenizes the input.
 *
 * @param input Input to tokenize.
 * @param tokens Console tokens structure to setup.
 *
 * @return Result code.
 */
FUNCRESULT consoleTokenizeInput(ConsoleInput *input, ConsoleTokens *tokens);

/**
 * @brief Clears the input.
 *
 * @param input Console input structure to clear.
 *
 * @return Result code.
 */
FUNCRESULT consoleClear(ConsoleInput *input, ConsoleTokens *tokens);