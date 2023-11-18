#include "modules/drivers/hal/serial_driver.h"
#include "pico/stdlib.h"

bool hal_serial_read_char(int *chr)
{
    *chr = getchar_timeout_us(0);

    return *chr != PICO_ERROR_TIMEOUT;
}