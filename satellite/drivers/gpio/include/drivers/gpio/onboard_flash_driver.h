#pragma once

#include "tools/typedefs.h"

/**
 * @brief Get flash write buffer size.
 *
 * @return Size.
 */
UINT32 flashWriteBufferSize();

/**
 * @brief Get flash sector size.
 *
 * @return Size.
 */
UINT32 flashSectorSize();

/**
 * @brief Read data from flash.
 *
 * @param offset Offset of data.
 * @param data pointer to data pointer.
 * @return Result code.
 */
FUNCRESULT readFlash(UINT32 offset, const BYTE **data);

/**
 * @brief Write page to flash. Only 256-byte pages are allowed!
 *
 * @param offset Offset of data.
 * @param buffer Data to write.
 * @return Result code.
 */
FUNCRESULT writeFlashPage(UINT32 offset, const BYTE *buffer);

/**
 * @brief Erase data from flash sector.
 *
 * @param offset Offset of data.
 * @param size Size of sectors. Only multiples of 4096 are allowed!
 * @return Result code.
 */
FUNCRESULT eraseFlashSectors(UINT32 offset, SIZE size);