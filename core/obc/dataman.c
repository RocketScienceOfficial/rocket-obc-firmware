#include "dataman.h"
#include "sensors.h"
#include "hal/flash_hal_driver.h"
#include "lib/drivers/storage/flash_driver.h"
#include <string.h>
#include <stdbool.h>
#include <string.h>

#define SECTORS_OFFSET 96
#define SECTORS_COUNT 4000

static uint8_t s_Buffer[256 * 2 * 16];
static size_t s_BufferSize;
static size_t s_OffsetPages;

void dataman_init(void)
{
    flash_erase_sectors(SECTORS_OFFSET, SECTORS_COUNT);
}

void dataman_update(void)
{
    if (sensors_get_info()->normal)
    {
        dataman_frame_t frame = {
            .magic = DATAMAN_MAGIC_BYTE,
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

        uint8_t *data = (uint8_t *)&frame;
        size_t len = sizeof(frame);

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

            flash_write_pages(SECTORS_OFFSET * hal_flash_sector_size() / hal_flash_write_buffer_size() + s_OffsetPages, s_Buffer, pages);

            s_OffsetPages += pages;

            memcpy(s_Buffer, data + tmpLen, len - tmpLen);
            s_BufferSize = len - tmpLen;
        }
    }
}

// #define READ_MAX_ITER 50000
// #define LOG_FRAME(fmt, ...) hal_serial_printf("/*" fmt "*/\n", ##__VA_ARGS__);

// size_t dataman_read(void)
// {
//     const uint8_t *data;
//     flash_read(DATAMAN_SECTORS_OFFSET * hal_flash_sector_size(), &data);

//     dataman_frame_t *frame;
//     size_t sz = 0, i = 0;

//     while (i < READ_MAX_ITER)
//     {
//         frame = (dataman_frame_t *)(data + sz);

//         if (frame->magic == DATAMAN_MAGIC_BYTE && frame->len < DATAMAN_MAX_PAYLOAD_LENGTH)
//         {
//             if (frame->frameId == DATAMAN_FRAME_SENSORS)
//             {
//                 dataman_sensor_frame_t newFrame;
//                 memcpy(&newFrame, frame->payload, frame->len);

//                 LOG_FRAME("%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f",
//                           newFrame.acc1.x,
//                           newFrame.acc1.y,
//                           newFrame.acc1.z,
//                           newFrame.acc2.x,
//                           newFrame.acc2.y,
//                           newFrame.acc2.z,
//                           newFrame.acc3.x,
//                           newFrame.acc3.y,
//                           newFrame.acc3.z,
//                           newFrame.gyro1.x,
//                           newFrame.gyro1.y,
//                           newFrame.gyro1.z,
//                           newFrame.gyro2.x,
//                           newFrame.gyro2.y,
//                           newFrame.gyro2.z,
//                           newFrame.mag1.x,
//                           newFrame.mag1.y,
//                           newFrame.mag1.z,
//                           newFrame.press,
//                           newFrame.temp,
//                           newFrame.lat,
//                           newFrame.lon,
//                           newFrame.alt);
//             }
//             else
//             {
//                 hal_serial_printf("Invalid frame Id!\n");

//                 break;
//             }
//         }
//         else
//         {
//             break;
//         }

//         sz += (3 + frame->len) * sizeof(uint8_t) + sizeof(usec_t);
//         i++;
//     }

//     if (i == READ_MAX_ITER)
//     {
//         hal_serial_printf("Database read max iterations reached!\n");
//     }

//     hal_serial_printf("Finished reading\n");

//     return i;
// }

// void dataman_flush(void)
// {
//     if (s_BufferSize > 0)
//     {
//         memset(s_Buffer + s_BufferSize, 0, sizeof(s_Buffer) - s_BufferSize);

//         flash_write_pages(DATAMAN_SECTORS_OFFSET * hal_flash_sector_size() / hal_flash_write_buffer_size() + s_OffsetPages, s_Buffer, sizeof(s_Buffer) / hal_flash_write_buffer_size());
//     }
// }