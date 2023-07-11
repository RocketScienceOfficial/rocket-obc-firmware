#pragma once

#include <obc/api.h>
#include "maths/vector.h"
#include "maths/quaternion.h"

typedef struct KalmanFilterConfig
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
} KalmanFilterConfig;

typedef struct KalmanFilterInputData
{
    vec3 acc;
    vec3 gyro;
    vec3 mag;
    vec3 gpsPos;
    FLOAT pressure;
    FLOAT temperature;
} KalmanFilterInputData;

typedef struct KalmanFilterOutputData
{
    vec3 pos;
    vec3 vel;
    quat q;
} KalmanFilterOutputData;

typedef struct KalmanFilterState
{
    KalmanFilterConfig config;
    FLOAT dt2;
    FLOAT dt3;
    FLOAT dt4;
    FLOAT dt5;
    KalmanFilterOutputData stateVector;
    FLOAT stateCovariance[10][10];
    FLOAT measurementVector[11];
    FLOAT stateTransitionMatrix[10][10];
    FLOAT processNoiseCovariance[10][10];
    FLOAT measurementNoiseCovariance[11][11];
    FLOAT kalmanGain[10][11];
} KalmanFilterState;

VOID kalmanFilterInit(KalmanFilterState *state, KalmanFilterConfig *config);
VOID kalmanFilterUpdate(KalmanFilterState *state, KalmanFilterInputData *inputData, KalmanFilterOutputData *pOutputData);

VOID __kalmanFilterPredictState(KalmanFilterState *state, KalmanFilterInputData *inputData);
VOID __kalmanFilterPredictCovariance(KalmanFilterState *state);
VOID __kalmanFilterUpdateState(KalmanFilterState *state, KalmanFilterInputData *inputData);
VOID __kalmanFilterUpdateCovariance(KalmanFilterState *state);
VOID __kalmanFilterUpdateGain(KalmanFilterState *state, KalmanFilterInputData *inputData);