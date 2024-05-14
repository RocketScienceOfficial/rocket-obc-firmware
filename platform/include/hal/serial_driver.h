#ifndef _SERIAL_DRIVER_H
#define _SERIAL_DRIVER_H

#include <stdbool.h>

/**
 * @brief Prints message to serial
 * 
 * @param fmt Format
 * @param ... Params
*/
void hal_serial_printf(char* fmt, ...);

/**
 * @brief Read char from serial
 * 
 * @param chr Char that was read
 * @return True if char was available
*/
bool hal_serial_read_char(int *chr);

#endif