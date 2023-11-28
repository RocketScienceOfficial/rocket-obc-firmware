#ifndef _PARAMS_H
#define _PARAMS_H

#include <stdint.h>
#include <stdbool.h>

#define DB_PARAMS_MAGIC 0x9B /** Magic byte for params */

/**
 * @brief Params configuration structure
 */
typedef struct db_params
{
} db_params_t;

/**
 * @brief Params frame structure
 */
typedef struct db_params_frame
{
    uint8_t magic;
    db_params_t params;
} db_params_frame_t;

/**
 * @brief Get params
 *
 * @param params Params pointer
 * @return true if success
 */
bool params_get(db_params_t *params);

/**
 * @brief Save params
 *
 * @param params Params pointer
 * @return true if success
 */
bool params_save(db_params_t *params);

#endif