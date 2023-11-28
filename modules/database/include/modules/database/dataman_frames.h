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
    vec3_t accel;
    vec3_t gyro;
    vec3_t mag;
} dataman_sensor_frame_t;

#endif