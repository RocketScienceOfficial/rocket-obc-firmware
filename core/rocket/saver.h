#pragma once

#include <obc/api.h>
#include "measurements_utils.h"

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