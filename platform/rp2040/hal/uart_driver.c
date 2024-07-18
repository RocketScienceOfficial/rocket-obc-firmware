#include "hal/uart_driver.h"
#include "hardware/uart.h"

static uart_inst_t *_get_uart(hal_uart_instance_t uart);
static bool _uart_check_instance(hal_uart_instance_t uart);
static bool _uart_check_rx(hal_uart_instance_t uart, hal_pin_number_t rx);
static bool _uart_check_tx(hal_uart_instance_t uart, hal_pin_number_t tx);

bool hal_uart_init_all(hal_uart_instance_t uart, hal_pin_number_t rx, hal_pin_number_t tx, hal_baud_rate_t baudrate)
{
    if (!_uart_check_instance(uart) || !_uart_check_rx(uart, rx) || !_uart_check_tx(uart, tx))
    {
        return false;
    }

    if (uart_init(_get_uart(uart), baudrate))
    {
        bool v1 = hal_gpio_set_pin_function(rx, GPIO_FUNCTION_UART);
        bool v2 = hal_gpio_set_pin_function(tx, GPIO_FUNCTION_UART);

        return v1 && v2;
    }

    return false;
}

bool hal_uart_write(hal_uart_instance_t uart, const uint8_t *data, size_t size)
{
    if (!_uart_check_instance(uart) || !data || size == 0)
    {
        return false;
    }

    uart_write_blocking(_get_uart(uart), data, size);

    return true;
}

bool hal_uart_read(hal_uart_instance_t uart, uint8_t *destination, size_t size)
{
    if (!_uart_check_instance(uart) || !destination || size == 0)
    {
        return false;
    }

    uart_read_blocking(_get_uart(uart), destination, size);

    return true;
}

bool hal_uart_is_readable(hal_uart_instance_t uart)
{
    if (!_uart_check_instance(uart))
    {
        return false;
    }

    return uart_is_readable(_get_uart(uart));
}

static uart_inst_t *_get_uart(hal_uart_instance_t uart)
{
    return (uart == 0 ? uart0 : uart1);
}

static bool _uart_check_instance(hal_uart_instance_t uart)
{
    return uart >= 0 && uart <= 1;
}

static bool _uart_check_rx(hal_uart_instance_t uart, hal_pin_number_t rx)
{
    if (!_uart_check_instance(uart))
    {
        return false;
    }

    if (uart == 0)
    {
        return rx == 1 || rx == 13 || rx == 17;
    }
    else if (uart == 1)
    {
        return rx == 5 || rx == 9;
    }
    else
    {
        return false;
    }
}

static bool _uart_check_tx(hal_uart_instance_t uart, hal_pin_number_t tx)
{
    if (!_uart_check_instance(uart))
    {
        return false;
    }

    if (uart == 0)
    {
        return tx == 0 || tx == 12 || tx == 16;
    }
    else if (uart == 1)
    {
        return tx == 4 || tx == 8;
    }
    else
    {
        return false;
    }
}