#pragma once

#include <stdbool.h>
#include <stddef.h>

void helloCommand();
void helloNameCommand();

/**
 * @brief Get size of default commands.
 * 
 * @return Count of default commands.
 */
size_t defaultCommandsGetCount();

/**
 * @brief Register default console commands.
 * 
 * @return True if commands were registered successfully, false otherwise.
 */
bool registerDefaultConsoleCommands();