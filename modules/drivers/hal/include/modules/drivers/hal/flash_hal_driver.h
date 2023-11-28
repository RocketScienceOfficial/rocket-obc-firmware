#ifndef _FLASH_HAL_DRIVER_H
#define _FLASH_HAL_DRIVER_H

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Get flash write buffer size (256 bytes)
 *
 * @return Size
 */
size_t hal_flash_write_buffer_size();

/**
 * @brief Get flash sector size (4096 bytes)
 *
 * @return Size
 */
size_t hal_flash_sector_size();

/**
 * @brief Read data from flash
 *
 * @param offset Offset of data
 * @param data pointer to data pointer
 */
void hal_flash_read(size_t offset, const uint8_t **data);

/**
 * @brief Write a single 256-byte page to flash
 *
 * @param offsetPages Offset of data
 * @param buffer Data to write
 */
void hal_flash_write_page(size_t offsetPages, const uint8_t *buffer);

/**
 * @brief Write pages to flash. Only 256-byte pages are allowed!
 *
 * @param offsetPages Offset of data
 * @param buffer Data to write
 * @param pagesCount Number of pages to write (256 byte each)
 */
void hal_flash_write_pages(size_t offsetPages, const uint8_t *buffer, size_t pagesCount);

/**
 * @brief Erase data from flash sector
 *
 * @param sectorsOffset Offset of data
 * @param sectorsCount Number of sectors to erase (4096 byte each)
 */
void hal_flash_erase_sectors(size_t sectorsOffset, size_t sectorsCount);

#endif