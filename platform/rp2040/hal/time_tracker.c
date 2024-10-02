#include "hal/time_tracker.h"
#include "pico/time.h"

hal_msec_t hal_time_get_ms_since_boot()
{
	return to_ms_since_boot(get_absolute_time());
}

hal_usec_t hal_time_get_us_since_boot()
{
	return to_us_since_boot(get_absolute_time());
}

bool hal_time_run_every_ms(hal_msec_t ms, hal_msec_t *timerOffset)
{
	hal_msec_t currentTime = hal_time_get_ms_since_boot();

	if (currentTime - *timerOffset >= ms)
	{
		*timerOffset = currentTime;

		return true;
	}

	return false;
}

bool hal_time_run_every_us(hal_usec_t us, hal_usec_t *timerOffset)
{
	hal_usec_t currentTime = hal_time_get_us_since_boot();

	if (currentTime - *timerOffset >= us)
	{
		*timerOffset = currentTime;

		return true;
	}

	return false;
}

void hal_time_sleep_ms(hal_msec_t ms)
{
	sleep_ms(ms);
}

void hal_time_sleep_us(hal_usec_t us)
{
	sleep_us(us);
}