#include "modules/drivers/hal/adc_driver.h"
#include "hardware/adc.h"

#define ADC_VOLTAGE 3.3f
#define ADC_BITS 12
#define ADC_CONVERSION_FACTOR (ADC_VOLTAGE / (1 << ADC_BITS))

#define ADC_TEMP_SENSOR_INPUT 4

bool adc_check_pin(pin_number_t pin)
{
    return pin == 26 || pin == 27 || pin == 28 || pin == 29;
}

bool adc_check_input(adc_input_t input)
{
    return input >= 0 && input <= 4;
}

adc_input_t adc_convert_pin_to_input(pin_number_t pin)
{
    if (!adc_check_pin(pin))
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

pin_number_t adc_convert_input_to_pin(adc_input_t input)
{
    if (!adc_check_input(input))
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

void adc_init_all(void)
{
    adc_init();
}

void adc_init_pin(adc_input_t input)
{
    if (!adc_check_input(input))
    {
        return;
    }

    if (input != ADC_TEMP_SENSOR_INPUT)
    {
        adc_gpio_init(adc_convert_input_to_pin(input));
    }
    else
    {
        adc_set_temp_sensor_enabled(1);
    }
}

voltage_level_t adc_read_voltage(adc_input_t input)
{
    if (!adc_check_input(input))
    {
        return 0;
    }

    if (adc_get_selected_input() != input)
    {
        adc_select_input(input);
    }

    return (float)adc_read() * ADC_CONVERSION_FACTOR;
}