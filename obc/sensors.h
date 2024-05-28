#ifndef _SENSORS_H
#define _SENSORS_H

#include "lib/maths/vector.h"
#include "lib/geo/geo.h"
#include "hal/gpio_driver.h"
#include <stdbool.h>

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
    hal_voltage_level_t ignDet1;
    hal_voltage_level_t ignDet2;
    hal_voltage_level_t ignDet3;
    hal_voltage_level_t ignDet4;
    hal_voltage_level_t batVolts;
    uint8_t batPercent;
    uint8_t batNCells;
} sensors_frame_t;

/**
 * @brief Sensors update info structure
 */
typedef struct sensors_update_info
{
    bool normal;
    bool gps;
    bool adc;
} sensors_update_info_t;

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

/**
 * @brief Check what was updated in this frame
 * 
 * @return Sensors update info
 */
const sensors_update_info_t *sensors_get_info(void);

#endif