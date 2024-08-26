#include "hal/gpio_driver.h"
#include "pico/stdlib.h"

bool hal_gpio_is_pin_valid(hal_pin_number_t pin)
{
    return pin >= 0 && pin <= 29;
}

bool hal_gpio_init_pin(hal_pin_number_t pin, hal_gpio_direction_t dir)
{
    if (!hal_gpio_is_pin_valid(pin))
    {
        return false;
    }

    gpio_init(pin);
    gpio_set_dir(pin, dir == GPIO_INPUT ? GPIO_IN : GPIO_OUT);

    return true;
}

bool hal_gpio_set_pin_state(hal_pin_number_t pin, hal_gpio_state_t state)
{
    if (!hal_gpio_is_pin_valid(pin))
    {
        return false;
    }

    gpio_put(pin, state == GPIO_HIGH ? 1 : 0);

    return true;
}

hal_gpio_state_t hal_gpio_get_pin_state(hal_pin_number_t pin)
{
    if (!hal_gpio_is_pin_valid(pin))
    {
        return GPIO_LOW;
    }

    return gpio_get(pin) ? GPIO_HIGH : GPIO_LOW;
}

bool hal_gpio_set_pin_function(hal_pin_number_t pin, hal_gpio_function_t function)
{
    if (!hal_gpio_is_pin_valid(pin))
    {
        return false;
    }

    enum hal_gpio_function func = GPIO_FUNC_NULL;

    switch (function)
    {
    case GPIO_FUNCTION_I2C:
        func = GPIO_FUNC_I2C;
        break;
    case GPIO_FUNCTION_SPI:
        func = GPIO_FUNC_SPI;
        break;
    case GPIO_FUNCTION_UART:
        func = GPIO_FUNC_UART;
        break;
    case GPIO_FUNCTION_PWM:
        func = GPIO_FUNC_PWM;
        break;
    default:
        return false;
    }

    gpio_set_function(pin, func);

    return true;
}

bool hal_gpio_pull_up_pin(hal_pin_number_t pin)
{
    if (!hal_gpio_is_pin_valid(pin))
    {
        return false;
    }

    gpio_pull_up(pin);

    return true;
}