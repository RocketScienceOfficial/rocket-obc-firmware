#pragma once

#include <obc/api.h>
#include "maths/vector.h"
#include "maths/quaternion.h"

#define INS_KALMAN_FILTER_STATE_DIMENSION 6
#define INS_KALMAN_FILTER_INPUT_DIMENSION 3
#define INS_KALMAN_FILTER_MEASURE_DIMENSION 5

/**
 * @brief Kalman filter variances structure
 */
typedef struct INSKalmanFilterVariances
{
    FLOAT accelVariance;
    FLOAT gpsPosVariance;
    FLOAT pressureVariance;
    FLOAT temperatureVariance;
} INSKalmanFilterVariances;

/**
 * @brief Kalman filter input data
 */
typedef struct INSKalmanFilterInputData
{
    vec3 acc;
    vec3 gpsPos;
    FLOAT pressure;
    FLOAT temperature;
} INSKalmanFilterInputData;

/**
 * @brief Kalman filter output data
 */
typedef struct INSKalmanFilterOutputData
{
    vec3 pos;
    vec3 vel;
} INSKalmanFilterOutputData;

/**
 * @brief Kalman filter state
 */
typedef struct INSKalmanFilterState
{
    FLOAT dt;
    FLOAT dt2;
    FLOAT dt3;
    FLOAT dt4;
    FLOAT dt5;
    union
    {
        INSKalmanFilterOutputData stateVector;
        FLOAT x[INS_KALMAN_FILTER_STATE_DIMENSION];
    };
    FLOAT F[INS_KALMAN_FILTER_STATE_DIMENSION][INS_KALMAN_FILTER_STATE_DIMENSION];
    FLOAT P[INS_KALMAN_FILTER_STATE_DIMENSION][INS_KALMAN_FILTER_STATE_DIMENSION];
    FLOAT Q[INS_KALMAN_FILTER_STATE_DIMENSION][INS_KALMAN_FILTER_STATE_DIMENSION];
    FLOAT R[INS_KALMAN_FILTER_MEASURE_DIMENSION][INS_KALMAN_FILTER_MEASURE_DIMENSION];
    FLOAT K[INS_KALMAN_FILTER_STATE_DIMENSION][INS_KALMAN_FILTER_MEASURE_DIMENSION];
    FLOAT H[INS_KALMAN_FILTER_MEASURE_DIMENSION][INS_KALMAN_FILTER_STATE_DIMENSION];
    FLOAT FP[INS_KALMAN_FILTER_STATE_DIMENSION][INS_KALMAN_FILTER_STATE_DIMENSION];
    FLOAT h[INS_KALMAN_FILTER_MEASURE_DIMENSION];
    FLOAT v[INS_KALMAN_FILTER_MEASURE_DIMENSION];
    FLOAT I_KH[INS_KALMAN_FILTER_STATE_DIMENSION][INS_KALMAN_FILTER_STATE_DIMENSION];
    FLOAT I_KHP[INS_KALMAN_FILTER_STATE_DIMENSION][INS_KALMAN_FILTER_STATE_DIMENSION];
    FLOAT I_KHP_I_KH_[INS_KALMAN_FILTER_STATE_DIMENSION][INS_KALMAN_FILTER_STATE_DIMENSION];
    FLOAT KR[INS_KALMAN_FILTER_STATE_DIMENSION][INS_KALMAN_FILTER_MEASURE_DIMENSION];
    FLOAT PH_[INS_KALMAN_FILTER_STATE_DIMENSION][INS_KALMAN_FILTER_MEASURE_DIMENSION];
    FLOAT HPH_R[INS_KALMAN_FILTER_MEASURE_DIMENSION][INS_KALMAN_FILTER_MEASURE_DIMENSION];
    FLOAT HPH_R_INV[INS_KALMAN_FILTER_MEASURE_DIMENSION][INS_KALMAN_FILTER_MEASURE_DIMENSION];
} INSKalmanFilterState;

/**
 * @brief Initializes the Kalman filter
 *
 * @param state Kalman filter state
 * @param dt Delta time
 * @param baseState Kalman filter base state
 * @param variances Kalman filter variances
 */
VOID insKalmanFilterInit(INSKalmanFilterState *state, FLOAT dt, INSKalmanFilterOutputData *baseState, INSKalmanFilterVariances *variances);

/**
 * @brief Updates the Kalman filter
 *
 * @param state Kalman filter state
 * @param inputData Kalman filter input data
 * @param pOutputData Kalman filter output data
 */
VOID insKalmanFilterUpdate(INSKalmanFilterState *state, INSKalmanFilterInputData *inputData, INSKalmanFilterOutputData *pOutputData);

/**
 * @brief Updates the Kalman filter variances
 *
 * @param state Kalman filter state
 * @param variances Kalman filter variances
 */
VOID insKalmanFilterUpdateVariances(INSKalmanFilterState *state, INSKalmanFilterVariances *variances);

/**
 * @brief Predicts the Kalman state
 *
 * @param state Kalman filter state
 * @param inputData Kalman filter input data
 */
VOID __insKalmanFilterPredictState(INSKalmanFilterState *state, INSKalmanFilterInputData *inputData);

/**
 * @brief Predicts the Kalman covariance
 *
 * @param state Kalman filter state
 */
VOID __insKalmanFilterPredictCovariance(INSKalmanFilterState *state);

/**
 * @brief Updates the Kalman state
 *
 * @param state Kalman filter state
 * @param inputData Kalman filter input data
 */
VOID __insKalmanFilterUpdateState(INSKalmanFilterState *state, INSKalmanFilterInputData *inputData);

/**
 * @brief Updates the Kalman covariance
 *
 * @param state Kalman filter state
 */
VOID __insKalmanFilterUpdateCovariance(INSKalmanFilterState *state);

/**
 * @brief Updates the Kalman gain
 *
 * @param state Kalman filter state
 */
VOID __insKalmanFilterUpdateGain(INSKalmanFilterState *state);

/**
 * @brief Calculates the Kalman gain temp (HPH_ + R) inverse. See: https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/matrix-inverse/matrix-inverse.html
 *
 * @param state Kalman filter state
 */
VOID __insKalmanFilterCalculateGainTempInverse(INSKalmanFilterState *state);

/**
 * @brief Update state vector with revolution
 *
 * @param state Kalman filter state
 */
VOID __insKalmanFilterUpdateStateRevolution(INSKalmanFilterState *state);

/**
 * @brief Recalculates the F Jacobian
 *
 * @param state Kalman filter state
 */
VOID __insKalmanFilterRecalculateFJacobian(INSKalmanFilterState *state);

/**
 * @brief Recalculates the H Jacobian
 *
 * @param state Kalman filter state
 */
VOID __insKalmanFilterRecalculateHJacobian(INSKalmanFilterState *state);