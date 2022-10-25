#include "drivers/gpio/gpio_driver.h"
#include "pico/stdlib.h"

FUNCRESULT gpioInitPin(PinNumber pin, GPIODirection dir)
{
    gpio_init(pin);
    gpio_set_dir(pin, dir == GPIO_INPUT ? GPIO_IN : GPIO_OUT);

    return SUC_OK;
}

FUNCRESULT gpioSetPinState(PinNumber pin, GPIOState state)
{
    gpio_put(pin, state == GPIO_HIGH ? 1 : 0);

    return SUC_OK;
}

FUNCRESULT gpioSetPinFunction(PinNumber pin, GPIOFunction function)
{
    INT32 func = 0;

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
        return ERR_INVALIDARG;
    }

    gpio_set_function(pin, func);

    return SUC_OK;
}

FUNCRESULT gpioPullUpPin(PinNumber pin)
{
    gpio_pull_up(pin);

    return SUC_OK;
}