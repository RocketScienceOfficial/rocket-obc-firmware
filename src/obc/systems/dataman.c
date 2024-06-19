#include "dataman.h"
#include "sensors.h"
#include "sm.h"
#include "ign.h"
#include "board_config.h"
#include "../middleware/events.h"
#include "../middleware/syslog.h"
#include "hal/flash_driver.h"
#include "hal/serial_driver.h"
#include "lib/crypto/crc.h"
#include <string.h>

#define SYSTEM_NAME "dataman"
#define SECTORS_OFFSET_FILE_INFO 64
#define SECTORS_OFFSET_STANDING_BUFFER 65
#define SECTORS_OFFSET_DATA 75
#define SECTORS_COUNT_STANDING_BUFFER ((SECTORS_OFFSET_DATA) - (SECTORS_OFFSET_STANDING_BUFFER))
#define SECTORS_COUNT_DATA 3500
#define STANDING_BUFFER_LENGTH (FLASH_PAGE_SIZE)
#define DATA_RECOVERY_MAX_FRAMES 150000
#define DATAMAN_FILE_INFO_MAGIC 0x8F3E
#define SEND_DATA(fmt, ...) hal_serial_printf("/*" fmt "*/\n", ##__VA_ARGS__)
#define SEND_CMD(cmd) hal_serial_printf("\\" cmd "\n")

typedef struct __attribute__((__packed__)) dataman_file_info
{
    uint16_t magic;
    size_t savedFramesCount;
    size_t standingFramesCount;
    uint16_t crc;
} dataman_file_info_t;

static bool s_Terminated;
static uint8_t s_SaveBuffer[FLASH_PAGE_SIZE];
static size_t s_SaveBufferSize;
static size_t s_SaveFlashOffsetPages;
static size_t s_SavedFramesCount;
static dataman_frame_t s_StandingBuffer[STANDING_BUFFER_LENGTH];
static size_t s_StandingBufferLength;
static bool s_ReadyTest;

static dataman_frame_t _get_frame(void);
static bool _validate_frame(const dataman_frame_t *frame);
static bool _validate_info(const dataman_file_info_t *info);
static bool _can_save_data(void);
static void _clear_database(void);
static void _flush_data(void);
static void _flush_standing_buffer(void);
static const dataman_file_info_t *_read_info(void);
static bool _print_saved_frame(const dataman_frame_t *frame);
static void _read_data_raw(size_t maxFrames, size_t *currentFrameCount, const uint8_t *data);
static void _read_data(void);
static void _recover_data_read(const uint8_t *data);
static void _recover_data(void);
static void _save_frame(void);
static void _save_standing_buffer_frame(void);
static void _save_info_file(size_t savedFramesCount, size_t standingFramesCount);

void dataman_init(void)
{
    SYS_LOG("READY");
}

void dataman_update(void)
{
    if (!s_Terminated)
    {
        if (sm_get_state() == FLIGHT_STATE_LANDED)
        {
            _flush_data();
            _flush_standing_buffer();
            _save_info_file(s_SavedFramesCount, s_StandingBufferLength);

            s_Terminated = true;
        }
        else if (sm_get_state() == FLIGHT_STATE_STANDING)
        {
            if (events_poll(MSG_CMD_DATA_READ))
            {
                _read_data();
            }
            if (events_poll(MSG_CMD_DATA_CLEAR))
            {
                _clear_database();
            }
            if (events_poll(MSG_CMD_DATA_RECOVERY))
            {
                _recover_data();
            }
            if (events_poll(MSG_SENSORS_NORMAL_READ))
            {
                _save_standing_buffer_frame();
            }
        }
        else if (sm_get_state() == FLIGHT_STATE_ACCELERATING || sm_get_state() == FLIGHT_STATE_FREE_FALL || sm_get_state() == FLIGHT_STATE_FREE_FLIGHT)
        {
            if (events_poll(MSG_SENSORS_NORMAL_READ))
            {
                _save_frame();
            }
        }
    }
}

bool dataman_is_ready(void)
{
    static bool ready = false;

    if (!s_ReadyTest)
    {
        s_ReadyTest = true;

        const dataman_file_info_t *info = _read_info();

        SYS_LOG("Info file exists: %d", info != NULL);

        ready = info ? info->savedFramesCount + info->standingFramesCount == 0 : true;
    }

    return ready;
}

static dataman_frame_t _get_frame(void)
{
    dataman_frame_t frame = {
        .magic = DATAMAN_MAGIC,
        .time = hal_time_get_us_since_boot(),
        .acc1 = sensors_get_frame()->acc1,
        .acc2 = sensors_get_frame()->acc2,
        .acc3 = sensors_get_frame()->acc3,
        .gyro1 = sensors_get_frame()->gyro1,
        .gyro2 = sensors_get_frame()->gyro2,
        .mag1 = sensors_get_frame()->mag1,
        .press = sensors_get_frame()->press,
        .temp = sensors_get_frame()->temp,
        .pos = sensors_get_frame()->pos,
        .smState = (uint8_t)sm_get_state(),
        .ignFlags = ign_get_flags(),
    };

    frame.crc = crc16_mcrf4xx_calculate((const uint8_t *)&frame, sizeof(frame) - 2);

    return frame;
}

static bool _validate_frame(const dataman_frame_t *frame)
{
    if (frame->magic != DATAMAN_MAGIC)
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
    SYS_LOG("Clearing database...");

    hal_flash_erase_sectors(SECTORS_OFFSET_STANDING_BUFFER, SECTORS_COUNT_STANDING_BUFFER + SECTORS_COUNT_DATA);

    SYS_LOG("Database cleared!");

    _save_info_file(0, 0);

    SEND_CMD("data-clear-finish");
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

        SYS_LOG("Flash save buffer has been flushed. %d bytes were written", s_SaveBufferSize);

        s_SaveBufferSize = 0;
    }
}

static void _flush_standing_buffer(void)
{
    uint8_t *data = (uint8_t *)s_StandingBuffer;
    size_t pages = sizeof(s_StandingBuffer) / FLASH_PAGE_SIZE;

    hal_flash_write_pages(SECTORS_OFFSET_STANDING_BUFFER * FLASH_SECTOR_SIZE / FLASH_PAGE_SIZE, data, pages);

    SYS_LOG("Standing buffer has been flushed. %d frames (%d bytes) were written", s_StandingBufferLength, s_StandingBufferLength * sizeof(dataman_frame_t));
}

static const dataman_file_info_t *_read_info(void)
{
    const uint8_t *data;
    hal_flash_read(SECTORS_OFFSET_FILE_INFO * FLASH_SECTOR_SIZE, &data);

    const dataman_file_info_t *info = (const dataman_file_info_t *)data;

    return _validate_info(info) ? info : NULL;
}

static bool _print_saved_frame(const dataman_frame_t *frame)
{
    if (_validate_frame(frame))
    {
        SEND_DATA("%d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%d,%f,%f,%f,%f,%d,%d",
                  frame->time,
                  frame->acc1.x,
                  frame->acc1.y,
                  frame->acc1.z,
                  frame->acc2.x,
                  frame->acc2.y,
                  frame->acc2.z,
                  frame->acc3.x,
                  frame->acc3.y,
                  frame->acc3.z,
                  frame->gyro1.x,
                  frame->gyro1.y,
                  frame->gyro1.z,
                  frame->gyro2.x,
                  frame->gyro2.y,
                  frame->gyro2.z,
                  frame->mag1.x,
                  frame->mag1.y,
                  frame->mag1.z,
                  frame->press,
                  frame->temp,
                  frame->pos.lat,
                  frame->pos.lon,
                  frame->pos.alt,
                  frame->smState,
                  frame->ignFlags);

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
            SEND_DATA("%d", *currentFrameCount);
        }
    }
}

static void _read_data(void)
{
    SYS_LOG("Begining of data read...");

    const dataman_file_info_t *info = _read_info();

    if (info)
    {
        size_t currentFrameCount = info->savedFramesCount + info->standingFramesCount;
        SEND_DATA("%d", currentFrameCount);

        const uint8_t *data;

        hal_flash_read(SECTORS_OFFSET_STANDING_BUFFER * FLASH_SECTOR_SIZE, &data);
        _read_data_raw(info->standingFramesCount, &currentFrameCount, data);

        hal_flash_read(SECTORS_OFFSET_DATA * FLASH_SECTOR_SIZE, &data);
        _read_data_raw(info->savedFramesCount, &currentFrameCount, data);
    }
    else
    {
        SEND_DATA("%d", 0);
    }

    SYS_LOG("Data read finish!");
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

    SEND_CMD("data-recovery-finish");
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

    if (s_StandingBufferLength < STANDING_BUFFER_LENGTH)
    {
        s_StandingBuffer[s_StandingBufferLength++] = frame;
    }
    else
    {
        for (size_t i = 0; i < STANDING_BUFFER_LENGTH - 1; i++)
        {
            s_StandingBuffer[i] = s_StandingBuffer[i + 1];
        }

        s_StandingBuffer[STANDING_BUFFER_LENGTH - 1] = frame;
    }
}

static void _save_info_file(size_t savedFramesCount, size_t standingFramesCount)
{
    dataman_file_info_t info = {
        .magic = DATAMAN_FILE_INFO_MAGIC,
        .savedFramesCount = savedFramesCount,
        .standingFramesCount = standingFramesCount,
    };

    info.crc = crc16_mcrf4xx_calculate((const uint8_t *)&info, sizeof(info) - 2);

    uint8_t data[256];
    memcpy(data, &info, sizeof(dataman_file_info_t));

    hal_flash_erase_sectors(SECTORS_OFFSET_FILE_INFO, 1);
    hal_flash_write_pages(SECTORS_OFFSET_FILE_INFO * FLASH_SECTOR_SIZE / FLASH_PAGE_SIZE, data, 1);

    s_ReadyTest = false;

    SYS_LOG("Dataman file info was saved. Total frame count: %d", info.savedFramesCount + info.standingFramesCount);
}