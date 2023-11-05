#pragma once

#include <obc/api.h>
#include "gpio_driver.h"

typedef struct PWMConfig
{
    PinNumber pin;
    UINT32 frequency;
} PWMConfig;

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
 * @param config PWM configuration
 * @param pin Pin to intialize
 * @param frequency Frequency in Hz of PWM
 * @return Result code
 */
FUNCRESULT pwmInit(PWMConfig *config, PinNumber pin, UINT32 frequency);

/**
 * @brief Sets frequency of PWM
 *
 * @param config PWM configuration
 * @param frequency Frequency in Hz of PWM
 * @return Result code
 */
FUNCRESULT pwmSetFrequency(PWMConfig *config, UINT32 frequency);

/**
 * @brief Set duty cycle of PWM
 * 
 * @param config PWM configuration
 * @param dutyCyclePercent Duty cycle in percent
 * @return Result code
 */
FUNCRESULT pwmSetDuty(PWMConfig *config, FLOAT dutyCyclePercent);