#pragma once

#include "drivers/gpio/gpio_driver.h"
#include "drivers/gpio/pwm_driver.h"

/**
 * @brief Initializes the passive buzzer driver
 *
 * @param config The PWM config to use for the buzzer
 * @param pin The pin to use for the buzzer
 * @return Result code
 */
FUNCRESULT passiveBuzzerInit(PWMConfig *config, PinNumber pin, UINT32 frequency);

/**
 * @brief Sets the buzzer active or inactive
 *
 * @param config The PWM config to use for the buzzer
 * @param active TRUE to set the buzzer active, FALSE to set it inactive
 * @return Result code
 */
FUNCRESULT passiveBuzzerSetActive(PWMConfig *config, BOOL active);