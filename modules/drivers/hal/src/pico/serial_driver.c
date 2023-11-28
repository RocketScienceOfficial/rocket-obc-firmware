#include "modules/drivers/hal/serial_driver.h"
#include "pico/stdlib.h"
#include <stdarg.h>
#include <stdio.h>

void hal_serial_printf(char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    vprintf(fmt, args);

    va_end(args);
}

bool hal_serial_read_char(int *chr)
{
    *chr = getchar_timeout_us(0);

    return *chr != PICO_ERROR_TIMEOUT;
}