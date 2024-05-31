#ifndef _SENSORS_H
#define _SENSORS_H

#include "lib/maths/vector.h"
#include "lib/geo/geo.h"
#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Sensors frame structure
 */
typedef struct sensors_frame
{
    vec3_t acc1;
    vec3_t acc2;
    vec3_t acc3;
    vec3_t gyro1;
    vec3_t gyro2;
    vec3_t mag1;
    int press;
    float temp;
    geo_position_wgs84_t pos;
    bool gpsFix;
    float ignDet1Volts;
    float ignDet2Volts;
    float ignDet3Volts;
    float ignDet4Volts;
    float batRawVolts;
    float batVolts;
    uint8_t batPercent;
    uint8_t batNCells;
} sensors_frame_t;

/**
 * @brief Initialize sensors
 */
void sensors_init(void);

/**
 * @brief Get data from sensors
 */
void sensors_update(void);

/**
 * @brief Get current frame
 *
 * @return Sensors frame
 */
const sensors_frame_t *sensors_get_frame(void);

#endif