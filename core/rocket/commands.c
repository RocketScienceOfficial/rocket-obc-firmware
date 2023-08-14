#include "commands.h"
#include "measurements_utils.h"
#include "saver.h"
#include "drivers/console/console_input.h"
#include "drivers/storage/flash_driver.h"
#include <string.h>
#include <stdio.h>

static BOOL s_Available;
static INT32 s_Chr;
static ConsoleInput s_Input;
static BOOL s_TokensReady;

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
    consoleCheckInput(&s_Available, &s_Chr);

    if (s_Available)
    {
        consoleInputProcessCharacter(s_Chr, &s_Input, &s_TokensReady);

        if (s_TokensReady)
        {
            if (strcmp(s_Input.tokens[0], "read-data") == 0)
            {
                readFlashData();
            }

            s_TokensReady = FALSE;
        }

        s_Available = FALSE;
    }
}