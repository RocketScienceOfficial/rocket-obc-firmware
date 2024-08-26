#ifndef _GPIO_DRIVER_H
#define _GPIO_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

typedef uint8_t hal_pin_number_t;     /** Type for GPIO pin number */
typedef unsigned int hal_baud_rate_t; /** Type for baud rate */

#define PIN_NUMBER_INVALID 0xFF /** Invalid pin number */

/**
 * @brief Type for GPIO pin mode
 */
typedef enum hal_gpio_direction
{
    GPIO_INPUT = 0,
    GPIO_OUTPUT = 1
} hal_gpio_direction_t;

/**
 * @brief Type for GPIO pin state
 */
typedef enum hal_gpio_state
{
    GPIO_LOW = 0,
    GPIO_HIGH = 1
} hal_gpio_state_t;

/**
 * @brief Type for GPIO pin function
 */
typedef enum hal_gpio_function
{
    GPIO_FUNCTION_I2C = 0,
    GPIO_FUNCTION_SPI = 1,
    GPIO_FUNCTION_UART = 2,
    GPIO_FUNCTION_PWM = 3,
} hal_gpio_function_t;

/**
 * @brief Checks if pin is valid
 *
 * @param pin Pin number
 * @return true if pin is valid, false otherwise
 */
bool hal_gpio_is_pin_valid(hal_pin_number_t pin);

/**
 * @brief Initialize GPIO pin
 *
 * @param pin Pin to initialize
 * @param dir Direction of pin
 * @return true if success
 */
bool hal_gpio_init_pin(hal_pin_number_t pin, hal_gpio_direction_t dir);

/**
 * @brief Set state of GPIO pin
 *
 * @param pin Pin to set state
 * @param state State to set
 * @return true if success
 */
bool hal_gpio_set_pin_state(hal_pin_number_t pin, hal_gpio_state_t state);

/**
 * @brief Gety state of GPIO pin
 *
 * @param pin Pin to get state of
 * @return State
 */
hal_gpio_state_t hal_gpio_get_pin_state(hal_pin_number_t pin);

/**
 * @brief Set function of GPIO pin
 *
 * @param pin Pin to set function
 * @param function Function to set
 * @return true if success
 */
bool hal_gpio_set_pin_function(hal_pin_number_t pin, hal_gpio_function_t function);

/**
 * @brief Pull GPIO pin up
 *
 * @param pin Pin to pull up
 * @return true if success
 */
bool hal_gpio_pull_up_pin(hal_pin_number_t pin);

#endif