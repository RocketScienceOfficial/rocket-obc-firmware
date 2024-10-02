#ifndef _BOARD_CONTROL_H
#define _BOARD_CONTROL_H

#include "time_tracker.h"

/**
 * @brief Initialize the board
 *
 * @param sleepTimeMs The time to sleep after startup
 * @return true if success
 */
bool hal_board_init(hal_msec_t sleepTimeMs);

/**
 * @brief Tight loop
 */
void hal_tight_loop(void);

#endif