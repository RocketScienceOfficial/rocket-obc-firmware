#include "saver.h"
#include "driver_calling.h"
#include "drivers/storage/flash_driver.h"
#include <string.h>

#define MEASUREMENTS_BUFFER_SIZE 128
#define FLASH_STORAGE_MAX_SIZE (1 << 19)
#define FLASH_DATA_FRAME_START_SEQUENCE 0xEEEEEEEE

static MeasurementData_FlashSave s_Measurements[MEASUREMENTS_BUFFER_SIZE];
static MeasurementData s_LastNormalMeasurement;
static SIZE s_MeasurementIndex;
static SIZE s_PagesOffset;

VOID initStorage(VOID)
{
    DRIVER_CALL(flashEraseSectors(0, (SIZE)FLASH_STORAGE_MAX_SIZE / flashSectorSize()));
}

VOID saveData(RawMeasurementData *data)
{
    s_Measurements[s_MeasurementIndex++] = (MeasurementData_FlashSave){
        ._startSeqeuence = FLASH_DATA_FRAME_START_SEQUENCE,
        .data = *data,
    };

    if (s_MeasurementIndex >= MEASUREMENTS_BUFFER_SIZE)
    {
        DRIVER_CALL(flashWritePages(s_PagesOffset, (BYTE *)s_Measurements, sizeof(s_Measurements) / flashWriteBufferSize()));

        memset(s_Measurements, 0, sizeof(s_Measurements));
        s_MeasurementIndex = 0;
        s_PagesOffset += sizeof(s_Measurements) / flashWriteBufferSize();
    }
}