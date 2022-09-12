#include "utils/time_tracker.h"
#include "pico/time.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

Timer getMsSinceBoot()
{
	Timer time_ms = to_ms_since_boot(get_absolute_time());

	return time_ms;
}

bool runEvery(Timer ms, Timer* timerOffset)
{
	if (getMsSinceBoot() - *timerOffset >= ms)
	{
		*timerOffset = getMsSinceBoot();

		return true;
	}

	return false;
}