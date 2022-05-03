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

char *getTimestamp()
{
	uint32_t time_ms = getMsSinceBoot();
	uint minutes = floor(time_ms / 60000);
	uint seconds = floor(time_ms / 1000 - minutes * 60);
	uint miliseconds = floor(time_ms - minutes * 60000 - seconds * 1000);

	char *buffer = malloc(32);

	snprintf(buffer, 16, "%u:%u:%u", minutes, seconds, miliseconds);

	return buffer;
}