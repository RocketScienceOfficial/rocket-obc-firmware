#ifndef _ACTIVE_BUZZER_DRIVER_H
#define _ACTIVE_BUZZER_DRIVER_H

#include "modules/drivers/hal/gpio_driver.h"
#include <stdbool.h>

/**
 * @brief Initializes the active buzzer driver
 *
 * @param pin The pin to use for the buzzer
 */
void active_buzzer_init(pin_number_t pin);

/**
 * @brief Sets the buzzer active or inactive
 *
 * @param pin The pin to use for the buzzer
 * @param active true to set the buzzer active, false to set it inactive
 */
void active_buzzer_set_active(pin_number_t pin, bool active);

#endif