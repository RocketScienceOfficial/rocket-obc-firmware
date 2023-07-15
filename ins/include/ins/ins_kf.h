#pragma once

#include <obc/api.h>
#include "maths/vector.h"
#include "maths/quaternion.h"

typedef struct INSKalmanFilterConfig
{
    FLOAT dt;
    FLOAT accelVariance;
    FLOAT gpsPosVariance;
    FLOAT pressureVariance;
    FLOAT temperatureVariance;
} INSKalmanFilterConfig;

typedef struct INSKalmanFilterInputData
{
    vec3 acc;
    vec3 gpsPos;
    FLOAT pressure;
    FLOAT temperature;
    quat orientation;
} INSKalmanFilterInputData;

typedef struct INSKalmanFilterOutputData
{
    vec3 pos;
    vec3 vel;
} INSKalmanFilterOutputData;

typedef struct INSKalmanFilterState
{
    INSKalmanFilterConfig config;
    FLOAT dt2;
    FLOAT dt3;
    FLOAT dt4;
    FLOAT dt5;
    INSKalmanFilterOutputData stateVector;
    FLOAT P[6][6];
    FLOAT R[5][5];
    FLOAT K[6][5];
    FLOAT H[5][6];
    FLOAT I_KH[6][6];
    FLOAT I_KHP[6][6];
    FLOAT I_KHP_I_HK_[6][6];
    FLOAT KR[6][5];
    FLOAT PH_[6][5];
    FLOAT HPH_R[5][5];
    FLOAT HPH_R_INV[5][5];
} INSKalmanFilterState;

VOID insKalmanFilterInit(INSKalmanFilterState *state, INSKalmanFilterConfig *config);
VOID insKalmanFilterUpdate(INSKalmanFilterState *state, INSKalmanFilterInputData *inputData, INSKalmanFilterOutputData *pOutputData);

VOID __insKalmanFilterPredictState(INSKalmanFilterState *state, INSKalmanFilterInputData *inputData);
VOID __insKalmanFilterPredictCovariance(INSKalmanFilterState *state);
VOID __insKalmanFilterUpdateState(INSKalmanFilterState *state, INSKalmanFilterInputData *inputData);
VOID __insKalmanFilterUpdateCovariance(INSKalmanFilterState *state);
VOID __insKalmanFilterUpdateGain(INSKalmanFilterState *state, INSKalmanFilterInputData *inputData);