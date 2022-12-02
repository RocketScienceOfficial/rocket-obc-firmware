#pragma once

#include <obc/api.h>
#include "drivers/gpio/gpio_driver.h"

/**
 * @brief Initialize LED on pin.
 *
 * @param pin Pin to initialize.
 * @return Result Code.
 */
FUNCRESULT ledInit(PinNumber pin);

/**
 * @brief Set LED on or off.
 *
 * @param pin Pin to change state for.
 * @param state State of pin - on or off.
 * @return Result Code.
 */
FUNCRESULT setLedState(PinNumber pin, BOOL state);