#ifndef _EKF_H
#define _EKF_H

#include "modules/maths/vector.h"
#include "modules/maths/quaternion.h"

#define EKF_NUM_STATES 22
#define EKF_NUM_CONTROLS 6

typedef struct ekf_state
{
    quat_t orientation;
    vec3_t velocity_ned;
    vec3_t position_ned;
    vec3_t gyroBias;
    vec3_t accelBias;
    vec3_t mag_ned;
    vec3_t magBias;
} ekf_state_t;

typedef struct ekf_controls
{
    vec3_t accel;
    vec3_t gyro;
} ekf_controls_t;

typedef struct ekf_inputs
{
    vec3_t gps;
    vec3_t mag;
    float baroHeight;
} ekf_inputs_t;

typedef struct ekf_config
{
    float dt;
} ekf_config_t;

typedef struct ekf_data
{
    ekf_state_t state;
    ekf_config_t config;
    float P[EKF_NUM_STATES][EKF_NUM_STATES];
} ekf_data_t;

void ekf_init(ekf_data_t *data, ekf_state_t *startingState);
void ekf_predict_state(ekf_data_t *data, ekf_controls_t *controls);
void ekf_predict_covariance(ekf_data_t *data);
void ekf_update_gain(ekf_data_t *data);
void ekf_update_state(ekf_data_t *data);
void ekf_update_covariance(ekf_data_t *data);

#endif