#include "drivers/tools/time_tracker.h"
#include "pico/time.h"

TIME getMsSinceBoot()
{
	TIME time_ms = to_ms_since_boot(get_absolute_time());

	return time_ms;
}

TIME getUsSinceBoot()
{
	TIME time_us = to_us_since_boot(get_absolute_time());

	return time_us;
}

BOOL runEveryMs(TIME ms, TIME *timerOffset)
{
	if (getMsSinceBoot() - *timerOffset >= ms)
	{
		*timerOffset = getMsSinceBoot();

		return TRUE;
	}

	return FALSE;
}

BOOL runEveryUs(TIME us, TIME *timerOffset)
{
	if (getUsSinceBoot() - *timerOffset >= us)
	{
		*timerOffset = getUsSinceBoot();

		return TRUE;
	}

	return FALSE;
}

VOID sleepMiliseconds(TIME ms)
{
	sleep_ms(ms);
}

VOID sleepMicroseconds(TIME us)
{
	sleep_us(us);
}