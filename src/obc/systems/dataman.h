#ifndef _DATAMAN_H
#define _DATAMAN_H

#include "hal/time_tracker.h"
#include "lib/maths/vector.h"
#include "lib/geo/geo.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define DATAMAN_MAGIC 0x2E /** Dataman magic byte */

/**
 * @brief Dataman frame which is saved
 */
typedef struct __attribute__((__packed__)) dataman_frame
{
    uint8_t magic;
    usec_t time;
    vec3_t acc1;
    vec3_t acc2;
    vec3_t acc3;
    vec3_t gyro1;
    vec3_t gyro2;
    vec3_t mag1;
    int press;
    float temp;
    float kalmanHeight;
    geo_position_wgs84_t pos;
    uint8_t smState;
    uint8_t ignFlags;
    uint16_t crc;
} dataman_frame_t;

/**
 * @brief Params structure
 */
typedef struct __attribute__((__packed__)) dataman_config
{
    uint16_t mainHeight;
} dataman_config_t;

/**
 * @brief Initializes dataman
 */
void dataman_init(void);

/**
 * @brief Updates dataman
 */
void dataman_update(void);

/**
 * @brief Checks if dataman is ready (is cleared)
 *
 * @param True if database is clear
 */
bool dataman_is_ready(void);

/**
 * @brief Get config
 *
 * @return Config
 */
const dataman_config_t* dataman_get_config(void);

#endif