#ifndef _ADC_DRIVER_H
#define _ADC_DRIVER_H

#include "gpio_driver.h"
#include <stdint.h>
#include <stdbool.h>

typedef uint8_t hal_adc_input_t; /** ADC input modes */

#define ADC_INPUT_INVALID 0xFF /** Invalid input of ADC */

/**
 * @brief Checks whenether the given pin is valid for ADC
 *
 * @param pin Pin number to check
 * @return true if pin is valid
 */
bool hal_adc_check_pin(hal_pin_number_t pin);

/**
 * @brief Checks whenether the given ADC input is valid for ADC
 *
 * @param input Input to check
 * @return true if input is valid
 */
bool hal_adc_check_input(hal_adc_input_t input);

/**
 * @brief Converts pin to input
 *
 * @param pin Pin to convert
 * @return ADC input
 */
hal_adc_input_t hal_adc_convert_pin_to_input(hal_pin_number_t pin);

/**
 * @brief Converts input to pin
 *
 * @param input Input to convert
 * @return Pin number
 */
hal_pin_number_t hal_adc_convert_input_to_pin(hal_adc_input_t input);

/**
 * @brief Initialize ADC
 */
void hal_adc_init_all(void);

/**
 * @brief Initialize ADC for given input
 *
 * @param input Input to initialize
 */
void hal_adc_init_pin(hal_adc_input_t input);

/**
 * @brief Reads ADC value. To convert that value using proportion, use the following formula: (delta_value) / (delta_voltage) * (voltage - voltage_min) + value_min.
 *
 * @param input Input to read from
 * @return Voltage value
 */
hal_voltage_level_t hal_adc_read_voltage(hal_adc_input_t input);

#endif