#include "drivers/storage/flash_driver.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include <string.h>

int main()
{
    stdio_init_all();
    sleep_ms(10000);

    flashEraseSectors(0, 1);

    BYTE *writeBuff = (BYTE *)malloc(256);
    memcpy(writeBuff, "Hello World!    aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa END", 256);
    flashWritePage(0, (const BYTE *)writeBuff);

    const BYTE *buffer;
    flashRead(0, &buffer);
    printf("Read from flash: \n%s\n", buffer);

    flashEraseSectors(0, 1);

    while (TRUE)
    {
        sleep_ms(1000);
    }

    return 0;
}