#ifndef _DATAMAN_H
#define _DATAMAN_H

#include "hal/time_tracker.h"
#include "lib/maths/vector.h"
#include "lib/geo/geo.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define DATAMAN_FRAME_MAGIC 0x2E
#define DATAMAN_FILE_INFO_MAGIC 0x8F3E
#define SECTORS_OFFSET_FILE_INFO 64
#define SECTORS_OFFSET_STANDING_BUFFER 65
#define SECTORS_OFFSET_DATA 80
#define SECTORS_COUNT_STANDING_BUFFER ((SECTORS_OFFSET_DATA) - (SECTORS_OFFSET_STANDING_BUFFER))
#define SECTORS_COUNT_DATA 3500
#define STANDING_BUFFER_LENGTH (FLASH_PAGE_SIZE * 2)
#define LANDING_BUFFER_LENGTH (FLASH_PAGE_SIZE * 2)
#define DATA_RECOVERY_MAX_FRAMES 150000

/**
 * @brief Dataman frame which is saved
 */
typedef struct __attribute__((__packed__)) dataman_frame
{
    uint8_t magic;
    hal_usec_t time;
    vec3_t acc1;
    vec3_t acc2;
    vec3_t acc3;
    vec3_t gyro1;
    vec3_t gyro2;
    vec3_t mag1;
    int press;
    float kalmanHeight;
    geo_position_wgs84_t pos;
    uint8_t smState;
    uint8_t batteryVoltage;
    uint8_t ignFlags;
    uint8_t gpsData;
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
 * @brief Dataman info file structure
 */
typedef struct __attribute__((__packed__)) dataman_file_info
{
    uint16_t magic;
    size_t savedFramesCount;
    size_t standingFramesCount;
    dataman_config_t config;
    uint16_t crc;
} dataman_file_info_t;

/**
 * @brief Possible flags of igniters
 */
typedef enum dataman_ign_flags
{
    DATAMAN_IGN_FLAG_IGN_1_CONT = 1 << 0,
    DATAMAN_IGN_FLAG_IGN_2_CONT = 1 << 1,
    DATAMAN_IGN_FLAG_IGN_3_CONT = 1 << 2,
    DATAMAN_IGN_FLAG_IGN_4_CONT = 1 << 3,
    DATAMAN_IGN_FLAG_IGN_1_STATE = 1 << 4,
    DATAMAN_IGN_FLAG_IGN_2_STATE = 1 << 5,
    DATAMAN_IGN_FLAG_IGN_3_STATE = 1 << 6,
    DATAMAN_IGN_FLAG_IGN_4_STATE = 1 << 7,
} dataman_ign_flags_t;

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
const dataman_config_t *dataman_get_config(void);

#endif