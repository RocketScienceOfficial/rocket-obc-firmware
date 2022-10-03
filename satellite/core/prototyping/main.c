#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"

int main()
{
    stdio_init_all();
    sleep_ms(5000);
    printf("Hello, World!\n");

    while (1)
    {
        tight_loop_contents();
    }
}