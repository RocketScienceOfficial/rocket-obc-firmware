#include "time_tracker.h"
#include "pico/time.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

unsigned int getMsSinceBoot()
{
	uint32_t time_ms = to_ms_since_boot(get_absolute_time());

	return time_ms;
}

int runEvery(unsigned int ms, unsigned int* timerOffset)
{
	if (getMsSinceBoot() - *timerOffset >= ms)
	{
		*timerOffset = getMsSinceBoot();

		return 1;
	}

	return 0;
}

int runEvery_offset(unsigned int ms, unsigned int offset)
{
	static unsigned int timerOffset = 0;

	if (timerOffset == 0)
	{
		timerOffset = offset;
	}

	if (getMsSinceBoot() - timerOffset >= ms)
	{
		timerOffset = getMsSinceBoot();

		return 1;
	}

	return 0;
}