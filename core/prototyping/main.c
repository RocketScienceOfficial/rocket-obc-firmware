#include "pico/stdlib.h"
#include "core/common/measurements_utils.h"
#include "drivers/storage/flash_driver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct MeasurementData_FlashSave
{
    INT32 _startSeqeuence;
    MeasurementData data;
} MeasurementData_FlashSave;

int main()
{
    stdio_init_all();
    sleep_ms(10000);

    initializeMeasurementLogger();

    const SIZE dataSize = 256;
    const SIZE singleDataSize = sizeof(MeasurementData_FlashSave);
    const SIZE dataBytesSize = dataSize * singleDataSize;

    MeasurementData_FlashSave *data = (MeasurementData_FlashSave *)malloc(dataBytesSize);

    for (SIZE i = 0; i < dataSize; i++)
    {
        data[i] = (MeasurementData_FlashSave){0};
        data[i]._startSeqeuence = 0xEEEEEEEE;
        data[i].data.accel_x = 100;
    }

    BYTE *buffer = (BYTE *)data;

    flashEraseSectors(0, (SIZE)ceil((FLOAT)dataBytesSize / flashSectorSize()));
    flashWritePages(0, buffer, dataBytesSize / flashWriteBufferSize());

    free(data);

    const BYTE *newBuffer;
    flashRead(0, &newBuffer);
    MeasurementData_FlashSave *newData = (MeasurementData_FlashSave *)newBuffer;
    INT32 seq = 0, i = 0;

    while (TRUE)
    {
        memcpy(&seq, &newBuffer[i * sizeof(MeasurementData_FlashSave)], sizeof(INT32));

        if (seq != 0xEEEEEEEE)
        {
            break;
        }

        logMeasurementData(&newData[i].data);

        i++;
    }

    while (TRUE)
    {
        sleep_ms(1000);
    }

    return 0;
}