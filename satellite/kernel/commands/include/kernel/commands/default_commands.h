#pragma once

#include <stdbool.h>
#include <stddef.h>

void helloCommand(char **args, size_t argc);
void helloNameCommand(char **args, size_t argc);

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