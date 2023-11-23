#include "modules/drivers/hal/adc_driver.h"
#include "hardware/adc.h"

#define ADC_VOLTAGE 3.3f
#define ADC_BITS 12
#define ADC_CONVERSION_FACTOR (ADC_VOLTAGE / (1 << ADC_BITS))

#define ADC_TEMP_SENSOR_INPUT 4

bool hal_adc_check_pin(hal_pin_number_t pin)
{
    return pin == 26 || pin == 27 || pin == 28 || pin == 29;
}

bool hal_adc_check_input(hal_adc_input_t input)
{
    return input >= 0 && input <= 4;
}

hal_adc_input_t hal_adc_convert_pin_to_input(hal_pin_number_t pin)
{
    if (!hal_adc_check_pin(pin))
    {
        return ADC_INPUT_INVALID;
    }

    switch (pin)
    {
    case 26:
        return 0;
    case 27:
        return 1;
    case 28:
        return 2;
    case 29:
        return 3;
    default:
        return ADC_INPUT_INVALID;
    }
}

hal_pin_number_t hal_adc_convert_input_to_pin(hal_adc_input_t input)
{
    if (!hal_adc_check_input(input))
    {
        return PIN_NUMBER_INVALID;
    }

    switch (input)
    {
    case 0:
        return 26;
    case 1:
        return 27;
    case 2:
        return 28;
    case 3:
        return 29;
    default:
        return PIN_NUMBER_INVALID;
    }
}

void hal_adc_init_all(void)
{
    adc_init();
}

void hal_adc_init_pin(hal_adc_input_t input)
{
    if (!hal_adc_check_input(input))
    {
        return;
    }

    if (input != ADC_TEMP_SENSOR_INPUT)
    {
        adc_gpio_init(hal_adc_convert_input_to_pin(input));
    }
    else
    {
        adc_set_temp_sensor_enabled(1);
    }
}

hal_voltage_level_t hal_adc_read_voltage(hal_adc_input_t input)
{
    if (!hal_adc_check_input(input))
    {
        return 0;
    }

    if (adc_get_selected_input() != input)
    {
        adc_select_input(input);
    }

    return (float)adc_read() * ADC_CONVERSION_FACTOR;
}