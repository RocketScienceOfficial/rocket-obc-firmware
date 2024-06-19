#include "hal/flash_driver.h"
#include "hardware/flash.h"
#include "hardware/sync.h"

#define DISABLE_INTS() uint32_t __ints = save_and_disable_interrupts()
#define RESTORE_INTS() restore_interrupts(__ints)
#define FLASH_SAFE_CALL(x) \
    {                      \
        DISABLE_INTS();    \
        (x);               \
        RESTORE_INTS();    \
    }

void hal_flash_read(size_t offset, const uint8_t **data)
{
    *data = (const uint8_t *)(XIP_BASE + offset);
}

void hal_flash_write_pages(size_t offsetPages, const uint8_t *buffer, size_t pagesCount)
{
    size_t offset = offsetPages * FLASH_PAGE_SIZE;
    size_t size = pagesCount * FLASH_PAGE_SIZE;

    FLASH_SAFE_CALL(flash_range_program(offset, buffer, size));
}

void hal_flash_erase_sectors(size_t sectorsOffset, size_t sectorsCount)
{
    size_t offset = sectorsOffset * FLASH_SECTOR_SIZE;
    size_t size = sectorsCount * FLASH_SECTOR_SIZE;

    FLASH_SAFE_CALL(flash_range_erase(offset, size));
}