#include "dataman.h"
#include "sensors.h"
#include "ahrs.h"
#include "sm.h"
#include "serial.h"
#include "ign.h"
#include "board_config.h"
#include "../middleware/events.h"
#include <hal/flash_driver.h>
#include <lib/crypto/crc.h>
#include <string.h>

static bool s_Terminated;
static uint8_t s_SaveBuffer[FLASH_PAGE_SIZE];
static size_t s_SaveBufferSize;
static size_t s_SaveFlashOffsetPages;
static size_t s_SavedFramesCount;
static dataman_frame_t s_StandingBuffer[STANDING_BUFFER_LENGTH];
static size_t s_StandingBufferLength;
static size_t s_StandingBufferIndex;
static size_t s_LandingBufferIndex;
static dataman_file_info_t s_CurrentInfoFile;
static hal_usec_t s_LastSaveTime;

static dataman_frame_t _get_frame(void);
static uint8_t _get_ign_flags(void);
static uint8_t _get_gps_data(void);
static uint8_t _get_single_ign_flag(uint8_t ignNumber, dataman_ign_flags_t contFlag, dataman_ign_flags_t stateFlag);
static bool _validate_frame(const dataman_frame_t *frame);
static bool _validate_info(const dataman_file_info_t *info);
static bool _can_save_data(void);
static void _clear_database(void);
static void _flush_data(void);
static void _flush_standing_buffer(void);
static bool _print_saved_frame(const dataman_frame_t *frame);
static void _read_data_raw(size_t maxFrames, size_t *currentFrameCount, const uint8_t *data);
static void _read_data(void);
static void _recover_data_read(const uint8_t *data);
static void _recover_data(void);
static void _save_frame(void);
static void _save_standing_buffer_frame(void);
static void _read_info_file(void);
static void _save_info_file(void);
static void _get_config(void);
static void _set_config(const datalink_frame_structure_serial_t *msg);

void dataman_init(void)
{
    _read_info_file();

    SERIAL_DEBUG_LOG("READY");
}

void dataman_update(void)
{
    if (!s_Terminated)
    {
        if (sm_get_state() == FLIGHT_STATE_STANDING)
        {
            if (events_poll(MSG_SERIAL_MESSAGE_RECEIVED))
            {
                const datalink_frame_structure_serial_t *msg = serial_get_current_message();

                if (msg)
                {
                    if (msg->msgId == DATALINK_MESSAGE_DATA_REQUEST_READ)
                    {
                        _read_data();
                    }
                    else if (msg->msgId == DATALINK_MESSAGE_DATA_REQUEST_CLEAR)
                    {
                        _clear_database();
                    }
                    else if (msg->msgId == DATALINK_MESSAGE_DATA_REQUEST_RECOVERY)
                    {
                        _recover_data();
                    }
                    else if (msg->msgId == DATALINK_MESSAGE_CONFIG_GET)
                    {
                        _get_config();
                    }
                    else if (msg->msgId == DATALINK_MESSAGE_CONFIG_SET)
                    {
                        _set_config(msg);
                    }
                }
            }

            hal_usec_t currentTime = hal_time_get_us_since_boot();

            if (currentTime - s_LastSaveTime >= DATAMAN_SAVE_RATE_US)
            {
                s_LastSaveTime = currentTime;

                _save_standing_buffer_frame();
            }
        }
        else if (sm_get_state() == FLIGHT_STATE_ACCELERATING || sm_get_state() == FLIGHT_STATE_FREE_FALL || sm_get_state() == FLIGHT_STATE_FREE_FLIGHT || sm_get_state() == FLIGHT_STATE_LANDED)
        {
            hal_usec_t currentTime = hal_time_get_us_since_boot();

            if (currentTime - s_LastSaveTime >= DATAMAN_SAVE_RATE_US)
            {
                s_LastSaveTime = currentTime;

                _save_frame();

                if (sm_get_state() == FLIGHT_STATE_LANDED)
                {
                    s_LandingBufferIndex++;

                    if (s_LandingBufferIndex == LANDING_BUFFER_LENGTH)
                    {
                        _flush_data();
                        _flush_standing_buffer();

                        s_CurrentInfoFile.savedFramesCount = s_SavedFramesCount;
                        s_CurrentInfoFile.standingFramesCount = s_StandingBufferLength;

                        _save_info_file();

                        s_Terminated = true;
                    }
                }
            }
        }
    }
}

bool dataman_is_ready(void)
{
    return s_CurrentInfoFile.savedFramesCount + s_CurrentInfoFile.standingFramesCount == 0;
}

const dataman_config_t *dataman_get_config(void)
{
    return &s_CurrentInfoFile.config;
}

static dataman_frame_t _get_frame(void)
{
    static hal_usec_t lastTime = 0;

    hal_usec_t currentTime = hal_time_get_us_since_boot();
    uint16_t dt = (uint16_t)(currentTime - lastTime);
    lastTime = currentTime;

    dataman_frame_t frame = {
        .magic = DATAMAN_FRAME_MAGIC,
        .dt_us = dt,
        .acc = ahrs_get_data()->acceleration,
        .vel = ahrs_get_data()->velocity,
        .pos = ahrs_get_data()->position,
        .q = ahrs_get_data()->orientation,
        .gpsPos = sensors_get_frame()->pos,
        .smState = (uint8_t)sm_get_state(),
        .batteryVoltage100 = sensors_get_frame()->batVolts * 100,
        .ignFlags = _get_ign_flags(),
        .gpsData = _get_gps_data(),
    };

    frame.crc = crc16_mcrf4xx_calculate((const uint8_t *)&frame, sizeof(frame) - 2);

    return frame;
}

static uint8_t _get_ign_flags(void)
{
    uint8_t flags = 0;

    flags |= _get_single_ign_flag(1, DATAMAN_IGN_FLAG_IGN_1_CONT, DATAMAN_IGN_FLAG_IGN_1_STATE);
    flags |= _get_single_ign_flag(2, DATAMAN_IGN_FLAG_IGN_2_CONT, DATAMAN_IGN_FLAG_IGN_2_STATE);
    flags |= _get_single_ign_flag(3, DATAMAN_IGN_FLAG_IGN_3_CONT, DATAMAN_IGN_FLAG_IGN_3_STATE);
    flags |= _get_single_ign_flag(4, DATAMAN_IGN_FLAG_IGN_4_CONT, DATAMAN_IGN_FLAG_IGN_4_STATE);

    return flags;
}

static uint8_t _get_gps_data(void)
{
    return (uint8_t)sensors_get_frame()->gpsIs3dFix | (sensors_get_frame()->gpsSatellitesCount << 1);
}

static uint8_t _get_single_ign_flag(uint8_t ignNumber, dataman_ign_flags_t contFlag, dataman_ign_flags_t stateFlag)
{
    uint8_t contFlags = ign_get_cont_flags(ignNumber);
    bool state = ign_get_state(ignNumber);

    return ((contFlags & IGN_CONT_FLAG_IGN_PRESENT) && (contFlags & IGN_CONT_FLAG_FUSE_WORKING) ? (uint8_t)contFlag : 0) | (state ? (uint8_t)stateFlag : 0);
}

static bool _validate_frame(const dataman_frame_t *frame)
{
    if (frame->magic != DATAMAN_FRAME_MAGIC)
    {
        return false;
    }

    uint16_t crc = crc16_mcrf4xx_calculate((const uint8_t *)frame, sizeof(dataman_frame_t) - 2);

    return crc == frame->crc;
}

static bool _validate_info(const dataman_file_info_t *info)
{
    if (info->magic != DATAMAN_FILE_INFO_MAGIC)
    {
        return false;
    }

    uint16_t crc = crc16_mcrf4xx_calculate((const uint8_t *)info, sizeof(dataman_file_info_t) - 2);

    return crc == info->crc;
}

static bool _can_save_data(void)
{
    return s_SaveFlashOffsetPages + sizeof(s_SaveBuffer) / FLASH_PAGE_SIZE <= SECTORS_COUNT_DATA * FLASH_SECTOR_SIZE / FLASH_PAGE_SIZE;
}

static void _clear_database(void)
{
    SERIAL_DEBUG_LOG("Clearing database...");

    size_t totalCount = SECTORS_COUNT_STANDING_BUFFER + SECTORS_COUNT_DATA;

    for (size_t i = 0; i < totalCount; i++)
    {
        hal_flash_erase_sectors(SECTORS_OFFSET_STANDING_BUFFER + i, 1);

        float progress = (i + 1) * 100 / (float)totalCount;

        datalink_frame_data_progress_clear_t payload = {
            .percentage = (uint8_t)progress,
        };
        datalink_frame_structure_serial_t response = {
            .msgId = DATALINK_MESSAGE_DATA_PROGRESS_CLEAR,
            .len = sizeof(payload),
        };
        memcpy(response.payload, &payload, sizeof(payload));
        serial_send_message(&response);
    }

    SERIAL_DEBUG_LOG("Database cleared!");

    s_CurrentInfoFile.savedFramesCount = 0;
    s_CurrentInfoFile.standingFramesCount = 0;

    _save_info_file();

    datalink_frame_structure_serial_t response = {
        .msgId = DATALINK_MESSAGE_DATA_FINISH_CLEAR,
        .len = 0,
    };
    serial_send_message(&response);
}

static void _flush_data(void)
{
    if (_can_save_data())
    {
        if (s_SaveBufferSize > 0)
        {
            memset(s_SaveBuffer + s_SaveBufferSize, 0, sizeof(s_SaveBuffer) - s_SaveBufferSize);

            hal_flash_write_pages(SECTORS_OFFSET_DATA * FLASH_SECTOR_SIZE / FLASH_PAGE_SIZE + s_SaveFlashOffsetPages, s_SaveBuffer, sizeof(s_SaveBuffer) / FLASH_PAGE_SIZE);
        }

        SERIAL_DEBUG_LOG("Flash save buffer has been flushed. %d bytes were written", s_SaveBufferSize);

        s_SaveBufferSize = 0;
    }
}

static void _reverse_buff(dataman_frame_t *buffer, size_t start, size_t end)
{
    while (start < end)
    {
        dataman_frame_t tmp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = tmp;

        start++;
        end--;
    }
}

static void _flush_standing_buffer(void)
{
    _reverse_buff(s_StandingBuffer, 0, s_StandingBufferIndex - 1);
    _reverse_buff(s_StandingBuffer, s_StandingBufferIndex, STANDING_BUFFER_LENGTH - 1);
    _reverse_buff(s_StandingBuffer, 0, STANDING_BUFFER_LENGTH - 1);

    uint8_t *data = (uint8_t *)s_StandingBuffer;
    size_t pages = sizeof(s_StandingBuffer) / FLASH_PAGE_SIZE;

    hal_flash_write_pages(SECTORS_OFFSET_STANDING_BUFFER * FLASH_SECTOR_SIZE / FLASH_PAGE_SIZE, data, pages);

    SERIAL_DEBUG_LOG("Standing buffer has been flushed. %d frames (%d bytes) were written", s_StandingBufferLength, s_StandingBufferLength * sizeof(dataman_frame_t));
}

static bool _print_saved_frame(const dataman_frame_t *frame)
{
    if (_validate_frame(frame))
    {
        datalink_frame_data_saved_chunk_t payload = {
            .dt = frame->dt_us,
            .accX = frame->acc.x,
            .accY = frame->acc.y,
            .accZ = frame->acc.z,
            .velN = frame->vel.x,
            .velE = frame->vel.y,
            .velD = frame->vel.z,
            .posN = frame->pos.x,
            .posE = frame->pos.y,
            .posD = frame->pos.z,
            .qw = frame->q.w,
            .qx = frame->q.x,
            .qy = frame->q.y,
            .qz = frame->q.z,
            .lat = frame->gpsPos.lat,
            .lon = frame->gpsPos.lon,
            .alt = frame->gpsPos.alt,
            .smState = frame->smState,
            .batteryVoltage100 = frame->batteryVoltage100,
            .ignFlags = frame->ignFlags,
            .gpsData = frame->gpsData,
        };
        datalink_frame_structure_serial_t response = {
            .msgId = DATALINK_MESSAGE_DATA_SAVED_CHUNK,
            .len = sizeof(payload),
        };
        memcpy(response.payload, &payload, sizeof(payload));
        serial_send_message(&response);

        return true;
    }
    else
    {
        return false;
    }
}

static void _read_data_raw(size_t maxFrames, size_t *currentFrameCount, const uint8_t *data)
{
    for (size_t i = 0; i < maxFrames; i++)
    {
        const dataman_frame_t *frame = (const dataman_frame_t *)data + i;

        if (!_print_saved_frame(frame))
        {
            (*currentFrameCount)--;

            datalink_frame_data_saved_size_t payload = {
                .size = *currentFrameCount,
            };
            datalink_frame_structure_serial_t response = {
                .msgId = DATALINK_MESSAGE_DATA_SAVED_SIZE,
                .len = sizeof(payload),
            };
            memcpy(response.payload, &payload, sizeof(payload));
            serial_send_message(&response);
        }
    }
}

static void _read_data(void)
{
    SERIAL_DEBUG_LOG("Begining of data read...");

    size_t currentFrameCount = s_CurrentInfoFile.savedFramesCount + s_CurrentInfoFile.standingFramesCount;

    datalink_frame_data_saved_size_t payload = {
        .size = currentFrameCount,
    };
    datalink_frame_structure_serial_t sizeResponse = {
        .msgId = DATALINK_MESSAGE_DATA_SAVED_SIZE,
        .len = sizeof(payload),
    };
    memcpy(sizeResponse.payload, &payload, sizeof(payload));
    serial_send_message(&sizeResponse);

    const uint8_t *data;

    hal_flash_read(SECTORS_OFFSET_STANDING_BUFFER * FLASH_SECTOR_SIZE, &data);
    _read_data_raw(s_CurrentInfoFile.standingFramesCount, &currentFrameCount, data);

    hal_flash_read(SECTORS_OFFSET_DATA * FLASH_SECTOR_SIZE, &data);
    _read_data_raw(s_CurrentInfoFile.savedFramesCount, &currentFrameCount, data);

    datalink_frame_structure_serial_t finishResponse = {
        .msgId = DATALINK_MESSAGE_DATA_FINISH_READ,
        .len = 0,
    };
    serial_send_message(&finishResponse);

    SERIAL_DEBUG_LOG("Data read finish!");
}

static void _recover_data_read(const uint8_t *data)
{
    for (size_t i = 0; i < DATA_RECOVERY_MAX_FRAMES; i++)
    {
        const dataman_frame_t *frame = (const dataman_frame_t *)data + i;

        if (!_print_saved_frame(frame))
        {
            break;
        }
    }
}

static void _recover_data(void)
{
    const uint8_t *data;

    hal_flash_read(SECTORS_OFFSET_STANDING_BUFFER * FLASH_SECTOR_SIZE, &data);
    _recover_data_read(data);

    hal_flash_read(SECTORS_OFFSET_DATA * FLASH_SECTOR_SIZE, &data);
    _recover_data_read(data);

    datalink_frame_structure_serial_t response = {
        .msgId = DATALINK_MESSAGE_DATA_FINISH_RECOVERY,
        .len = 0,
    };
    serial_send_message(&response);
}

static void _save_frame(void)
{
    if (!_can_save_data())
    {
        return;
    }

    dataman_frame_t frame = _get_frame();
    uint8_t *data = (uint8_t *)&frame;
    size_t len = sizeof(frame);

    s_SavedFramesCount++;

    if (s_SaveBufferSize + len <= sizeof(s_SaveBuffer))
    {
        memcpy(s_SaveBuffer + s_SaveBufferSize, data, len);
        s_SaveBufferSize += len;
    }
    else
    {
        int tmpLen = (int)sizeof(s_SaveBuffer) - (int)s_SaveBufferSize;

        if (tmpLen > 0)
        {
            memcpy(s_SaveBuffer + s_SaveBufferSize, data, tmpLen);

            s_SaveBufferSize += tmpLen;
        }

        size_t pages = s_SaveBufferSize / FLASH_PAGE_SIZE;

        hal_flash_write_pages(SECTORS_OFFSET_DATA * FLASH_SECTOR_SIZE / FLASH_PAGE_SIZE + s_SaveFlashOffsetPages, s_SaveBuffer, pages);

        s_SaveFlashOffsetPages += pages;

        memcpy(s_SaveBuffer, data + tmpLen, len - tmpLen);
        s_SaveBufferSize = len - tmpLen;
    }
}

static void _save_standing_buffer_frame(void)
{
    dataman_frame_t frame = _get_frame();

    s_StandingBuffer[s_StandingBufferIndex++] = frame;

    if (s_StandingBufferIndex >= STANDING_BUFFER_LENGTH)
    {
        s_StandingBufferIndex = 0;
    }

    if (s_StandingBufferLength < STANDING_BUFFER_LENGTH)
    {
        s_StandingBufferLength++;
    }
}

static void _read_info_file(void)
{
    const uint8_t *data;
    hal_flash_read(SECTORS_OFFSET_FILE_INFO * FLASH_SECTOR_SIZE, &data);

    const dataman_file_info_t *info = (const dataman_file_info_t *)data;

    if (_validate_info(info))
    {
        s_CurrentInfoFile = *info;

        SERIAL_DEBUG_LOG("Dataman file info was read. Total frame count: %d", s_CurrentInfoFile.savedFramesCount + s_CurrentInfoFile.standingFramesCount);
    }
    else
    {
        SERIAL_DEBUG_LOG("Dataman file info is invalid, creating new one...");

        s_CurrentInfoFile = (dataman_file_info_t){
            .magic = DATAMAN_FILE_INFO_MAGIC,
        };

        _save_info_file();
    }
}

static void _save_info_file(void)
{
    s_CurrentInfoFile.crc = crc16_mcrf4xx_calculate((const uint8_t *)&s_CurrentInfoFile, sizeof(s_CurrentInfoFile) - 2);

    uint8_t data[256];
    memset(data, 0xff, sizeof(data));
    memcpy(data, &s_CurrentInfoFile, sizeof(dataman_file_info_t));

    hal_flash_erase_sectors(SECTORS_OFFSET_FILE_INFO, 1);
    hal_flash_write_pages(SECTORS_OFFSET_FILE_INFO * FLASH_SECTOR_SIZE / FLASH_PAGE_SIZE, data, 1);

    SERIAL_DEBUG_LOG("Dataman file info was saved. Total frame count: %d", s_CurrentInfoFile.savedFramesCount + s_CurrentInfoFile.standingFramesCount);
}

static void _get_config(void)
{
    datalink_frame_config_get_t payload = {
        .mainHeight = s_CurrentInfoFile.config.mainHeight,
        .malfunctionSpeed = s_CurrentInfoFile.config.malfunctionSpeed,
    };
    datalink_frame_structure_serial_t response = {
        .msgId = DATALINK_MESSAGE_CONFIG_GET_ACK,
        .len = sizeof(payload),
    };
    memcpy(response.payload, &payload, sizeof(payload));
    serial_send_message(&response);
}

static void _set_config(const datalink_frame_structure_serial_t *msg)
{
    const datalink_frame_config_set_t *payload = (const datalink_frame_config_set_t *)msg->payload;

    s_CurrentInfoFile.config.mainHeight = payload->mainHeight;
    s_CurrentInfoFile.config.malfunctionSpeed = payload->malfunctionSpeed;

    _save_info_file();

    datalink_frame_structure_serial_t response = {
        .msgId = DATALINK_MESSAGE_CONFIG_SET_ACK,
        .len = 0,
    };
    serial_send_message(&response);
}