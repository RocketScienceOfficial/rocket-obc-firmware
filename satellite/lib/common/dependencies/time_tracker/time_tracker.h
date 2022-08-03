#pragma once

#include <stdint.h>

typedef uint32_t timer_t;

timer_t getMsSinceBoot();
int runEvery(timer_t ms, timer_t* timerOffset);