#pragma once

#include "tools/typedefs.h"

/**
 * @brief Type for GPIO pin number.
 */
typedef UINT32 PinNumber;

/**
 * @brief Type for voltage level.
 */
typedef float VoltageLevel;

/**
 * @brief Type for GPIO pin mode.
 */
typedef enum GPIODirection
{
    GPIO_INPUT = 0,
    GPIO_OUTPUT = 1
} GPIODirection;

/**
 * @brief Type for GPIO pin state.
 */
typedef enum GPIOState
{
    GPIO_LOW = 0,
    GPIO_HIGH = 1
} GPIOState;

/**
 * @brief Type for GPIO pin function.
 */
typedef enum GPIOFunction
{
    GPIO_FUNCTION_I2C = 0,
    GPIO_FUNCTION_SPI = 1,
    GPIO_FUNCTION_UART = 2,
    GPIO_FUNCTION_PWM = 3,
    GPIO_FUNCTION_SIO = 4,
    GPIO_FUNCTION_PIO0 = 5,
    GPIO_FUNCTION_PIO1 = 6,
} GPIOFunction;

/**
 * @brief Number for invalid pin.
 */
#define INVALID_PIN_NUMBER 0xFFFFFFFF

/**
 * @brief Checks if pin is valid.
 *
 * @param pin Pin number.
 * @return TRUE if pin is valid, FALSE otherwise.
 */
BOOL gpioIsPinValid(PinNumber pin);

/**
 * @brief Initialize GPIO pin.
 *
 * @param pin Pin to initialize.
 * @param dir Direction of pin.
 * @return Result code.
 */
FUNCRESULT gpioInitPin(PinNumber pin, GPIODirection dir);

/**
 * @brief Set state of GPIO pin.
 *
 * @param pin Pin to set state.
 * @param state State to set.
 * @return Result code.
 */
FUNCRESULT gpioSetPinState(PinNumber pin, GPIOState state);

/**
 * @brief Gety state of GPIO pin.
 *
 * @param pin Pin to get state of.
 * @param state State to get.
 * @return Result code.
 */
FUNCRESULT gpioGetPinState(PinNumber pin, GPIOState *state);

/**
 * @brief Set function of GPIO pin.
 *
 * @param pin Pin to set function.
 * @param function Function to set.
 * @return Result code.
 */
FUNCRESULT gpioSetPinFunction(PinNumber pin, GPIOFunction function);

/**
 * @brief Pull up GPIO pin.
 *
 * @param pin Pin to pull up.
 * @return Result code.
 */
FUNCRESULT gpioPullUpPin(PinNumber pin);