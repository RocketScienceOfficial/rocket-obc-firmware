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

void hal_time_sleep_ms(hal_msec_t ms)
{
	sleep_ms(ms);
}

void hal_time_sleep_us(hal_usec_t us)
{
	sleep_us(us);
}