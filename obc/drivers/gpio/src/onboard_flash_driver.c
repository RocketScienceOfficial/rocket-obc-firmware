#include "drivers/gpio/onboard_flash_driver.h"
#include "hardware/flash.h"
#include "hardware/sync.h"

#define FLASH_BASE_OFFSET (FLASH_SECTOR_SIZE * (1 << 6u))

UINT32 flashWriteBufferSize()
{
    return FLASH_PAGE_SIZE;
}

UINT32 flashSectorSize()
{
    return FLASH_SECTOR_SIZE;
}

FUNCRESULT readFlash(UINT32 offset, const BYTE **data)
{
    if (!data)
    {
        return ERR_INVALIDARG;
    }

    *data = (const BYTE *)(XIP_BASE + FLASH_BASE_OFFSET + offset);

    return SUC_OK;
}

FUNCRESULT writeFlashPage(UINT32 offset, const BYTE *buffer)
{
    if (!buffer)
    {
        return ERR_INVALIDARG;
    }

    UINT32 ints = save_and_disable_interrupts();
    flash_range_program(FLASH_BASE_OFFSET + offset, buffer, flashWriteBufferSize());
    restore_interrupts(ints);

    return SUC_OK;
}

FUNCRESULT eraseFlashSectors(UINT32 offset, SIZE size)
{
    if (size % flashSectorSize() != 0)
    {
        return ERR_INVALIDARG;
    }

    UINT32 ints = save_and_disable_interrupts();
    flash_range_erase(FLASH_BASE_OFFSET + offset, size);
    restore_interrupts(ints);

    return SUC_OK;
}