#pragma once

#include <obc/api.h>

/**
 * @brief Prints hello message to console
 */
VOID helloCommand(STRING *args, SIZE argc);

/**
 * @brief Prints hello message to console with name
 */
VOID helloNameCommand(STRING *args, SIZE argc);

/**
 * @brief Get size of default commands
 *
 * @return Count of default commands
 */
SIZE defaultCommandsGetCount();

/**
 * @brief Register default console commands
 *
 * @return True if commands were registered successfully, false otherwise
 */
BOOL registerDefaultConsoleCommands();