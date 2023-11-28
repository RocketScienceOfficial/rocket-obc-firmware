#include "modules/database/dataman.h"
#include "modules/database/db_flash_sectors.h"
#include "modules/drivers/storage/flash_driver.h"
#include "modules/drivers/hal/flash_hal_driver.h"
#include "modules/logger/logger.h"
#include <string.h>
#include <stdbool.h>
#include <string.h>

#define READ_MAX_ITER 50000
#define LOG_FRAME(fmt, ...) printf("/*" fmt "*/\n", ##__VA_ARGS__);

static uint8_t s_Buffer[256 * 32];
static size_t s_BufferSize;
static size_t s_OffsetPages;

static void _dataman_get_bytes(dataman_frame_t *frame, uint8_t *data, size_t *len)
{
    if (frame->len > DATAMAN_MAX_PAYLOAD_LENGTH)
    {
        OBC_ERR("Invalid database frame length!");

        return;
    }

    *len = sizeof(usec_t) + (2 + frame->len) * sizeof(uint8_t);

    memcpy(data, frame, *len);
}

static void _dataman_save(dataman_frame_t *frame)
{
    uint8_t *data;
    size_t len;
    _dataman_get_bytes(frame, data, &len);

    if (s_BufferSize + len <= sizeof(s_Buffer))
    {
        memcpy(s_Buffer + s_BufferSize, data, len);
        s_BufferSize += len;
    }
    else
    {
        size_t tmpLen = sizeof(s_Buffer) - s_BufferSize;

        if (tmpLen > 0)
        {
            memcpy(s_Buffer + s_BufferSize, data, tmpLen);
        }

        size_t pages = s_BufferSize / hal_flash_write_buffer_size();

        flash_write_pages(s_OffsetPages, s_Buffer, pages);

        s_OffsetPages += pages;

        memcpy(s_Buffer, data + tmpLen, len - tmpLen);
        s_BufferSize = len - tmpLen;

        OBC_INFO("Successfully saved data!");
    }
}

void dataman_clear(void)
{
    flash_erase_sectors(DATAMAN_SECTORS_OFFSET, DATAMAN_SECTORS_COUNT);

    OBC_INFO("Cleared database!");
}

void dataman_read(void)
{
    const uint8_t *data;
    flash_read(DATAMAN_SECTORS_OFFSET * hal_flash_sector_size(), &data);

    dataman_frame_t *frame;
    size_t sz = 0, i = 0;

    while (i < READ_MAX_ITER)
    {
        frame = (dataman_frame_t *)(data + sz);

        if (frame->magic == DATAMAN_MAGIC_BYTE && frame->len < DATAMAN_MAX_PAYLOAD_LENGTH)
        {
            if (frame->frameId == DATAMAN_FRAME_SENSORS)
            {
                dataman_sensor_frame_t newFrame;
                memcpy(&newFrame, frame->payload, frame->len);

                LOG_FRAME("%f,%f,%f,%f,%f,%f,%f,%f,%f", newFrame.accel.x, newFrame.accel.y, newFrame.accel.z, newFrame.gyro.x, newFrame.gyro.y, newFrame.gyro.z, newFrame.mag.x, newFrame.mag.y, newFrame.mag.z);
            }
            else
            {
                OBC_ERR("Invalid frame Id!");

                break;
            }
        }
        else
        {
            break;
        }

        sz += (3 + frame->len) * sizeof(uint8_t) + sizeof(usec_t);
        i++;
    }

    if (i == READ_MAX_ITER)
    {
        OBC_WARN("Database read max iterations reached!");
    }

    OBC_INFO("Finished reading");
}

void dataman_flush(void)
{
    if (s_BufferSize > 0)
    {
        memset(s_Buffer + s_BufferSize, 0, sizeof(s_Buffer) - s_BufferSize);

        flash_write_pages(s_OffsetPages, s_Buffer, sizeof(s_Buffer));
    }
}

void dataman_save_sensors_frame(dataman_sensor_frame_t *frame)
{
    dataman_frame_t dFrame = {
        .magic = DATAMAN_MAGIC_BYTE,
        .time = hal_time_get_us_since_boot(),
        .len = sizeof(*frame),
        .frameId = DATAMAN_FRAME_SENSORS,
    };

    memcpy(&dFrame.payload, frame, dFrame.len);

    _dataman_save(&dFrame);
}