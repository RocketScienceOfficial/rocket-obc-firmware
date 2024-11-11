#ifndef _SERIAL_DRIVER_H
#define _SERIAL_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Prints message to serial
 *
 * @param fmt Format
 * @param ... Params
 */
void hal_serial_printf(char *fmt, ...);

/**
 * @brief Sends byte buffer to serial
 *
 * @param buffer Buffer of bytes to send
 * @param len Length of buffer
 */
void hal_serial_send_buffer(uint8_t *buffer, int len);

/**
 * @brief Read char from serial
 *
 * @param chr Char that was read
 * @return True if char was available
 */
bool hal_serial_read_char(int *chr);

#endif