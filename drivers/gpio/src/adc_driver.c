#include "drivers/gpio/adc_driver.h"
#include "hardware/adc.h"

#define ADC_VOLTAGE 3.3f
#define ADC_BITS 12
#define ADC_CONVERSION_FACTOR (ADC_VOLTAGE / (1 << ADC_BITS))

BOOL adcCheckPin(PinNumber pin)
{
    return pin == 26 || pin == 27 || pin == 28 || pin == 29;
}

BOOL adcCheckInput(ADCInput input)
{
    return input >= 0 && input <= 4;
}

ADCInput adcConvertPinToInput(PinNumber pin)
{
    if (!adcCheckPin(pin))
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

PinNumber adcConvertInputToPin(ADCInput input)
{
    if (!adcCheckInput(input))
    {
        return INVALID_PIN_NUMBER;
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
        return INVALID_PIN_NUMBER;
    }
}

FUNCRESULT adcInitAll()
{
    adc_init();

    return SUC_OK;
}

FUNCRESULT adcInitPin(ADCInput input)
{
    if (!adcCheckInput(input))
    {
        return ERR_INVALIDARG;
    }

    if (input != ADC_TEMP_INPUT)
    {
        adc_gpio_init(adcConvertInputToPin(input));
    }
    else
    {
        adc_set_temp_sensor_enabled(true);
    }

    return SUC_OK;
}

FUNCRESULT adcRead(ADCInput input, VoltageLevel *voltage)
{
    if (!adcCheckInput(input) || !voltage)
    {
        return ERR_INVALIDARG;
    }

    if (adc_get_selected_input() != input)
    {
        adc_select_input(input);
    }

    *voltage = (FLOAT)adc_read() * ADC_CONVERSION_FACTOR;

    return SUC_OK;
}