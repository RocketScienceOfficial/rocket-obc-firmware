#ifndef _UART_DRIVER_H
#define _UART_DRIVER_H

#include "gpio_driver.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef uint8_t hal_uart_instance_t; /** UART instance */

#define UART_INSTANCE_INVALID 0xFF /** UART invalid instance */

/**
 * @brief Checks if UART is valid
 *
 * @param instance UART Instance
 * @return True if instance is valid
 */
bool hal_uart_check_instance(hal_uart_instance_t uart);

/**
 * @brief Checks if UART RX pin is valid
 *
 * @param uart UART instance
 * @param miso RX pin
 * @return True if pin is valid
 */
bool hal_uart_check_rx(hal_uart_instance_t uart, hal_pin_number_t rx);

/**
 * @brief Checks if UART TX pin is valid
 *
 * @param uart UART instance
 * @param mosi TX pin
 * @return True if pin is valid
 */
bool hal_uart_check_tx(hal_uart_instance_t uart, hal_pin_number_t tx);

/**
 * @brief Initialize UART instance
 *
 * @param uart UART Instance
 * @param rx RX pin
 * @param tx TX pin
 * @param baudrate Baud rate
 */
void hal_uart_init_all(hal_uart_instance_t uart, hal_pin_number_t rx, hal_pin_number_t tx, hal_baud_rate_t baudrate);

/**
 * @brief Write to UART with blocking
 *
 * @param uart UART Instance
 * @param data Data pointer to write
 * @param size Size of data
 * @return True if operation was successful
 */
bool hal_uart_write(hal_uart_instance_t uart, const uint8_t *data, size_t size);

/**
 * @brief Read from UART with blocking
 *
 * @param uart UART Instance
 * @param destination Data pointer to read from
 * @param size Size of data to receive
 * @return True if operation was successful
 */
bool hal_uart_read(hal_uart_instance_t uart, uint8_t *destination, size_t size);

#endif