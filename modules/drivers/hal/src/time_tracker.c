#include "modules/drivers/hal/time_tracker.h"
#include "pico/time.h"

time_t time_get_ms_since_boot()
{
	return to_ms_since_boot(get_absolute_time());
}

time_t time_get_us_since_boot()
{
	return to_us_since_boot(get_absolute_time());
}

bool time_run_every_ms(time_t ms, time_t *timerOffset)
{
	if (time_get_ms_since_boot() - *timerOffset >= ms)
	{
		*timerOffset = time_get_ms_since_boot();

		return true;
	}

	return false;
}

bool time_run_every_us(time_t us, time_t *timerOffset)
{
	if (time_get_us_since_boot() - *timerOffset >= us)
	{
		*timerOffset = time_get_us_since_boot();

		return true;
	}

	return false;
}

void time_sleep_ms(time_t ms)
{
	sleep_ms(ms);
}

void time_sleep_us(time_t us)
{
	sleep_us(us);
}