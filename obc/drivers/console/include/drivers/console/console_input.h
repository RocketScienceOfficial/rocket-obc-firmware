#pragma once

#include <obc/api.h>

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
    CHAR _cmd[CONSOLE_INPUT_MAX_LENGTH];

    /**
     * @brief Current characters length
     */
    SIZE _cmdSize;
} ConsoleInput;

/**
 * @brief Console tokens structure.
 */
typedef struct ConsoleTokens
{
    /**
     * @brief Two-dimensional array of tokens.
     */
    STRING *tokens;

    /**
     * @brief Number of tokens.
     */
    SIZE size;
} ConsoleTokens;

/**
 * @brief Checks if the input is available.
 *
 * @return ASCII Character if available, 0 otherwise.
 */
INT32 consoleCheckInput();

/**
 * @brief Processess the character.
 *
 * @param c Character to process.
 * @param input Console input structure to setup.
 * @param input Console tokens structure to setup.
 *
 * @return Result code.
 */
FUNCRESULT consoleInputProcessCharacter(INT32 c, ConsoleInput *input, ConsoleTokens *tokens);

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
FUNCRESULT consoleInputClear(ConsoleInput *input, ConsoleTokens *tokens);