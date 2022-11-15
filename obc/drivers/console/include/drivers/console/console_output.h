#pragma once

#include "tools/typedefs.h"

#define DEFAULT_LOG_SERIAL_PATTERN "[%t %l %M:%S:%m] %c\n"

/**
 * @brief Logs normal message to serial output.
 *
 * @param format Format.
 * @param ... Arguments
 */
VOID consoleLog(const STRING format, ...);

/**
 * @brief Logs error message to serial output.
 *
 * @param format Format.
 * @param ... Arguments
 */
VOID consoleLogError(const STRING format, ...);