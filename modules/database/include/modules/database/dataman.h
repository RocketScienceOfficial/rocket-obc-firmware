#ifndef _DATAMAN_H
#define _DATAMAN_H

#include <stdint.h>
#include <stddef.h>
#include "dataman_frames.h"
#include "modules/drivers/hal/time_tracker.h"

#define DATAMAN_MAGIC_BYTE 0x2E        /** Dataman magic byte */
#define DATAMAN_MAX_PAYLOAD_LENGTH 255 /** Dataman max payload length */

/**
 * @brief Dataman frame which is saved
 */
typedef struct dataman_frame
{
    uint8_t magic;
    usec_t time;
    uint8_t len;
    uint8_t frameId;
    uint8_t payload[DATAMAN_MAX_PAYLOAD_LENGTH];
} dataman_frame_t;

/**
 * @brief Clears and prepares the storage
 */
void dataman_clear(void);

/**
 * @brief Reads frames from storage and sends it immediately to the serial
 *
 * @return Count of frames read
 */
size_t dataman_read(void);

/**
 * @brief Flushes current buffer to the storage. Write may be unavailable after that!
 */
void dataman_flush(void);

/**
 * @brief Saves Sensors Frame on the storage
 *
 * @param frame Frame to save
 */
void dataman_save_sensors_frame(const dataman_sensor_frame_t *frame);

#endif