#pragma once

#include <obc/api.h>
#include "measurements.h"

#define FLASH_DATA_FRAME_START_SEQUENCE 0xEEEEEEEE

/**
 * @brief Single measurement data structure for saving
 */
typedef struct MeasurementData_FlashSave
{
    INT32 _startSeqeuence;
    RawMeasurementData data;
} MeasurementData_FlashSave;

/**
 * @brief Initializes the storage.
 */
VOID initStorage(VOID);

/**
 * @brief Saves the data to the Flash
 *
 * @param data The data to save
 */
VOID saveData(RawMeasurementData *data);