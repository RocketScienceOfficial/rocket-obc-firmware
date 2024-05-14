#ifndef _PWM_DRIVER_H
#define _PWM_DRIVER_H

#include "gpio_driver.h"
#include <stdbool.h>

typedef struct pwm_config
{
    hal_pin_number_t pin;
    unsigned long frequency;
} hal_pwm_config_t;

/**
 * @brief Checks whenether the given pin is valid for PWM
 *
 * @param pin Pin number to check
 * @return True if pin is valid
 */
bool hal_pwm_check_pin(hal_pin_number_t pin);

/**
 * @brief Initialize PWM on pin
 *
 * @param config PWM configuration
 * @param pin Pin to intialize
 * @param frequency Frequency in Hz of PWM
 */
void hal_pwm_init_pin(hal_pwm_config_t *config, hal_pin_number_t pin, unsigned long frequency);

/**
 * @brief Sets frequency of PWM
 *
 * @param config PWM configuration
 * @param frequency Frequency in Hz of PWM
 */
void hal_pwm_set_frequency(hal_pwm_config_t *config, unsigned long frequency);

/**
 * @brief Set duty cycle of PWM
 * 
 * @param config PWM configuration
 * @param dutyCyclePercent Duty cycle in percent
 */
void hal_pwm_set_duty(hal_pwm_config_t *config, float dutyCyclePercent);

#endif