#pragma once

#include <obc/api.h>
#include "maths/vector.h"
#include "maths/quaternion.h"

typedef struct AHRSKalmanFilterConfig
{
    FLOAT dt;
    FLOAT accelXVariance;
    FLOAT accelYVariance;
    FLOAT accelZVariance;
    FLOAT accelXYCovariance;
    FLOAT accelXZCovariance;
    FLOAT accelYZCovariance;
    FLOAT gyroXVariance;
    FLOAT gyroYVariance;
    FLOAT gyroZVariance;
    FLOAT gyroXYCovariance;
    FLOAT gyroXZCovariance;
    FLOAT gyroYZCovariance;
    FLOAT magXVariance;
    FLOAT magYVariance;
    FLOAT magZVariance;
    FLOAT magXYCovariance;
    FLOAT magXZCovariance;
    FLOAT magYZCovariance;
    FLOAT gpsPosXVariance;
    FLOAT gpsPosYVariance;
    FLOAT gpsPosZVariance;
    FLOAT gpsPosXYCovariance;
    FLOAT gpsPosXZCovariance;
    FLOAT gpsPosYZCovariance;
    FLOAT pressureVariance;
    FLOAT temperatureVariance;
} AHRSKalmanFilterConfig;

typedef struct AHRSKalmanFilterInputData
{
    vec3 acc;
    vec3 gyro;
    vec3 mag;
    vec3 gpsPos;
    FLOAT pressure;
    FLOAT temperature;
} AHRSKalmanFilterInputData;

typedef struct AHRSKalmanFilterOutputData
{
    vec3 pos;
    vec3 vel;
    quat q;
} AHRSKalmanFilterOutputData;

typedef struct AHRSKalmanFilterState
{
    AHRSKalmanFilterConfig config;
    FLOAT dt2;
    FLOAT dt3;
    FLOAT dt4;
    FLOAT dt5;
    AHRSKalmanFilterOutputData stateVector;
    FLOAT stateCovariance[10][10];
    FLOAT measurementVector[11];
    FLOAT stateTransitionMatrix[10][10];
    FLOAT processNoiseCovariance[10][10];
    FLOAT measurementNoiseCovariance[11][11];
    FLOAT kalmanGain[10][11];
} AHRSKalmanFilterState;

VOID ahrsKalmanFilterInit(AHRSKalmanFilterState *state, AHRSKalmanFilterConfig *config);
VOID ahrsKalmanFilterUpdate(AHRSKalmanFilterState *state, AHRSKalmanFilterInputData *inputData, AHRSKalmanFilterOutputData *pOutputData);

VOID __ahrsKalmanFilterPredictState(AHRSKalmanFilterState *state, AHRSKalmanFilterInputData *inputData);
VOID __ahrsKalmanFilterPredictCovariance(AHRSKalmanFilterState *state);
VOID __ahrsKalmanFilterUpdateState(AHRSKalmanFilterState *state, AHRSKalmanFilterInputData *inputData);
VOID __ahrsKalmanFilterUpdateCovariance(AHRSKalmanFilterState *state);
VOID __ahrsKalmanFilterUpdateGain(AHRSKalmanFilterState *state, AHRSKalmanFilterInputData *inputData);