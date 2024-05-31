#include "hal/adc_driver.h"
#include "hardware/adc.h"

#define ADC_VOLTAGE 3.3f
#define ADC_BITS 12
#define ADC_CONVERSION_FACTOR (ADC_VOLTAGE / (1 << ADC_BITS))

static int _convert_pin_to_input(hal_pin_number_t pin)
{
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
        return -1;
    }
}

bool hal_adc_check_pin(hal_pin_number_t pin)
{
    return pin >= 26 && pin <= 29;
}

void hal_adc_init_all(void)
{
    adc_init();
}

void hal_adc_init_pin(hal_pin_number_t pin)
{
    if (!hal_adc_check_pin(pin))
    {
        return;
    }

    adc_gpio_init(pin);
}

float hal_adc_read_voltage(hal_pin_number_t pin)
{
    if (!hal_adc_check_pin(pin))
    {
        return 0;
    }

    int input = _convert_pin_to_input(pin);

    if (adc_get_selected_input() != input)
    {
        adc_select_input(input);
    }

    return (float)adc_read() * ADC_CONVERSION_FACTOR;
}