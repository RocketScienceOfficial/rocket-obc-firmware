#include "drivers/storage/flash_driver.h"
#include "tools/board_control.h"
#include <stdio.h>
#include <string.h>

int main()
{
    boardInit(10000);

    flashEraseSectors(0, 1);

    const BYTE *writeBuffer = (const BYTE *)"Hello World!    aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa END";
    printf("Written to flash: \n%s\n\n", writeBuffer);
    flashWritePage(0, writeBuffer);

    const BYTE *readBuffer;
    flashRead(0, &readBuffer);
    printf("Read from flash: \n%s\n", readBuffer);

    flashEraseSectors(0, 1);

    while (TRUE)
    {
        tightLoop();
    }

    return 0;
}