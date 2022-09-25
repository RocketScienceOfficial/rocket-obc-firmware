#include "tools/time_tracker.h"
#include "pico/time.h"

TIME getMsSinceBoot()
{
	TIME time_ms = to_ms_since_boot(get_absolute_time());

	return time_ms;
}

BOOL runEvery(TIME ms, TIME* timerOffset)
{
	if (getMsSinceBoot() - *timerOffset >= ms)
	{
		*timerOffset = getMsSinceBoot();

		return TRUE;
	}

	return FALSE;
}