#pragma once

#include "gpio_driver.h"
#include <obc/api.h>

typedef UINT8 UARTInstance; /** UART Instance */

/**
 * @brief Initialize UART instance
 *
 * @param uart UART instance
 * @param baudrate Baud rate
 * @return Result code
 */
FUNCRESULT uartInitAll(UARTInstance uart, BaudRate baudrate);