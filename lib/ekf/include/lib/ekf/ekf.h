#ifndef _EKF_H
#define _EKF_H

#include "lib/maths/vector.h"
#include "lib/maths/quaternion.h"

#define EKF_NUM_STATES 22      /** Number of states */
#define EKF_NUM_CONTROLS 6     /** Number of control variables */
#define EKF_NUM_MEASUREMENTS 7 /** Number of measurements */

/**
 * @brief EKF state structure
 */
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

/**
 * @brief EKF controls structure
 */
typedef struct ekf_controls
{
    vec3_t accel;
    vec3_t gyro;
} ekf_controls_t;

/**
 * @brief EKF inputs structure
 */
typedef struct ekf_inputs
{
    vec3_t gps;
    vec3_t mag;
    float baroHeight;
} ekf_inputs_t;

/**
 * @brief EKF config
 */
typedef struct ekf_config
{
    float dt;
    float accelVariance;
    float gyroVariance;
    float gpsVariance;
    float baroVariance;
    float magVariance;
} ekf_config_t;

/**
 * @brief EKF data
 */
typedef struct ekf_data
{
    ekf_state_t state;
    ekf_config_t config;
    float P[EKF_NUM_STATES][EKF_NUM_STATES];
    float P_Next[EKF_NUM_STATES][EKF_NUM_STATES];
    float H[EKF_NUM_MEASUREMENTS][EKF_NUM_STATES];
    float K[EKF_NUM_STATES][EKF_NUM_MEASUREMENTS];
    float KH[EKF_NUM_STATES][EKF_NUM_STATES];
} ekf_data_t;

/**
 * @brief Initialize EKF variables
 *
 * @param data Pointer to data
 * @param config Filter configuration
 * @param startingState Starting state
 */
void ekf_init(ekf_data_t *data, ekf_config_t config, ekf_state_t startingState);

/**
 * @brief Predict next state
 *
 * @param data Pointer to data
 * @param controls Pointer to controls to provide more intormation for filter
 */
void ekf_predict_state(ekf_data_t *data, const ekf_controls_t *controls);

/**
 * @brief Predict covariance
 *
 * @param data Pointer to data
 * @param controls Pointer to controls to provide more intormation for filter
 */
void ekf_predict_covariance(ekf_data_t *data, const ekf_controls_t *controls);

/**
 * @brief Fuse data frame
 *
 * @param data Pointer to data
 * @param inputs Filter inputs
 */
void ekf_fuse(ekf_data_t *data, ekf_inputs_t *inputs);

#endif