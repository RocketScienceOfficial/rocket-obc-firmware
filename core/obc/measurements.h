#ifndef _MEASUREMENTS_H
#define _MEASUREMENTS_H

#include "modules/flight_sm/flight_sm_control.h"
#include "modules/drivers/hal/time_tracker.h"

typedef struct measurement_data
{
    vec3_t acc1;
    vec3_t acc2;
    vec3_t acc3;
    vec3_t gyro1;
    vec3_t gyro2;
    vec3_t mag1;
    float press;
    float temp;
    double lat;
    double lon;
    float alt;
} measurement_data_t;

void measurements_init(void);
void measurements_get(measurement_data_t *data);

usec_t measurements_get_rate(const flight_sm_data_t *sm);
bool measurements_is_save(const flight_sm_data_t *sm);

#endif