#ifndef _SD_DRIVER_H
#define _SD_DRIVER_H

#include "modules/drivers/hal/gpio_driver.h"
#include "ff.h"
#include "sd_card.h"
#include "f_util.h"
#include <stdbool.h>
#include <stddef.h>

#define SD_FILES_MAX_COUNT 4

/**
 * @brief SD card file structure
 */
typedef struct sd_file
{
    const char *name; /** File name */
    FIL file;         /** File instance */
    bool isOpened;    /** Is file opened */
} sd_file_t;

/**
 * @brief SD card structure. All changes are done internally, so keep it just for reference!
 */
typedef struct sd_card_inst
{
    bool isInitialized;                  /** Is SD card initialized */
    sd_card_t *sd;                       /** SD Card instance */
    sd_file_t files[SD_FILES_MAX_COUNT]; /** SD Card files */
    size_t filesCount;                   /** SD Card files count */
} sd_card_inst_t;

/**
 * @brief Initialize SD card
 *
 * @param sd_card Data to setup
 * @param checkPin Pin to check if SD card is connected
 */
void sd_init(sd_card_inst_t *sd_card, hal_pin_number_t checkPin);

/**
 * @brief Check if SD card is connected
 *
 * @param pin Pin to check
 * @param result Result of check
 */
void sd_check(hal_pin_number_t pin, bool *result);

/**
 * @brief Initialize file
 *
 * @param sd_card SD card structure
 * @param fileName File name
 */
void sd_init_file(sd_card_inst_t *sd_card, const char *fileName);

/**
 * @brief Open and prepare file for writing
 *
 * @param sd_card SD card structure
 * @param fileName File name
 */
void sd_begin(sd_card_inst_t *sd_card, const char *fileName);

/**
 * @brief Write message to SD card
 *
 * @param sd_card SD card structure
 * @param msg Message
 * @param fileName File name
 */
void sd_write(sd_card_inst_t *sd_card, const char *msg, const char *fileName);

/**
 * @brief Close file
 *
 * @param sd_card SD card structure
 * @param fileName File name
 */
void sd_end(sd_card_inst_t *sd_card, const char *fileName);

/**
 * @brief Clear file
 *
 * @param sd_card SD card structure
 * @param fileName File name
 */
void sd_clear_file(sd_card_inst_t *sd_card, const char *fileName);

/**
 * @brief Terminate SD. After this call, SD card is not usable.
 *
 * @param sd_card SD card structure
 */
void sd_terminate(sd_card_inst_t *sd_card);

#endif