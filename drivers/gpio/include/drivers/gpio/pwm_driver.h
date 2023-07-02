#pragma once

#include "gpio_driver.h"
#include <obc/api.h>

/**
 * @brief Checks whenether the given pin is valid for PWM
 *
 * @param pin Pin number to check
 * @return True if pin is valid
 */
BOOL pwmCheckPin(PinNumber pin);

/**
 * @brief Initialize PWM
 *
 * @param pin Pin to intialize
 * @param frequency Frequency in Hz of PWM
 * @return Result code
 */
FUNCRESULT pwmInit(PinNumber pin, UINT32 frequency);

/**
 * @brief Set duty cycle of PWM
 *
 * @param pin Pin to set duty cycle of
 * @param frequency Frequency in Hz of PWM
 * @param dutyCyclePercent Duty cycle in percent
 * @return Result code
 */
FUNCRESULT pwmSetDuty(PinNumber pin, UINT32 frequency, FLOAT dutyCyclePercent);