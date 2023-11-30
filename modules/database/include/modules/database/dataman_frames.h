#ifndef _DATAMAN_FRAMES_H
#define _DATAMAN_FRAMES_H

#include "modules/maths/vector.h"

/**
 * @brief List of available Dataman messages types
 */
typedef enum dataman_message_type
{
    DATAMAN_FRAME_SENSORS = 0,
} dataman_message_type_t;

/**
 * @brief Dataman Sensor Frame
 */
typedef struct dataman_sensor_frame
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
} dataman_sensor_frame_t;

#endif