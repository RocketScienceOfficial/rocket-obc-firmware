#pragma once

#define DEFAULT_LOG_SERIAL_PATTERN "[%t %l %M:%S:%m] %c\n"

/**
 * @brief Logs normal message to serial output.
 *
 * @param format Format.
 * @param ... Arguments
 */
void logSerial(const char *format, ...);

/**
 * @brief Logs error message to serial output.
 *
 * @param format Format.
 * @param ... Arguments
 */
void logSerialError(const char *format, ...);