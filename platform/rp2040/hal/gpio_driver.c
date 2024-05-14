#include "hal/gpio_driver.h"
#include "pico/stdlib.h"

bool hal_gpio_is_pin_valid(hal_pin_number_t pin)
{
    return pin >= 0 && pin <= 28;
}

void hal_gpio_init_pin(hal_pin_number_t pin, gpio_direction_t dir)
{
    if (!hal_gpio_is_pin_valid(pin))
    {
        return;
    }

    gpio_init(pin);
    gpio_set_dir(pin, dir == GPIO_INPUT ? GPIO_IN : GPIO_OUT);
}

void hal_gpio_set_pin_state(hal_pin_number_t pin, gpio_state_t state)
{
    if (!hal_gpio_is_pin_valid(pin))
    {
        return;
    }

    gpio_put(pin, state == GPIO_HIGH ? 1 : 0);
}

void hal_gpio_get_pin_state(hal_pin_number_t pin, gpio_state_t *state)
{
    if (!hal_gpio_is_pin_valid(pin))
    {
        return;
    }

    gpio_get(pin) ? (*state = GPIO_HIGH) : (*state = GPIO_LOW);
}

void hal_gpio_set_pin_function(hal_pin_number_t pin, gpio_function_t function)
{
    if (!hal_gpio_is_pin_valid(pin))
    {
        return;
    }

    enum gpio_function func = GPIO_FUNC_NULL;

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
    case GPIO_FUNCTION_SIO:
        func = GPIO_FUNC_SIO;
        break;
    case GPIO_FUNCTION_PIO0:
        func = GPIO_FUNC_PIO0;
        break;
    case GPIO_FUNCTION_PIO1:
        func = GPIO_FUNC_PIO1;
        break;
    default:
        return;
    }

    gpio_set_function(pin, func);
}

void hal_gpio_pull_up_pin(hal_pin_number_t pin)
{
    if (!hal_gpio_is_pin_valid(pin))
    {
        return;
    }

    gpio_pull_up(pin);
}