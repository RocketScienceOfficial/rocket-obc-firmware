#include "modules/radiolink/radiolink.h"
#include "modules/maths/crc.h"
#include "modules/logger/logger.h"
#include <string.h>

void radiolink_serialize_sensor_frame(radiolink_frame_t *rlFrame, radiolink_sensor_frame_t *frame)
{
    rlFrame->magic = RADIOLINK_MAGIC;
    rlFrame->len = sizeof(*frame);
    rlFrame->msgId = RADIOLINK_MESSAGE_SENSORS;

    memcpy(rlFrame->payload, frame, rlFrame->len);

    rlFrame->checksum = crc16_calculate((uint8_t *)rlFrame, (3 + rlFrame->len) * sizeof(uint8_t));
}

void radiolink_get_bytes(radiolink_frame_t *frame, uint8_t *data, size_t *len)
{
    if (frame->len > RADIOLINK_MAX_PAYLOAD_LENGTH)
    {
        OBC_WARN("RadioLink frame length is too big: %d", frame->len);

        return;
    }

    size_t offset = (3 + frame->len) * sizeof(uint8_t);

    memcpy(data, frame, offset);
    memcpy(data + offset, &frame->checksum, 2 * sizeof(uint8_t));

    *len = (3 + frame->len + 2) * sizeof(uint8_t);
}

bool radiolink_deserialize(radiolink_frame_t *frame, const uint8_t *data, size_t len)
{
    if (len > sizeof(radiolink_frame_t))
    {
        return false;
    }

    size_t offset = 3 * sizeof(uint8_t);

    memcpy(frame, data, offset);

    if (frame->len > RADIOLINK_MAX_PAYLOAD_LENGTH || len - offset != frame->len + 2)
    {
        OBC_ERR("RadioLink frame length is invalid!");

        return false;
    }

    memcpy(frame + offset, data + offset, (frame->len + 2) * sizeof(uint8_t));

    uint16_t checksum = crc16_calculate((uint8_t *)frame, (3 + frame->len) * sizeof(uint8_t));

    if (frame->checksum != checksum)
    {
        OBC_ERR("Checksum of RadioLink frame was incorrect!");

        return false;
    }

    return true;
}