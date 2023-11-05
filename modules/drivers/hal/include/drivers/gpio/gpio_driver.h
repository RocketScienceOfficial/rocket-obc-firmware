#ifndef

typedef unsigned int pin_number_t; /** Type for GPIO pin number */
typedef float voltage_level_t;     /** Type for voltage level */
typedef unsigned int baud_rate_t;  /** Type for baud rate */

/**
 * @brief Type for GPIO pin mode
 */
typedef enum
{
    GPIO_INPUT = 0,
    GPIO_OUTPUT = 1
} gpio_dir_t;

/**
 * @brief Type for GPIO pin state
 */
typedef enum
{
    GPIO_LOW = 0,
    GPIO_HIGH = 1
} gpio_state_t;

/**
 * @brief Type for GPIO pin function
 */
typedef enum
{
    GPIO_FUNCTION_I2C = 0,
    GPIO_FUNCTION_SPI = 1,
    GPIO_FUNCTION_UART = 2,
    GPIO_FUNCTION_PWM = 3,
    GPIO_FUNCTION_SIO = 4,
    GPIO_FUNCTION_PIO0 = 5,
    GPIO_FUNCTION_PIO1 = 6,
} gpio_func_t;

/**
 * @brief Type for GPIO pin protocol
 */
typedef enum
{
    GPIO_PROTOCOL_UNKWOWN = -1,
    GPIO_PROTOCOL_UART = 0,
    GPIO_PROTOCOL_SPI = 1,
    GPIO_PROTOCOL_I2C = 2,
} gpio_protocol_t;

#define INVALID_PIN_NUMBER 0xFFFFFFFF /** Number for invalid pin */

/**
 * @brief Checks if pin is valid
 *
 * @param pin Pin number
 * @return TRUE if pin is valid, FALSE otherwise
 */
int gpio_is_pin_valid(PinNumber pin);

/**
 * @brief Initialize GPIO pin
 *
 * @param pin Pin to initialize
 * @param dir Direction of pin
 * @return Result code
 */
void gpio_init_pin(PinNumber pin, GPIODirection dir);

/**
 * @brief Set state of GPIO pin
 *
 * @param pin Pin to set state
 * @param state State to set
 * @return Result code
 */
void gpioSetPinState(PinNumber pin, GPIOState state);

/**
 * @brief Gety state of GPIO pin
 *
 * @param pin Pin to get state of
 * @param state State to get
 * @return Result code
 */
void gpioGetPinState(PinNumber pin, GPIOState *state);

/**
 * @brief Set function of GPIO pin
 *
 * @param pin Pin to set function
 * @param function Function to set
 * @return Result code
 */
void gpioSetPinFunction(PinNumber pin, GPIOFunction function);

/**
 * @brief Pull GPIO pin up
 *
 * @param pin Pin to pull up
 * @return Result code
 */
void gpioPullUpPin(PinNumber pin);