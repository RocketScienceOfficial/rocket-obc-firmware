#pragma once

#include <obc/api.h>

/**
 * @brief Single measurement data structure
 */
typedef struct MeasurementData
{
    FLOAT pos_x;
    FLOAT pos_y;
    FLOAT pos_z;
    FLOAT roll;
    FLOAT pitch;
    FLOAT yaw;
    FLOAT latitude;
    FLOAT longitude;
    FLOAT altitude;
    FLOAT velocity;
} MeasurementData;

typedef struct RawMeasurementData
{
    FLOAT accelX_1;
    FLOAT accelX_2;
    FLOAT accelX_3;
    FLOAT accelY_1;
    FLOAT accelY_2;
    FLOAT accelY_3;
    FLOAT accelZ_1;
    FLOAT accelZ_2;
    FLOAT accelZ_3;
    FLOAT gyrolX_1;
    FLOAT gyrolX_2;
    FLOAT gyrolY_1;
    FLOAT gyrolY_2;
    FLOAT gyrolZ_1;
    FLOAT gyrolZ_2;
    FLOAT magX_1;
    FLOAT magY_1;
    FLOAT magZ_1;
    FLOAT lat;
    FLOAT lon;
    FLOAT alt;
    FLOAT pressure;
    FLOAT temperature;
} RawMeasurementData;

/**
 * @brief Initializes the measurements logger.
 */
VOID initializeMeasurementLogger();

/**
 * @brief Logs the normal measurement data.
 *
 * @param data The data to log.
 */
VOID logMeasurementData(MeasurementData *data);

/**
 * @brief Logs the raw measurement data.
 *
 * @param data The data to log.
 */
VOID logRawMeasurementData(RawMeasurementData *data);

/**
 * @brief Initializes the sensors.
 */
VOID initSensors(VOID);

/**
 * @brief Takes the measurements.
 *
 * @param rawMeasurements The raw measurements.
 */
VOID takeMeasurements(RawMeasurementData *rawMeasurements);

/**
 * @brief Checks if the GPS is initialized.
 *
 * @return TRUE if the GPS data is available.
 */
BOOL checkGPS(VOID);