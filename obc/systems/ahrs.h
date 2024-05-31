#ifndef _AHRS_H
#define _AHRS_H

#include "lib/maths/vector.h"

/**
 * @brief AHRS data
 */
typedef struct ahrs_data
{
    vec3_t rotation;
    vec3_t position;
    vec3_t velocity;
} ahrs_data_t;

/**
 * @brief Initialize AHRS system
 */
void ahrs_init(void);

/**
 * @brief Update AHRS system
 */
void ahrs_update(void);

/**
 * @brief Gete current AHRS data
 *
 * @return AHRS data
 */
const ahrs_data_t* ahrs_get_data(void);

#endif