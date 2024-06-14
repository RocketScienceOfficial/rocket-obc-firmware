#ifndef _FLASH_DRIVER_H
#define _FLASH_DRIVER_H

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Read data from flash
 *
 * @param offset Offset in bytes of data
 * @param data pointer to data pointer
 */
void hal_flash_read(size_t offset, const uint8_t **data);

/**
 * @brief Write pages to flash. If you are using multicore, you must lock core before!
 *
 * @param offsetPages Offset pages of data
 * @param buffer Data to write (Buffer must be at least total pages size!)
 * @param pagesCount Number of pages to write (see board_config.h for exact page size)
 */
void hal_flash_write_pages(size_t offsetPages, const uint8_t *buffer, size_t pagesCount);

/**
 * @brief Erase data from flash sector. If you are using multicore, you must lock core before!
 *
 * @param sectorsOffset Offset sectors of data
 * @param sectorsCount Number of sectors to erase (see board_config.h for exact page size)
 */
void hal_flash_erase_sectors(size_t sectorsOffset, size_t sectorsCount);

#endif