#pragma once

#include "gpio_typedefs.h"
#include "tools/typedefs.h"

/**
 * @brief I2C instance.
 */
typedef UINT8 UARTInstance;

/**
 * @brief I2C baud rate.
 */
typedef UINT32 UARTBaudRate;

/**
 * @brief Initialize UART instance.
 *
 * @param uart UART instance.
 * @param baudrate Baud rate.
 * @return Result code.
 */
FUNCRESULT uartInitAll(UARTInstance uart, UARTBaudRate baudrate);