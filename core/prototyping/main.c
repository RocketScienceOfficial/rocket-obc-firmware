#include "pico/stdlib.h"
#include "drivers/storage/flash_driver.h"
#include "common/measurements_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define FILE_INDEX 0

int main()
{
    stdio_init_all();
    sleep_ms(10000);

    const SIZE dataSize = 256;
    const SIZE singleDataSize = sizeof(MeasurementData);
    const SIZE dataBytesSize = dataSize * singleDataSize;

    MeasurementData *data = (MeasurementData *)malloc(dataBytesSize);

    for (SIZE i = 0; i < dataSize; i++)
    {
        data[i] = (MeasurementData){0};
        data[i].accel_x = 100;
    }

    BYTE *buffer = (BYTE *)malloc(dataBytesSize);
    memcpy(buffer, data, dataBytesSize);

    flashEraseSectors(0, (SIZE)ceil((FLOAT)dataBytesSize / flashSectorSize()));
    flashWritePages(0, buffer, dataBytesSize / flashWriteBufferSize());

    free(data);
    free(buffer);

    const BYTE *newBuffer;
    flashRead(0, &newBuffer);

    for (SIZE i = 0; i < dataSize; i++)
    {
        for (SIZE j = 0; j < singleDataSize; j++)
        {
            printf("%02X", newBuffer[i * singleDataSize + j]);
        }

        printf("\n");
    }

    while (TRUE)
    {
        sleep_ms(1000);
    }

    return 0;
}