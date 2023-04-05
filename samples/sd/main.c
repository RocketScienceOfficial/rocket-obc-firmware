#include "drivers/storage/sd_driver.h"
#include "pico/stdlib.h"

#define FILE_NAME "TEST_FILE.txt"

int main()
{
    stdio_init_all();
    sleep_ms(5000);

    SDCard sdCard = {0};

    sdInit(&sdCard, 0);
    sdInitFile(&sdCard, FILE_NAME);
    sdClearFile(&sdCard, FILE_NAME);
    sdBegin(&sdCard, FILE_NAME);
    sdWrite(&sdCard, "That's a simple test!", FILE_NAME);
    sdEnd(&sdCard, FILE_NAME);
    sdTerminate(&sdCard);

    while (TRUE)
    {
        sleep_ms(1000);
    }

    return 0;
}