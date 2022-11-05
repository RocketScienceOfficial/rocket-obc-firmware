#pragma once

#include "gpio_driver.h"
#include "tools/typedefs.h"

/**
 * @brief ADC input modes.
 */
typedef UINT8 ADCInput;

/**
 * @brief ADC input for temp sensor.
 */
#define ADC_TEMP_INPUT 4

/**
 * @brief ADC invalid input.
 */
#define ADC_INPUT_INVALID 0xFF

/**
 * @brief Checks whenether the given pin is valid for ADC.
 *
 * @param pin Pin number to check.
 * @return True if pin is valid.
 */
BOOL adcCheckPin(PinNumber pin);

/**
 * @brief Checks whenether the given ADC input is valid for ADC.
 *
 * @param input Input to check.
 * @return True if input is valid.
 */
BOOL adcCheckInput(ADCInput input);

/**
 * @brief Converts pin to input.
 *
 * @param pin Pin to convert.
 * @return ADC input.
 */
ADCInput adcConvertPinToInput(PinNumber pin);

/**
 * @brief Converts input to pin.
 *
 * @param input Input to convert.
 * @return Pin number.
 */
PinNumber adcConvertInputToPin(ADCInput input);

/**
 * @brief Initialize ADC.
 *
 * @return Result code.
 */
FUNCRESULT adcInitAll();

/**
 * @brief Initialize ADC for given input.
 *
 * @param input Input to initialize.
 * @return Result code.
 */
FUNCRESULT adcInitPin(ADCInput input);

/**
 * @brief Reads ADC value. To convert that value using proportion, use the following formula: (delta_value) / (delta_voltage) * (voltage - voltage_min) + value_min.
 *
 * @param input Input to read from.
 * @param voltage Voltage value to return.
 * @return Result code.
 */
FUNCRESULT adcRead(ADCInput input, VoltageLevel *voltage);