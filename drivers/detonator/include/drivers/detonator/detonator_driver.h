#pragma once

#include <obc/api.h>
#include "drivers/gpio/gpio_driver.h"

/**
 * @brief Activates detonator
 *
 * @param pin Pin number of detonator
 * @return Result code
 */
FUNCRESULT detonatorActivate(PinNumber pin);