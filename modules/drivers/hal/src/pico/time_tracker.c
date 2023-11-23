#include "modules/drivers/hal/time_tracker.h"
#include "pico/time.h"

hal_time_t hal_time_get_ms_since_boot()
{
	return to_ms_since_boot(get_absolute_time());
}

hal_time_t hal_time_get_us_since_boot()
{
	return to_us_since_boot(get_absolute_time());
}

bool hal_time_run_every_ms(hal_time_t ms, hal_time_t *timerOffset)
{
	if (hal_time_get_ms_since_boot() - *timerOffset >= ms)
	{
		*timerOffset = hal_time_get_ms_since_boot();

		return true;
	}

	return false;
}

bool hal_time_run_every_us(hal_time_t us, hal_time_t *timerOffset)
{
	if (hal_time_get_us_since_boot() - *timerOffset >= us)
	{
		*timerOffset = hal_time_get_us_since_boot();

		return true;
	}

	return false;
}

void hal_time_sleep_ms(hal_time_t ms)
{
	sleep_ms(ms);
}

void hal_time_sleep_us(hal_time_t us)
{
	sleep_us(us);
}