#include "drivers/storage/flash_driver.h"
#include "pico/stdlib.h"
#include <stdio.h>

#define FILE_INDEX 0

int main()
{
    stdio_init_all();
    sleep_ms(5000);

    flashInit(flashGetDefaultModule());
    flashClearFile(flashGetDefaultModule(), FILE_INDEX);
    flashWriteFile(flashGetDefaultModule(), FILE_INDEX, "Hello World!    aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa END");

    const BYTE *buffer;
    SIZE size;
    flashGetFile(flashGetDefaultModule(), FILE_INDEX, &buffer, &size);

    printf("Read from flash: %.*s\n", size, buffer);

    flashClearFile(flashGetDefaultModule(), FILE_INDEX);
    flashTerminate(flashGetDefaultModule());

    while (TRUE)
    {
        sleep_ms(1000);
    }

    return 0;
}