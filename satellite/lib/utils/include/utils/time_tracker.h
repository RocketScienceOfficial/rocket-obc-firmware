#pragma once

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Time tracker definition.
 */
typedef uint32_t Timer;

/**
 * @brief Gets the current time in milliseconds.
 *
 * @return Current time in milliseconds.
 */
Timer getMsSinceBoot();

/**
 * @brief periodically checks if the time has passed.
 *
 * @param ms Time in milliseconds.
 * @param timerOffset Start time offset.
 *
 * @return True if the time has passed, false otherwise.
 */
bool runEvery(Timer ms, Timer *timerOffset);