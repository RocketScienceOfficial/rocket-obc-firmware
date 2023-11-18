#ifndef _GPIO_DRIVER_H
#define _GPIO_DRIVER_H

#include <stdbool.h>

typedef unsigned int pin_number_t; /** Type for GPIO pin number */
typedef float voltage_level_t;     /** Type for voltage level */
typedef unsigned int baud_rate_t;  /** Type for baud rate */

#define PIN_NUMBER_INVALID 0xFFFFFFFF /** Invalid pin number */

/**
 * @brief Type for GPIO pin mode
 */
typedef enum gpio_direction
{
    GPIO_INPUT = 0,
    GPIO_OUTPUT = 1
} gpio_direction_t;

/**
 * @brief Type for GPIO pin state
 */
typedef enum gpio_state
{
    GPIO_LOW = 0,
    GPIO_HIGH = 1
} gpio_state_t;

/**
 * @brief Type for GPIO pin function
 */
typedef enum gpio_func
{
    GPIO_FUNCTION_I2C = 0,
    GPIO_FUNCTION_SPI = 1,
    GPIO_FUNCTION_UART = 2,
    GPIO_FUNCTION_PWM = 3,
    GPIO_FUNCTION_SIO = 4,
    GPIO_FUNCTION_PIO0 = 5,
    GPIO_FUNCTION_PIO1 = 6,
} gpio_function_t;

/**
 * @brief Type for GPIO pin protocol
 */
typedef enum gpio_protocol
{
    GPIO_PROTOCOL_UNKWOWN = -1,
    GPIO_PROTOCOL_UART = 0,
    GPIO_PROTOCOL_SPI = 1,
    GPIO_PROTOCOL_I2C = 2,
} gpio_protocol_t;

/**
 * @brief Checks if pin is valid
 *
 * @param pin Pin number
 * @return true if pin is valid, false otherwise
 */
bool gpio_is_pin_valid(pin_number_t pin);

/**
 * @brief Initialize GPIO pin
 *
 * @param pin Pin to initialize
 * @param dir Direction of pin
 */
void gpio_init_pin(pin_number_t pin, gpio_direction_t dir);

/**
 * @brief Set state of GPIO pin
 *
 * @param pin Pin to set state
 * @param state State to set
 */
void gpio_set_pin_state(pin_number_t pin, gpio_state_t state);

/**
 * @brief Gety state of GPIO pin
 *
 * @param pin Pin to get state of
 * @param state State to get
 */
void gpio_get_pin_state(pin_number_t pin, gpio_state_t *state);

/**
 * @brief Set function of GPIO pin
 *
 * @param pin Pin to set function
 * @param function Function to set
 */
void gpio_set_pin_function(pin_number_t pin, gpio_function_t function);

/**
 * @brief Pull GPIO pin up
 *
 * @param pin Pin to pull up
 */
void gpio_pull_up_pin(pin_number_t pin);

#endif