#pragma once

#include <obc/api.h>

/**
 * @brief Time tracker definition.
 */
typedef UINT32 TIME;

/**
 * @brief Gets the current time in milliseconds.
 *
 * @return Current time in milliseconds.
 */
TIME getMsSinceBoot();

/**
 * @brief periodically checks if the time has passed.
 *
 * @param ms Time in milliseconds.
 * @param timerOffset Start time offset.
 *
 * @return True if the time has passed, false otherwise.
 */
BOOL runEvery(TIME ms, TIME *timerOffset);