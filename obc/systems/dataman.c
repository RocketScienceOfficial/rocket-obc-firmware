#include "dataman.h"
#include "sensors.h"
#include "sm.h"
#include "../middleware/events.h"
#include "../middleware/syslog.h"
#include "hal/flash_hal_driver.h"
#include "hal/serial_driver.h"
#include "lib/drivers/storage/flash_driver.h"
#include "lib/crypto/crc.h"
#include <string.h>
#include <stdbool.h>
#include <string.h>

#define SYSTEM_NAME "dataman"

#define SECTORS_FILE_INFO_OFFSET 95
#define SECTORS_SAVE_OFFSET 96
#define SECTORS_COUNT 1000

#define MAX_FRAMES_COUNT 150000

#define DATAMAN_FILE_INFO_MAGIC 0x8F3E

typedef struct dataman_file_info
{
    uint16_t magic;
    size_t frameCount;
    uint16_t crc;
} dataman_file_info_t;

static uint8_t s_Buffer[256 * 2 * 16];
static size_t s_BufferSize;
static size_t s_OffsetPages;
static size_t s_FramesCount;
static bool s_Terminated;

static dataman_frame_t _get_frame(void);
static dataman_file_info_t _get_file_info(void);
static bool _validate_frame(const dataman_frame_t *frame);
static bool _validate_info(const dataman_file_info_t *info);
static void _read_data(void);
static void _flush(void);
static void _save_info_file(void);

void dataman_init(void)
{
    SYS_LOG("Clearing database...");

    flash_erase_sectors(SECTORS_SAVE_OFFSET, SECTORS_COUNT);

    SYS_LOG("Database cleared!");
}

void dataman_update(void)
{
    if (!s_Terminated)
    {
        if (sm_get_state() == FLIGHT_STATE_LANDED)
        {
            _flush();
            _save_info_file();

            s_Terminated = true;
        }
        else if (sm_get_state() == FLIGHT_STATE_STANDING)
        {
            if (events_poll(MSG_CMD_DATA_READ))
            {
                _read_data();
            }
        }
        else if (sm_get_state() == FLIGHT_STATE_ACCELERATING || sm_get_state() == FLIGHT_STATE_FREE_FALL || sm_get_state() == FLIGHT_STATE_FREE_FLIGHT)
        {
            if (events_poll(MSG_SENSORS_NORMAL_READ))
            {
                dataman_frame_t frame = _get_frame();
                uint8_t *data = (uint8_t *)&frame;
                size_t len = sizeof(frame);

                s_FramesCount++;

                if (s_BufferSize + len <= sizeof(s_Buffer))
                {
                    memcpy(s_Buffer + s_BufferSize, data, len);
                    s_BufferSize += len;
                }
                else
                {
                    int tmpLen = (int)sizeof(s_Buffer) - (int)s_BufferSize;

                    if (tmpLen > 0)
                    {
                        memcpy(s_Buffer + s_BufferSize, data, tmpLen);
                    }

                    size_t pages = s_BufferSize / hal_flash_write_buffer_size();

                    flash_write_pages(SECTORS_SAVE_OFFSET * hal_flash_sector_size() / hal_flash_write_buffer_size() + s_OffsetPages, s_Buffer, pages);

                    s_OffsetPages += pages;

                    memcpy(s_Buffer, data + tmpLen, len - tmpLen);
                    s_BufferSize = len - tmpLen;
                }
            }
        }
    }
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
    };

    frame.crc = crc16_mcrf4xx_calculate((const uint8_t *)&frame, sizeof(frame) - 2);

    return frame;
}

static dataman_file_info_t _get_file_info(void)
{
    dataman_file_info_t info = {
        .magic = DATAMAN_FILE_INFO_MAGIC,
        .frameCount = s_FramesCount,
    };

    info.crc = crc16_mcrf4xx_calculate((const uint8_t *)&info, sizeof(info) - 2);

    return info;
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

    if (info->frameCount > MAX_FRAMES_COUNT)
    {
        return false;
    }

    uint16_t crc = crc16_mcrf4xx_calculate((const uint8_t *)info, sizeof(dataman_file_info_t) - 2);

    return crc == info->crc;
}

static void _read_data(void)
{
    SYS_LOG("Begining of data read...");

    const uint8_t *data;
    flash_read(SECTORS_FILE_INFO_OFFSET * hal_flash_sector_size(), &data);

    const dataman_file_info_t *info = (const dataman_file_info_t *)data;

    if (_validate_info(info))
    {
        hal_serial_printf("/*%d*/\n", (int)info->frameCount);

        flash_read(SECTORS_SAVE_OFFSET * hal_flash_sector_size(), &data);

        for (size_t i = 0; i < info->frameCount; i++)
        {
            const dataman_frame_t *frame = (const dataman_frame_t *)data + i;

            if (_validate_frame(frame))
            {
                hal_serial_printf("/*%d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%d,%f,%f,%f,%f*/\n",
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
                                  frame->pos.alt);
            }
        }
    }
    else
    {
        hal_serial_printf("/*0*/\n");
    }

    SYS_LOG("Data read finish!");
}

static void _flush(void)
{
    if (s_BufferSize > 0)
    {
        memset(s_Buffer + s_BufferSize, 0, sizeof(s_Buffer) - s_BufferSize);

        flash_write_pages(SECTORS_SAVE_OFFSET * hal_flash_sector_size() / hal_flash_write_buffer_size() + s_OffsetPages, s_Buffer, sizeof(s_Buffer) / hal_flash_write_buffer_size());
    }

    SYS_LOG("Flash buffer has been flushed. %d bytes were written", s_BufferSize);
}

static void _save_info_file(void)
{
    dataman_file_info_t info = _get_file_info();
    uint8_t data[256];

    memcpy(data, &info, sizeof(dataman_file_info_t));

    flash_erase_sectors(SECTORS_FILE_INFO_OFFSET, 1);
    flash_write_page(SECTORS_FILE_INFO_OFFSET * hal_flash_sector_size() / hal_flash_write_buffer_size(), data);

    SYS_LOG("Dataman file info was saved. Total frame count: %d", s_FramesCount);
}