#include "modules/drivers/hal/flash_hal_driver.h"
#include "hardware/flash.h"
#include "hardware/sync.h"

size_t hal_flash_write_buffer_size()
{
    return FLASH_PAGE_SIZE;
}

size_t hal_flash_sector_size()
{
    return FLASH_SECTOR_SIZE;
}

void hal_flash_read(size_t offset, const uint8_t **data)
{
    if (!data)
    {
        return;
    }

    *data = (const uint8_t *)(XIP_BASE + offset);
}

void hal_flash_write_page(size_t offsetPages, uint8_t *buffer)
{
    return hal_flash_write_pages(offsetPages, buffer, 1);
}

void hal_flash_write_pages(size_t offsetPages, uint8_t *buffer, size_t pagesCount)
{
    if (!buffer || pagesCount == 0)
    {
        return;
    }

    size_t offset = offsetPages * hal_flash_write_buffer_size();
    size_t size = pagesCount * hal_flash_write_buffer_size();

    uint32_t ints = save_and_disable_interrupts();

    flash_range_program(offset, buffer, size);

    restore_interrupts(ints);
}

void hal_flash_erase_sectors(size_t sectorsOffset, size_t sectorsCount)
{
    if (sectorsCount == 0)
    {
        return;
    }

    size_t offset = sectorsOffset * hal_flash_sector_size();
    size_t size = sectorsCount * hal_flash_sector_size();

    uint32_t ints = save_and_disable_interrupts();

    flash_range_erase(offset, size);

    restore_interrupts(ints);
}