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

/**
 * @brief Gets current base altitude. 0 if state == STANDING
 *
 * @return Base Altitude
 */
float sm_get_base_alt(void);

/**
 * @brief Gets apogee reached. 0 if hasn't reached yet
 *
 * @return Apogee
 */
float sm_get_apogee(void);

/**
 * @brief Check if state machine is armed
 *
 * @return True if state machine is armed
 */
bool sm_is_armed(void);

#endif