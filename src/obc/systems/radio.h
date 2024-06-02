#ifndef _RADIO_H
#define _RADIO_H

#include "lib/maths/vector.h"
#include "lib/geo/geo.h"
#include <stdint.h>

#define RADIO_MAGIC 0x7B /** Radio magic byte */

/**
 * @brief Radio frame structure, which is sent
 */
typedef struct __attribute__((__packed__)) radio_frame
{
    uint8_t magic;
    uint8_t roll;
    uint8_t pitch;
    uint8_t yaw;
    uint16_t velocity;
    uint8_t batteryVoltage10;
    uint8_t batteryPercentage;
    geo_position_wgs84_t pos;
    uint8_t seq;
    uint16_t crc;
} radio_frame_t;

/**
 * @brief Initialize radio
 */
void radio_init(void);

/**
 * @brief Update radio
 */
void radio_update(void);

#endif