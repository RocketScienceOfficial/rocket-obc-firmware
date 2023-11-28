#include "modules/drivers/storage/flash_driver.h"
#include "modules/drivers/hal/flash_hal_driver.h"

#define FLASH_BASE_OFFSET (512 * 1024)

void flash_read(size_t offset, const uint8_t **data)
{
    hal_flash_read(FLASH_BASE_OFFSET + offset, data);
}

void flash_write_page(size_t offsetPages, const uint8_t *buffer)
{
    flash_write_pages(offsetPages, buffer, 1);
}

void flash_write_pages(size_t offsetPages, const uint8_t *buffer, size_t pagesCount)
{
    hal_flash_write_pages(FLASH_BASE_OFFSET / hal_flash_write_buffer_size() + offsetPages, buffer, pagesCount);
}

void flash_erase_sectors(size_t sectorsOffset, size_t sectorsCount)
{
    hal_flash_erase_sectors(FLASH_BASE_OFFSET / hal_flash_sector_size() + sectorsOffset, sectorsCount);
}