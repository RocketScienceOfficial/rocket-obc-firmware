#ifndef _BOARD_CONTROL_H
#define _BOARD_CONTROL_H

#include "time_tracker.h"

/**
 * @brief Initialize the board
 *
 * @param sleepTimeMS The time to sleep after startup
 */
void board_init(time_t sleepTimeMS);

/**
 * @brief Tight loop
 */
void tight_loop(void);

#endif