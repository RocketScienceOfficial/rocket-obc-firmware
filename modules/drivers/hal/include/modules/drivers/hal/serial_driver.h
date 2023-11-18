#ifndef _SERIAL_DRIVER_H
#define _SERIAL_DRIVER_H

#include <stdbool.h>

/**
 * @brief Read char from serial
 * 
 * @param chr Char that was read
 * @return True if char was available
*/
bool hal_serial_read_char(int *chr);

#endif