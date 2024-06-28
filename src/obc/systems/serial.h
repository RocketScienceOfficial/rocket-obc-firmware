#ifndef _SERIAL_H
#define _SERIAL_H

#include <stddef.h>

/**
 * @brief Initialize serial input system
 */
void serial_init(void);

/**
 * @brief Update serial input system, gather data
 */
void serial_update(void);

/**
 * @brief Get param of current cmd at index. If index is outside of the bounds, result is NULL
 *
 * @param index Index of param
 * @return Param
 */
const char *serial_get_param_at_index(size_t index);

/**
 * @brief Get current cmd params count
 *
 * @return Count
 */
size_t serial_get_params_count(void);

#endif