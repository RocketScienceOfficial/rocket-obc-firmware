#pragma once

#include <obc/api.h>

/**
 * @brief Checks if the mission is ready to start.
 *
 * @return TRUE if the mission is ready to start, FALSE otherwise.
 */
BOOL isMissionReady(VOID);

/**
 * @brief Checks if the mission is done.
 *
 * @return TRUE if the mission is done, FALSE otherwise.
 */
BOOL isMissionDone(VOID);