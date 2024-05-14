#ifndef _SM_H
#define _SM_H

#include "lib/maths/vector.h"
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
typedef struct sm_data
{
    flight_state_type_t state; /** Current state */
    float apogee;              /** Apogee */
    bool apogeeReached;        /** Has apogee been reached */
    float lastAlt;             /** Last altitude */
    size_t lastAltInd;         /** Last altitude verification index */
} sm_data_t;

/**
 * @brief Flight State Machine Input Data
 */
typedef struct sm_input
{
    vec3_t accel; /** Acceleration */
    float alt;    /** Altitude */
} sm_input_t;

/**
 * @brief Initialize Flight State Machine
 *
 * @param data Flight state data
 */
void sm_init(sm_data_t *data);

/**
 * @brief Checks and updates Flight State Machine
 *
 * @param data Flight state data
 * @param input Flight state input
 */
void sm_update(sm_data_t *data, sm_input_t *input);

#endif