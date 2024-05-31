#ifndef _SM_H
#define _SM_H

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
 * @brief Initialize Flight State Machine
 */
void sm_init(void);

/**
 * @brief Checks and updates Flight State Machine
 */
void sm_update(void);

/**
 * @brief Gets current state machine's state
 *
 * @return State
 */
flight_state_type_t sm_get_state(void);

#endif