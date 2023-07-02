#pragma once

#include <obc/api.h>
#include "time_tracker.h"

/**
 * @brief Initialize the board
 *
 * @param sleepTimeMS The time to sleep after startup
 */
VOID boardInit(TIME sleepTimeMS);

/**
 * @brief Tight loop
 */
VOID tightLoop();