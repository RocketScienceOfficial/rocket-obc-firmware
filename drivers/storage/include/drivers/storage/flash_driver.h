#pragma once

#include <obc/api.h>

/**
 * @brief Get flash write buffer size.
 *
 * @return Size.
 */
SIZE flashWriteBufferSize();

/**
 * @brief Get flash sector size.
 *
 * @return Size.
 */
SIZE flashSectorSize();

/**
 * @brief Read data from flash.
 *
 * @param offset Offset of data.
 * @param data pointer to data pointer.
 * @return Result code.
 */
FUNCRESULT flashRead(SIZE offset, const BYTE **data);

/**
 * @brief Write a single 256-byte page to flash.
 *
 * @param offsetPages Offset of data.
 * @param buffer Data to write.
 * @return Result code.
 */
FUNCRESULT flashWritePage(SIZE offsetPages, const BYTE *buffer);

/**
 * @brief Write pages to flash. Only 256-byte pages are allowed!
 *
 * @param offsetPages Offset of data.
 * @param buffer Data to write.
 * @param pagesCount Number of pages to write (256 byte each).
 * @return Result code.
 */
FUNCRESULT flashWritePages(SIZE offsetPages, const BYTE *buffer, SIZE pagesCount);

/**
 * @brief Erase data from flash sector.
 *
 * @param sectorsOffset Offset of data.
 * @param sectorsCount Number of sectors to erase (4096 byte each).
 * @return Result code.
 */
FUNCRESULT flashEraseSectors(SIZE sectorsOffset, SIZE sectorsCount);