#pragma once

#include <obc/api.h>

#define CONSOLE_INPUT_MAX_LENGTH 128
#define CONSOLE_TOKENS_MAX_COUNT 8
#define CONSOLE_TOKEN_MAX_LENGTH 32

/**
 * @brief Console input structure. All changes are done internally, so keep it just for reference!
 */
typedef struct ConsoleInput
{
    CHAR cmd[CONSOLE_INPUT_MAX_LENGTH];                              /** Current characters */
    SIZE cmdSize;                                                    /** Current characters length */
    CHAR tokens[CONSOLE_TOKENS_MAX_COUNT][CONSOLE_TOKEN_MAX_LENGTH]; /** Two-dimensional array of tokens */
    SIZE tokensSize;                                                 /** Number of tokens */
} ConsoleInput;

/**
 * @brief Checks if the input is available
 *
 * @param pAvailable Pointer to the result whether the input is available
 * @param chr Pointer to the character to store the result in
 *
 * @return True if available
 */
FUNCRESULT consoleCheckInput(BOOL *pAvailable, INT32 *chr);

/**
 * @brief Processess the character
 *
 * @param c Character to process
 * @param input Console input structure to setup
 * @param pTokensReady Have tokens been parsed
 *
 * @return Result code
 */
FUNCRESULT consoleInputProcessCharacter(INT32 c, ConsoleInput *input, BOOL *pTokensReady);

/**
 * @brief Tokenizes the input
 *
 * @param input Input to tokenize
 *
 * @return Result code
 */
BOOL __consoleTokenizeInput(ConsoleInput *input);