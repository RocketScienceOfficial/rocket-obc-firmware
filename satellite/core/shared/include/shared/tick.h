#pragma once

#include "tools/time_tracker.h"

#define TICK_DELAY_MS 500

/**
 * @brief Tick data.
 */
typedef struct TickData
{
    /**
     * @brief Timer used to periodicly tick.
     */
    TIME _time;
} TickData;

/**
 * @brief Tick in loop to make sure it's running.
 */
VOID tick(TickData *data);