#pragma once

#include <obc/api.h>

#define CONSOLE_ARGS_MAX_COUNT 8
#define CONSOLE_INPUT_MAX_LENGTH 128

/**
 * @brief Console input structure. All changes are done internally, so keep it just for reference!
 */
typedef struct ConsoleInput
{
    CHAR cmd[CONSOLE_INPUT_MAX_LENGTH]; /** Current characters. */
    SIZE cmdSize;                       /** Current characters length. */
} ConsoleInput;

/**
 * @brief Console tokens structure.
 */
typedef struct ConsoleTokens
{
    STRING *tokens; /** Two-dimensional array of tokens. */
    SIZE size;      /** Number of tokens. */
} ConsoleTokens;

/**
 * @brief Checks if the input is available.
 *
 * @param chr Pointer to the character to store the result in.
 *
 * @return True if available.
 */
BOOL consoleCheckInput(INT32 *chr);

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