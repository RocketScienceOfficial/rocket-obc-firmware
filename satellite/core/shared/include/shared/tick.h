#pragma once

#include "utils/time_tracker.h"

#define TICK_DELAY_MS 500

/**
 * @brief Tick data.
 */
typedef struct TickData
{
    /**
     * @brief Timer used to periodicly tick.
     */
    Timer _timer;
} TickData;

/**
 * @brief Tick in loop to make sure it's running.
 */
void tick(TickData *data);