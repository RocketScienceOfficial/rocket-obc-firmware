#pragma once

#include <obc/api.h>

/**
 * @brief Initialize commands module
 */
VOID initializeCommands();

/**
 * @brief Check if there is input from console
 */
BOOL consoleAvailable();

/**
 * @brief Read input from console, check if command is available and execute if it is
 */
VOID checkCommand();