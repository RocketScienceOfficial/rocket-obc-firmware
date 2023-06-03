#include "drivers/storage/sd_driver.h"
#include "tools/board_control.h"

#define FILE_NAME "TEST_FILE.txt"

int main()
{
    boardInit(5000);

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
        tightLoop();
    }

    return 0;
}