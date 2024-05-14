#ifndef _PASSIVE_BUZZER_DRIVER_H
#define _PASSIVE_BUZZER_DRIVER_H

#include "hal/gpio_driver.h"
#include "hal/pwm_driver.h"
#include <stdbool.h>

/**
 * @brief Initializes the passive buzzer driver
 *
 * @param config The PWM config to use for the buzzer
 * @param pin The pin to use for the buzzer
 */
void passive_buzzer_init(hal_pwm_config_t *config, hal_pin_number_t pin, unsigned int frequency);

/**
 * @brief Sets the buzzer active or inactive
 *
 * @param config The PWM config to use for the buzzer
 * @param active true to set the buzzer active, false to set it inactive
 */
void passive_buzzer_set_active(hal_pwm_config_t *config, bool active);

#endif