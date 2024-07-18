#ifndef _ADC_DRIVER_H
#define _ADC_DRIVER_H

#include "gpio_driver.h"
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Initialize ADC
 * 
 * @return true if success
 */
bool hal_adc_init_all(void);

/**
 * @brief Initialize ADC for given input
 *
 * @param pin Pin to initialize
 * @return true if success
 */
bool hal_adc_init_pin(hal_pin_number_t pin);

/**
 * @brief Reads ADC value. To convert that value using proportion, use the following formula: (delta_value) / (delta_voltage) * (voltage - voltage_min) + value_min.
 *
 * @param pin Pin to read from
 * @return Voltage value. 0 if pin is invalid
 */
float hal_adc_read_voltage(hal_pin_number_t pin);

#endif