#include "modules/database/params.h"
#include "modules/database/db_flash_sectors.h"
#include "modules/drivers/storage/flash_driver.h"
#include "modules/drivers/hal/flash_hal_driver.h"
#include "modules/logger/logger.h"
#include <string.h>

bool params_get(db_params_t *params)
{
    const uint8_t *data;
    flash_read(PARAMS_SECTORS_OFFSET * hal_flash_sector_size(), &data);

    db_params_frame_t frame;
    memcpy(&frame, data, sizeof(frame));

    if (frame.magic != DB_PARAMS_MAGIC)
    {
        OBC_WARN("DB Params magic did not match!");

        return false;
    }

    *params = frame.params;

    return true;
}

bool params_save(db_params_t *params)
{
    db_params_frame_t frame = {
        .magic = DB_PARAMS_MAGIC,
        .params = *params,
    };
    uint8_t buffer[256];

    memcpy(buffer, &frame, sizeof(frame));

    flash_erase_sectors(PARAMS_SECTORS_OFFSET, 1);
    flash_write_page(PARAMS_SECTORS_OFFSET * hal_flash_sector_size() / hal_flash_write_buffer_size(), buffer);

    OBC_INFO("Saved params");

    return true;
}