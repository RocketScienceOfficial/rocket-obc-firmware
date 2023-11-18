#ifndef _IGNITER_DRIVER_H
#define _IGNITER_DRIVER_H

#include "modules/drivers/hal/gpio_driver.h"
#include <stddef.h>

#define IGNITERS_MAX_COUNT 8

/**
 * @brief Igniter data
 */
typedef struct igniter_data
{
    pin_number_t pins[IGNITERS_MAX_COUNT];
    size_t count;
    unsigned int delay;
} igniter_data_t;

/**
 * @brief Initialize igniter system
 *
 * @param data Igniter data
 */
void ign_init(igniter_data_t *data);

/**
 * @brief Fires igniters
 *
 * @param data Igniter data
 */
void ign_fire(igniter_data_t *data);

/**
 * @brief Updates igniter system
 *
 * @param data Igniter data
 */
void ign_update(igniter_data_t *data);

#endif