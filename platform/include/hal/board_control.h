#ifndef _BOARD_CONTROL_H
#define _BOARD_CONTROL_H

#include "time_tracker.h"
#include <stdbool.h>

/**
 * @brief Initialize the board
 *
 * @param sleepTimeMs The time to sleep after startup
 * @param enableCRLF Should the automatically translate message sent through serial to CRLF
 * @return true if success
 */
bool hal_board_init(hal_msec_t sleepTimeMs, bool enableCRLF);

/**
 * @brief Tight loop
 */
void hal_board_tight_loop(void);

/**
 * @brief Check if USB is connected to the board
 *
 * @return true if connected
 */
bool hal_board_is_usb_connected(void);

#endif