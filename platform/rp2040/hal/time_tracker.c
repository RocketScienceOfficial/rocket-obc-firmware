#include "hal/time_tracker.h"
#include "pico/time.h"

msec_t hal_time_get_ms_since_boot()
{
	return to_ms_since_boot(get_absolute_time());
}

usec_t hal_time_get_us_since_boot()
{
	return to_us_since_boot(get_absolute_time());
}

bool hal_time_run_every_ms(msec_t ms, msec_t *timerOffset)
{
	if (hal_time_get_ms_since_boot() - *timerOffset >= ms)
	{
		*timerOffset = hal_time_get_ms_since_boot();

		return true;
	}

	return false;
}

bool hal_time_run_every_us(usec_t us, usec_t *timerOffset)
{
	if (hal_time_get_us_since_boot() - *timerOffset >= us)
	{
		*timerOffset = hal_time_get_us_since_boot();

		return true;
	}

	return false;
}

void hal_time_sleep_ms(msec_t ms)
{
	sleep_ms(ms);
}

void hal_time_sleep_us(usec_t us)
{
	sleep_us(us);
}