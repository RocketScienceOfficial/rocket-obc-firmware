#include "core/common/measurements_utils.h"
#include "kernel/logging/logger.h"

static Logger s_Logger;

#define MY_LOG_RECEIVER_DATA_PATTERN "%c"
#define MY_LOG_RECEIVER_DATA_NAME "RECEIVER_DATA_LOG"
#define MY_LOG_RECEIVER_DATA_BEGIN() myLog(&s_Logger, "", "/*")
#define MY_LOG_RECEIVER_DATA_END() myLog(&s_Logger, "", "*/\n")
#define MY_LOG_RECEIVER_DATA_INTERNAL(val, format) myLog(&s_Logger, "", format, val)
#define MY_LOG_RECEIVER_DATA_FLOAT(val) MY_LOG_RECEIVER_DATA_INTERNAL(val, "%f,")
#define MY_LOG_RECEIVER_DATA_INT(val) MY_LOG_RECEIVER_DATA_INTERNAL(val, "%d,")
#define MY_LOG_RECEIVER_DATA_EMPTY() myLog(&s_Logger, "", ",")

VOID initializeMeasurementLogger()
{
    myLogCreateLogger(&s_Logger, MY_LOG_RECEIVER_DATA_NAME);
    myLogCreateConsoleSink(&s_Logger, MY_LOG_RECEIVER_DATA_PATTERN);
}

VOID logMeasurementData(MeasurementData *data)
{
    MY_LOG_RECEIVER_DATA_BEGIN();

    MY_LOG_RECEIVER_DATA_FLOAT(data->accel_x);
    MY_LOG_RECEIVER_DATA_FLOAT(data->accel_y);
    MY_LOG_RECEIVER_DATA_FLOAT(data->accel_z);
    MY_LOG_RECEIVER_DATA_FLOAT(data->vel_x);
    MY_LOG_RECEIVER_DATA_FLOAT(data->vel_y);
    MY_LOG_RECEIVER_DATA_FLOAT(data->vel_z);
    MY_LOG_RECEIVER_DATA_FLOAT(data->pos_x);
    MY_LOG_RECEIVER_DATA_FLOAT(data->pos_y);
    MY_LOG_RECEIVER_DATA_FLOAT(data->pos_z);
    MY_LOG_RECEIVER_DATA_FLOAT(data->roll);
    MY_LOG_RECEIVER_DATA_FLOAT(data->pitch);
    MY_LOG_RECEIVER_DATA_FLOAT(data->yaw);
    MY_LOG_RECEIVER_DATA_FLOAT(data->latitude);
    MY_LOG_RECEIVER_DATA_FLOAT(data->longtitude);
    MY_LOG_RECEIVER_DATA_FLOAT(data->altitude);
    MY_LOG_RECEIVER_DATA_FLOAT(data->temperature);
    MY_LOG_RECEIVER_DATA_FLOAT(data->pressure);
    MY_LOG_RECEIVER_DATA_FLOAT(data->humidity);
    MY_LOG_RECEIVER_DATA_INT(data->radioSignalStrength);
    MY_LOG_RECEIVER_DATA_FLOAT(data->cpuTemp);
    MY_LOG_RECEIVER_DATA_FLOAT(data->batteryPercentage);
    MY_LOG_RECEIVER_DATA_FLOAT(data->fuelPercentage);

    MY_LOG_RECEIVER_DATA_END();
}

// #include "pico/stdlib.h"
// #include "core/common/measurements_utils.h"
// #include "drivers/storage/flash_driver.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <math.h>

// typedef struct MeasurementData_FlashSave
// {
//     INT32 _startSeqeuence;
//     MeasurementData data;
// } MeasurementData_FlashSave;

// int main()
// {
//     stdio_init_all();
//     sleep_ms(10000);

//     initializeMeasurementLogger();

//     const SIZE dataSize = 256;
//     const SIZE singleDataSize = sizeof(MeasurementData_FlashSave);
//     const SIZE dataBytesSize = dataSize * singleDataSize;

//     MeasurementData_FlashSave *data = (MeasurementData_FlashSave *)malloc(dataBytesSize);

//     for (SIZE i = 0; i < dataSize; i++)
//     {
//         data[i] = (MeasurementData_FlashSave){0};
//         data[i]._startSeqeuence = 0xEEEEEEEE;
//         data[i].data.accel_x = 100;
//     }

//     BYTE *buffer = (BYTE *)data;

//     flashEraseSectors(0, (SIZE)ceil((FLOAT)dataBytesSize / flashSectorSize()));
//     flashWritePages(0, buffer, dataBytesSize / flashWriteBufferSize());

//     free(data);

//     const BYTE *newBuffer;
//     flashRead(0, &newBuffer);
//     MeasurementData_FlashSave *newData = (MeasurementData_FlashSave *)newBuffer;
//     INT32 seq = 0, i = 0;

//     while (TRUE)
//     {
//         memcpy(&seq, &newBuffer[i * sizeof(MeasurementData_FlashSave)], sizeof(INT32));

//         if (seq != 0xEEEEEEEE)
//         {
//             break;
//         }

//         logMeasurementData(&newData[i].data);

//         i++;
//     }

//     while (TRUE)
//     {
//         sleep_ms(1000);
//     }

//     return 0;
// }