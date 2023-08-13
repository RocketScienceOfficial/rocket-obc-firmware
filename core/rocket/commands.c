#include "commands.h"
#include "measurements_utils.h"
#include "saver.h"
#include "drivers/console/console_input.h"
#include "drivers/storage/flash_driver.h"
#include <string.h>
#include <stdio.h>

VOID readFlashData(VOID)
{
    const BYTE *newBuffer;

    flashRead(0, &newBuffer);

    MeasurementData_FlashSave *newData = (MeasurementData_FlashSave *)newBuffer;
    SIZE seq = 0, i = 0;

    while (TRUE)
    {
        memcpy(&seq, &newBuffer[i * sizeof(MeasurementData_FlashSave)], sizeof(SIZE));

        if (seq != 0xEEEEEEEE)
        {
            break;
        }

        logRawMeasurementData(&newData[i].data);

        i++;
    }

    printf("CMD:read-done\n");
}

VOID checkCMD(VOID)
{
    BOOL available = FALSE;
    INT32 chr = 0;
    ConsoleInput input = {0};
    BOOL tokensReady = FALSE;

    while (TRUE)
    {
        consoleCheckInput(&available, &chr);

        if (available)
        {
            consoleInputProcessCharacter(chr, &input, &tokensReady);

            if (tokensReady)
            {
                if (strcmp(input.tokens[0], "read-data") == 0)
                {
                    readFlashData();
                }

                tokensReady = FALSE;
            }

            available = FALSE;
        }
    }
}