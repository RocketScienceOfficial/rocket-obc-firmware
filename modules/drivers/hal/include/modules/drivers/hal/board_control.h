#ifndef _BOARD_CONTROL_H
#define _BOARD_CONTROL_H

#include "time_tracker.h"

/**
 * @brief Initialize the board
 *
 * @param sleepTimeMs The time to sleep after startup
 */
void hal_board_init(msec_t sleepTimeMs);

/**
 * @brief Tight loop
 */
void hal_tight_loop(void);

#endif