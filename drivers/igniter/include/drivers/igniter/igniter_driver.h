#pragma once

#include <obc/api.h>
#include "drivers/gpio/gpio_driver.h"

/**
 * @brief Activates igniter
 *
 * @param pin Pin number of igniter
 * @param delay Delay in ms
 * @return Result code
 */
FUNCRESULT igniterActivate(PinNumber pin, UINT32 delay);