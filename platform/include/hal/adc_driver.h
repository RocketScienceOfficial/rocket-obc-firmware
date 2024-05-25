#ifndef _ADC_DRIVER_H
#define _ADC_DRIVER_H

#include "gpio_driver.h"
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Checks whenether the given pin is valid for ADC
 *
 * @param pin Pin number to check
 * @return true if pin is valid
 */
bool hal_adc_check_pin(hal_pin_number_t pin);

/**
 * @brief Initialize ADC
 */
void hal_adc_init_all(void);

/**
 * @brief Initialize ADC for given input
 *
 * @param pin Pin to initialize
 */
void hal_adc_init_pin(hal_pin_number_t pin);

/**
 * @brief Reads ADC value. To convert that value using proportion, use the following formula: (delta_value) / (delta_voltage) * (voltage - voltage_min) + value_min.
 *
 * @param pin Pin to read from
 * @return Voltage value
 */
hal_voltage_level_t hal_adc_read_voltage(hal_pin_number_t pin);

#endif