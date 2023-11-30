#ifndef _MEASUREMENTS_H
#define _MEASUREMENTS_H

#include "modules/maths/vector.h"
#include "modules/nmea/nmea_parser.h"

typedef struct measurement_data
{
    vec3_t acc1;
    vec3_t acc2;
    vec3_t acc3;
    vec3_t gyro1;
    vec3_t gyro2;
    vec3_t mag;
    float press;
    float temp;
    double lat;
    double lon;
    float alt;
} measurement_data_t;

void measurements_init(void);
void measurements_get(measurement_data_t *data);

#endif