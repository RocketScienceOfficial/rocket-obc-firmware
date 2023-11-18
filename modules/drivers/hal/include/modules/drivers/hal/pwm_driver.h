#ifndef _PWM_DRIVER_H
#define _PWM_DRIVER_H

#include "gpio_driver.h"
#include <stdbool.h>

typedef struct pwm_config
{
    pin_number_t pin;
    unsigned long frequency;
} pwm_config_t;

/**
 * @brief Checks whenether the given pin is valid for PWM
 *
 * @param pin Pin number to check
 * @return True if pin is valid
 */
bool pwm_check_pin(pin_number_t pin);

/**
 * @brief Initialize PWM on pin
 *
 * @param config PWM configuration
 * @param pin Pin to intialize
 * @param frequency Frequency in Hz of PWM
 */
void pwm_init_pin(pwm_config_t *config, pin_number_t pin, unsigned long frequency);

/**
 * @brief Sets frequency of PWM
 *
 * @param config PWM configuration
 * @param frequency Frequency in Hz of PWM
 */
void pwm_set_frequency(pwm_config_t *config, unsigned long frequency);

/**
 * @brief Set duty cycle of PWM
 * 
 * @param config PWM configuration
 * @param dutyCyclePercent Duty cycle in percent
 */
void pwm_set_duty(pwm_config_t *config, float dutyCyclePercent);

#endif