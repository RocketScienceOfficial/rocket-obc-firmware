#ifndef _RADIO_H
#define _RADIO_H

#include <stdint.h>
#include <stdbool.h>

#define RADIO_MAGIC 0x7B /** Radio magic byte */

/**
 * @brief Control flags
 */
typedef enum radio_obc_frame_control_flags
{
    RADIO_OBC_FRAME_CONTROL_FLAGS_ARMED = 1 << 0,
    RADIO_OBC_FRAME_CONTROL_FLAGS_VBAT = 1 << 1,
    RADIO_OBC_FRAME_CONTROL_FLAGS_5V = 1 << 2,
    RADIO_OBC_FRAME_CONTROL_FLAGS_3V3 = 1 << 3,
    RADIO_OBC_FRAME_CONTROL_FLAGS_CALIBRATION = 1 << 4,
    RADIO_OBC_FRAME_CONTROL_FLAGS_GPS = 1 << 5,
    RADIO_OBC_FRAME_CONTROL_FLAGS_SELF_TEST = 1 << 6,
} radio_obc_frame_control_flags_t;

/**
 * @brief Radio OBC frame structure, which is sent
 */
typedef struct __attribute__((__packed__)) radio_obc_frame
{
    uint8_t magic;
    float qw;
    float qx;
    float qy;
    float qz;
    uint16_t velocity;
    uint8_t batteryVoltage10;
    uint8_t batteryPercentage;
    double lat;
    double lon;
    uint16_t alt;
    uint8_t state;
    uint8_t controlFlags;
    uint8_t seq;
    uint16_t crc;
} radio_obc_frame_t;

/**
 * @brief Radio TLM flags enum
 */
typedef enum radio_tlm_flags
{
    RADIO_TLM_FLAG_ARM_ENABLE = 1 << 0,
    RADIO_TLM_FLAG_ARM_DISABLE = 1 << 1,
    RADIO_TLM_FLAG_3V3_ENABLE = 1 << 2,
    RADIO_TLM_FLAG_3V3_DISABLE = 1 << 3,
    RADIO_TLM_FLAG_5V_ENABLE = 1 << 4,
    RADIO_TLM_FLAG_5V_DISABLE = 1 << 5,
    RADIO_TLM_FLAG_VBAT_ENABLE = 1 << 6,
    RADIO_TLM_FLAG_VBAT_DISABLE = 1 << 7,
} radio_tlm_flags_t;

/**
 * @brief Radio TLM frame, which is sent
 */
typedef struct __attribute__((__packed__)) radio_tlm_frame
{
    uint8_t magic;
    uint8_t flags;
    uint16_t crc;
} radio_tlm_frame_t;

/**
 * @brief Parsed telemetry data
 */
typedef struct radio_tlm_parsed_data
{
    bool arm_enabled;
    bool arm_disabled;
    bool v3v3_enabled;
    bool v3v3_disabled;
    bool v5_enabled;
    bool v5_disabled;
    bool vbat_enabled;
    bool vbat_disabled;
} radio_tlm_parsed_data_t;

/**
 * @brief Initialize radio
 */
void radio_init(void);

/**
 * @brief Update radio
 */
void radio_update(void);

/**
 * @brief Gert current parsed telemetry data
 *
 * @return Parsed telemetry data
 */
const radio_tlm_parsed_data_t *radio_get_parsed_data(void);

#endif