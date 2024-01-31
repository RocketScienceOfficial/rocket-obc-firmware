#ifndef _RADIOLINK_H
#define _RADIOLINK_H

/**
 * REF: https://mavlink.io/en/guide/serialization.html
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "radiolink_messages.h"

#define RADIOLINK_MAGIC 0x7B
#define RADIOLINK_MAX_PAYLOAD_LENGTH 255

/**
 * @brief RadioLink frame structure
 */
typedef struct radiolink_frame
{
    uint8_t magic;
    uint8_t len;
    uint8_t msgId;
    uint8_t payload[RADIOLINK_MAX_PAYLOAD_LENGTH];
    uint16_t checksum;
} radiolink_frame_t;

/**
 * @brief Serialize sensors frame
 *
 * @param rlFrame Pointer to RadioLink frame
 * @param frame Sensors frame
 */
void radiolink_serialize_sensor_frame(radiolink_frame_t *rlFrame, radiolink_sensor_frame_t *frame);

/**
 * @brief Get bytes from RadioLink frame
 *
 * @param frame Pointer to frame
 * @param data Bytes buffer
 * @param len Pointer to length. Pass the length of the buffer and this will be set a new length
 * @return True if success
 */
bool radiolink_get_bytes(radiolink_frame_t *frame, uint8_t *data, size_t *len);

/**
 * @brief Deserialize RadioLink frame and validate it
 *
 * @param frame Pointer to frame
 * @param data Bytes buffer
 * @param len Length of buffer
 * @return True if success
 */
bool radiolink_deserialize(radiolink_frame_t *frame, const uint8_t *data, size_t len);

#endif