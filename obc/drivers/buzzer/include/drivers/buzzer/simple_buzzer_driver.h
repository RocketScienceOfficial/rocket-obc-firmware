#pragma once

#include "drivers/gpio/gpio_driver.h"

/**
 * @brief Initializes the simple buzzer driver
 *
 * @param pin The pin to use for the buzzer
 * @return Result code
 */
FUNCRESULT simpleBuzzerInit(PinNumber pin);

/**
 * @brief Sets the buzzer active or inactive
 *
 * @param pin The pin to use for the buzzer
 * @param active TRUE to set the buzzer active, FALSE to set it inactive
 * @return Result code
 */
FUNCRESULT simpleBuzzerSetActive(PinNumber pin, BOOL active);