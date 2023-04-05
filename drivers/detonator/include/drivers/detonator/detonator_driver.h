#pragma once

#include <obc/api.h>
#include "drivers/gpio/gpio_driver.h"

/**
 * @brief Activates detonator
 *
 * @param pin Pin number of detonator
 * @param delay Delay in ms
 * @return Result code
 */
FUNCRESULT detonatorActivate(PinNumber pin, UINT32 delay);