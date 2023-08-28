#pragma once

#include <obc/api.h>
#include "drivers/gpio/gpio_driver.h"

#define IGNITERS_MAX_COUNT 8

/**
 * @brief Igniter data
 */
typedef struct IgniterData
{
    PinNumber pins[IGNITERS_MAX_COUNT];
    SIZE count;
    UINT32 delay;
} IgniterData;

/**
 * @brief Initialize igniter system
 *
 * @param data Igniter data
 * @return Result code
 */
FUNCRESULT ignInit(IgniterData *data);

/**
 * @brief Fires igniters
 *
 * @param data Igniter data
 * @return Result code
 */
FUNCRESULT ignFire(IgniterData *data);

/**
 * @brief Updates igniter system
 *
 * @param data Igniter data
 * @return Result code
 */
FUNCRESULT ignUpdate(IgniterData *data);