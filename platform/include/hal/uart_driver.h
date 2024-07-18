#ifndef _UART_DRIVER_H
#define _UART_DRIVER_H

#include "gpio_driver.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef uint8_t hal_uart_instance_t; /** UART instance */

/**
 * @brief Initialize UART instance
 *
 * @param uart UART Instance
 * @param rx RX pin
 * @param tx TX pin
 * @param baudrate Baud rate
 * @return true if success
 */
bool hal_uart_init_all(hal_uart_instance_t uart, hal_pin_number_t rx, hal_pin_number_t tx, hal_baud_rate_t baudrate);

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

/**
 * @brief Check if UART data in FIFO is available
 *
 * @param UART Instance
 * @return True if available
 */
bool hal_uart_is_readable(hal_uart_instance_t uart);

#endif