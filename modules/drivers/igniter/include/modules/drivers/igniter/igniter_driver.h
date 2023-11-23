#ifndef _IGNITER_DRIVER_H
#define _IGNITER_DRIVER_H

#include "modules/drivers/hal/gpio_driver.h"
#include "modules/drivers/hal/time_tracker.h"
#include <stddef.h>

#define IGNITERS_MAX_COUNT 8

/**
 * @brief Igniter data
 */
typedef struct igniter_data
{
    hal_pin_number_t pins[IGNITERS_MAX_COUNT]; /** Pins of igniters */
    size_t count;                              /** Count of igniters */
    hal_time_t delay;                          /** Delay of each pin to trigger */
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