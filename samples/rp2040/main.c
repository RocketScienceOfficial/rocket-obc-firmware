#include "pico/stdlib.h"
#include <stdio.h>

int main()
{
    stdio_init_all();
    sleep_ms(5000);

    printf("Hello, World!");

    while (1)
    {
        int a = 2, b = 3;

        printf("2 + 3 = %d\n", a + b);
        
        sleep_ms(1000);
    }

    return 0;
}