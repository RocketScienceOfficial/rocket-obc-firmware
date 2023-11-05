#include "drivers/storage/flash_driver.h"
#include "hardware/flash.h"
#include "hardware/sync.h"

#define FLASH_BASE_OFFSET (512 * 1024)

SIZE flashWriteBufferSize()
{
    return FLASH_PAGE_SIZE;
}

SIZE flashSectorSize()
{
    return FLASH_SECTOR_SIZE;
}

FUNCRESULT flashRead(SIZE offset, const BYTE **data)
{
    if (!data)
    {
        return ERR_INVALIDARG;
    }

    *data = (const BYTE *)(XIP_BASE + FLASH_BASE_OFFSET + offset);

    return SUC_OK;
}

FUNCRESULT flashWritePage(SIZE offsetPages, BYTE *buffer)
{
    return flashWritePages(offsetPages, buffer, 1);
}

FUNCRESULT flashWritePages(SIZE offsetPages, BYTE *buffer, SIZE pagesCount)
{
    if (!buffer || pagesCount == 0)
    {
        return ERR_INVALIDARG;
    }

    SIZE offset = FLASH_BASE_OFFSET + offsetPages * flashWriteBufferSize();
    SIZE size = pagesCount * flashWriteBufferSize();

    UINT32 ints = save_and_disable_interrupts();
    flash_range_program(offset, buffer, size);
    restore_interrupts(ints);

    return SUC_OK;
}

FUNCRESULT flashEraseSectors(SIZE sectorsOffset, SIZE sectorsCount)
{
    if (sectorsCount == 0)
    {
        return ERR_INVALIDARG;
    }

    SIZE offset = FLASH_BASE_OFFSET + sectorsOffset * flashSectorSize();
    SIZE size = sectorsCount * flashSectorSize();

    UINT32 ints = save_and_disable_interrupts();
    flash_range_erase(offset, size);
    restore_interrupts(ints);

    return SUC_OK;
}