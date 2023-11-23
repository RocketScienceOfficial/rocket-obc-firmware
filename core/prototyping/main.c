#include <stdbool.h>
#include <stdio.h>
#include "pico/stdlib.h"

int main()
{
    stdio_init_all();

    sleep_ms(5000);

    while (1)
    {
        printf("Hello, world! %d\n", PICO_FLASH_SIZE_BYTES);

        sleep_ms(1000);
    }
}