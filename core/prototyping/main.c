#include "pico/stdlib.h"
#include "core/common/measurements_utils.h"
#include "drivers/storage/flash_driver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main()
{
    stdio_init_all();
    sleep_ms(10000);

    initializeMeasurementLogger();

    const SIZE dataSize = 256;
    const SIZE singleDataSize = sizeof(MeasurementData);
    const SIZE dataBytesSize = dataSize * singleDataSize;

    MeasurementData *data = (MeasurementData *)malloc(dataBytesSize);

    for (SIZE i = 0; i < dataSize; i++)
    {
        data[i] = (MeasurementData){0};
        data[i].accel_x = 100;
    }

    BYTE *buffer = (BYTE *)data;

    flashEraseSectors(0, (SIZE)ceil((FLOAT)dataBytesSize / flashSectorSize()));
    flashWritePages(0, buffer, dataBytesSize / flashWriteBufferSize());

    free(data);

    const BYTE *newBuffer;
    flashRead(0, &newBuffer);
    MeasurementData *newData = (MeasurementData *)newBuffer;

    for (SIZE i = 0; i < dataSize; i++)
    {
        logMeasurementData(&newData[i]);
    }

    while (TRUE)
    {
        sleep_ms(1000);
    }

    return 0;
}