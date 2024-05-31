#include "radio.h"
#include "sensors.h"
#include "ahrs.h"
#include "lib/crypto/crc.h"
#include <math.h>

static uint8_t s_Seq;

static radio_frame_t _create_packet(void);

void radio_init(void)
{
}

void radio_update(void)
{
}

static radio_frame_t _create_packet(void)
{
    radio_frame_t frame = {
        .magic = RADIO_MAGIC,
        .roll = ahrs_get_data()->rotation.x,
        .pitch = ahrs_get_data()->rotation.y,
        .yaw = ahrs_get_data()->rotation.z,
        .velocity = sqrtf(ahrs_get_data()->velocity.x * ahrs_get_data()->velocity.x + ahrs_get_data()->velocity.y * ahrs_get_data()->velocity.y + ahrs_get_data()->velocity.z * ahrs_get_data()->velocity.z),
        .batteryVoltage10 = sensors_get_frame()->batVolts * 10,
        .batteryPercentage = sensors_get_frame()->batPercent,
        .pos = sensors_get_frame()->pos,
        .seq = s_Seq,
    };

    frame.crc = crc16_mcrf4xx_calculate((const uint8_t *)&frame, sizeof(frame) - 2);

    s_Seq = s_Seq == 255 ? 0 : s_Seq + 1;

    return frame;
}