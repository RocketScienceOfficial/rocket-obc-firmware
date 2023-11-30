#ifndef _FLIGHT_SM_H
#define _FLIGHT_SM_H

#include "modules/maths/vector.h"
#include <stddef.h>
#include <stdbool.h>

/**
 * @brief Flight State Machine Type
 */
typedef enum flight_state_type
{
    FLIGHT_STATE_STANDING = 0,
    FLIGHT_STATE_ACCELERATING = 1,
    FLIGHT_STATE_FREE_FLIGHT = 2,
    FLIGHT_STATE_FREE_FALL = 3,
    FLIGHT_STATE_LANDED = 4,
} flight_state_type_t;

/**
 * @brief Flight State Machine Data
 */
typedef struct flight_sm_data
{
    flight_state_type_t state;
    float apogee;
    bool apogeeReached;
    float lastAlt;
    size_t lastAltInd;
} flight_sm_data_t;

/**
 * @brief Flight State Machine Input Data
 */
typedef struct flight_sm_input
{
    vec3_t accel;
    float alt;
} flight_sm_input_t;

/**
 * @brief Initialize Flight State Machine
 *
 * @param data Flight state data
 */
void flight_sm_init(flight_sm_data_t *data);

/**
 * @brief Checks and updates Flight State Machine
 *
 * @param data Flight state data
 * @param input Flight state input
 */
void flight_sm_update(flight_sm_data_t *data, flight_sm_input_t *input);

#endif