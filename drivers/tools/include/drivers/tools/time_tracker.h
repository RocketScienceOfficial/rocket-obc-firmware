#pragma once

#include <obc/api.h>

typedef UINT64 TIME; /** Time tracker definition */

/**
 * @brief Gets the current time in milliseconds
 *
 * @return Current time in milliseconds
 */
TIME getMsSinceBoot();

/**
 * @brief Gets the current time in microseconds
 *
 * @return Current time in microseconds
 */
TIME getUsSinceBoot();

/**
 * @brief Periodically checks if the time has passed
 *
 * @param ms Time in milliseconds
 * @param timerOffset Start time offset
 * @return True if the time has passed, false otherwise
 */
BOOL runEveryMs(TIME ms, TIME *timerOffset);

/**
 * @brief Periodically checks if the time has passed
 *
 * @param us Time in microseconds
 * @param timerOffset Start time offset
 * @return True if the time has passed, false otherwise
 */
BOOL runEveryUs(TIME us, TIME *timerOffset);

/**
 * @brief Sleeps for a given number of milliseconds
 *
 * @param ms Time in milliseconds
 */
VOID sleepMiliseconds(TIME ms);

/**
 * @brief Sleeps for a given number of microseconds
 *
 * @param us Time in microseconds
 */
VOID sleepMicroseconds(TIME us);