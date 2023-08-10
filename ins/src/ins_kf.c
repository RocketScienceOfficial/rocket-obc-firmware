#include "ins/ins_kf.h"
#include "maths/constants.h"
#include "maths/spherical_geo.h"
#include <math.h>
#include <string.h>

#define INIT_STATE_COVARIANCE 100.0f

VOID insKalmanFilterInit(INSKalmanFilterState *state, FLOAT dt, INSKalmanFilterOutputData *baseState, INSKalmanFilterVariances *variances)
{
    memset(state, 0, sizeof(*state));

    state->dt = dt;
    state->dt2 = state->dt * state->dt;
    state->dt3 = state->dt2 * state->dt;
    state->dt4 = state->dt3 * state->dt;
    state->dt5 = state->dt4 * state->dt;

    state->stateVector = *baseState;

    for (SIZE i = 0; i < INS_KALMAN_FILTER_STATE_DIMENSION; i++)
    {
        state->P[i][i] = INIT_STATE_COVARIANCE;
    }

    insKalmanFilterUpdateVariances(state, variances);

    INSKalmanFilterInputData tempInputData = {0};

    __insKalmanFilterPredictState(state, &tempInputData);
    __insKalmanFilterRecalculateFJacobian(state);
    __insKalmanFilterPredictCovariance(state);
}

VOID insKalmanFilterUpdate(INSKalmanFilterState *state, INSKalmanFilterInputData *inputData, INSKalmanFilterOutputData *pOutputData)
{
    __insKalmanFilterRecalculateHJacobian(state);

    __insKalmanFilterUpdateGain(state);
    __insKalmanFilterUpdateState(state, inputData);
    __insKalmanFilterUpdateCovariance(state);

    *pOutputData = state->stateVector;

    __insKalmanFilterPredictState(state, inputData);
    //__insKalmanFilterRecalculateFJacobian(state);
    __insKalmanFilterPredictCovariance(state);
}

VOID insKalmanFilterUpdateVariances(INSKalmanFilterState *state, INSKalmanFilterVariances *variances)
{
    state->Q[0][0] = variances->accelVariance * state->dt5 / 20.0f;
    state->Q[0][3] = variances->accelVariance * state->dt4 / 8.0f;
    state->Q[1][1] = variances->accelVariance * state->dt5 / 20.0f;
    state->Q[1][4] = variances->accelVariance * state->dt4 / 8.0f;
    state->Q[2][2] = variances->accelVariance * state->dt5 / 20.0f;
    state->Q[2][5] = variances->accelVariance * state->dt4 / 8.0f;
    state->Q[3][0] = variances->accelVariance * state->dt4 / 8.0f;
    state->Q[3][3] = variances->accelVariance * state->dt3 / 3.0f;
    state->Q[4][1] = variances->accelVariance * state->dt4 / 8.0f;
    state->Q[4][4] = variances->accelVariance * state->dt3 / 3.0f;
    state->Q[5][2] = variances->accelVariance * state->dt4 / 8.0f;
    state->Q[5][5] = variances->accelVariance * state->dt3 / 3.0f;

    state->R[0][0] = variances->gpsPosVariance;
    state->R[1][1] = variances->gpsPosVariance;
    state->R[2][2] = variances->gpsPosVariance;
    state->R[3][3] = variances->pressureVariance;
    state->R[4][4] = variances->temperatureVariance;
}

VOID __insKalmanFilterPredictState(INSKalmanFilterState *state, INSKalmanFilterInputData *inputData)
{
    state->stateVector.pos.x += state->stateVector.vel.x * state->dt + 0.5f * inputData->acc.x * state->dt2;
    state->stateVector.pos.y += state->stateVector.vel.y * state->dt + 0.5f * inputData->acc.y * state->dt2;
    state->stateVector.pos.z += state->stateVector.vel.z * state->dt + 0.5f * inputData->acc.z * state->dt2;
    state->stateVector.vel.x += inputData->acc.x * state->dt;
    state->stateVector.vel.y += inputData->acc.y * state->dt;
    state->stateVector.vel.z += inputData->acc.z * state->dt;
}

VOID __insKalmanFilterPredictCovariance(INSKalmanFilterState *state)
{
    SIZE i, j, k;

    for (i = 0; i < INS_KALMAN_FILTER_STATE_DIMENSION; i++)
    {
        for (j = 0; j < INS_KALMAN_FILTER_STATE_DIMENSION; j++)
        {
            state->FP[i][j] = 0.0f;

            for (k = 0; k < INS_KALMAN_FILTER_STATE_DIMENSION; k++)
            {
                state->FP[i][j] += state->F[i][k] * state->P[k][j];
            }
        }
    }

    for (i = 0; i < INS_KALMAN_FILTER_STATE_DIMENSION; i++)
    {
        for (j = 0; j < INS_KALMAN_FILTER_STATE_DIMENSION; j++)
        {
            state->P[i][j] = state->Q[i][j];

            for (k = 0; k < INS_KALMAN_FILTER_STATE_DIMENSION; k++)
            {
                state->P[i][j] += state->FP[i][k] * state->F[j][k];
            }
        }
    }
}

VOID __insKalmanFilterUpdateState(INSKalmanFilterState *state, INSKalmanFilterInputData *inputData)
{
    state->h[0] = state->stateVector.pos.x;
    state->h[1] = state->stateVector.pos.y;
    state->h[2] = state->stateVector.pos.z;
    state->h[3] = SEA_LEVEL_PRESSURE * powf(1.0f - (STANDARD_LAPSE_RATE * state->stateVector.pos.z) / SEA_LEVEL_TEMPERATURE, 5.25588f);
    state->h[4] = SEA_LEVEL_TEMPERATURE - STANDARD_LAPSE_RATE * state->stateVector.pos.z;

    state->v[0] = inputData->gpsPos.x - state->h[0];
    state->v[1] = inputData->gpsPos.y - state->h[1];
    state->v[2] = inputData->gpsPos.z - state->h[2];
    state->v[3] = inputData->pressure - state->h[3];
    state->v[4] = inputData->temperature - state->h[4];

    __insKalmanFilterUpdateStateRevolution(state);
}

VOID __insKalmanFilterUpdateCovariance(INSKalmanFilterState *state)
{
    SIZE i, j, k;
    FLOAT temp;

    for (i = 0; i < INS_KALMAN_FILTER_STATE_DIMENSION; i++)
    {
        for (j = 0; j < INS_KALMAN_FILTER_STATE_DIMENSION; j++)
        {
            state->I_KH[i][j] = 0.0f;

            for (k = 0; k < INS_KALMAN_FILTER_MEASURE_DIMENSION; k++)
            {
                state->I_KH[i][j] += state->K[i][k] * state->H[k][j];
            }

            if (i == j)
            {
                state->I_KH[i][j] = 1.0f - state->I_KH[i][j];
            }
            else
            {
                state->I_KH[i][j] = -state->I_KH[i][j];
            }
        }
    }

    for (i = 0; i < INS_KALMAN_FILTER_STATE_DIMENSION; i++)
    {
        for (j = 0; j < INS_KALMAN_FILTER_STATE_DIMENSION; j++)
        {
            state->I_KHP[i][j] = 0.0f;

            for (k = 0; k < INS_KALMAN_FILTER_STATE_DIMENSION; k++)
            {
                state->I_KHP[i][j] += state->I_KH[i][k] * state->P[k][j];
            }
        }
    }

    for (i = 0; i < INS_KALMAN_FILTER_STATE_DIMENSION; i++)
    {
        for (j = 0; j < INS_KALMAN_FILTER_STATE_DIMENSION; j++)
        {
            state->I_KHP_I_KH_[i][j] = 0.0f;

            for (k = 0; k < INS_KALMAN_FILTER_STATE_DIMENSION; k++)
            {
                state->I_KHP_I_KH_[i][j] += state->I_KHP[i][k] * state->I_KH[j][k];
            }
        }
    }

    for (i = 0; i < INS_KALMAN_FILTER_STATE_DIMENSION; i++)
    {
        for (j = 0; j < INS_KALMAN_FILTER_MEASURE_DIMENSION; j++)
        {
            state->KR[i][j] = 0.0f;

            for (k = 0; k < INS_KALMAN_FILTER_MEASURE_DIMENSION; k++)
            {
                state->KR[i][j] += state->K[i][k] * state->R[k][j];
            }
        }
    }

    for (i = 0; i < INS_KALMAN_FILTER_STATE_DIMENSION; i++)
    {
        for (j = 0; j < INS_KALMAN_FILTER_STATE_DIMENSION; j++)
        {
            temp = 0.0f;

            for (k = 0; k < INS_KALMAN_FILTER_MEASURE_DIMENSION; k++)
            {
                temp += state->KR[i][k] * state->K[j][k];
            }

            state->P[i][j] = state->I_KHP_I_KH_[i][j] + temp;
        }
    }
}

VOID __insKalmanFilterUpdateGain(INSKalmanFilterState *state)
{
    SIZE i, j, k;

    for (i = 0; i < INS_KALMAN_FILTER_STATE_DIMENSION; i++)
    {
        for (j = 0; j < INS_KALMAN_FILTER_MEASURE_DIMENSION; j++)
        {
            state->PH_[i][j] = 0.0f;

            for (k = 0; k < INS_KALMAN_FILTER_STATE_DIMENSION; k++)
            {
                state->PH_[i][j] += state->P[i][k] * state->H[j][k];
            }
        }
    }

    for (i = 0; i < INS_KALMAN_FILTER_MEASURE_DIMENSION; i++)
    {
        for (j = 0; j < INS_KALMAN_FILTER_MEASURE_DIMENSION; j++)
        {
            state->HPH_R[i][j] = 0.0f;

            if (i == j)
            {
                state->HPH_R_INV[i][j] = 1.0f;
            }
            else
            {
                state->HPH_R_INV[i][j] = 0.0f;
            }

            for (k = 0; k < INS_KALMAN_FILTER_STATE_DIMENSION; k++)
            {
                state->HPH_R[i][j] += state->H[i][k] * state->PH_[k][j];
            }

            state->HPH_R[i][j] += state->R[i][j];
        }
    }

    __insKalmanFilterCalculateGainTempInverse(state);

    for (i = 0; i < INS_KALMAN_FILTER_STATE_DIMENSION; i++)
    {
        for (j = 0; j < INS_KALMAN_FILTER_MEASURE_DIMENSION; j++)
        {
            state->K[i][j] = 0.0f;

            for (k = 0; k < INS_KALMAN_FILTER_MEASURE_DIMENSION; k++)
            {
                state->K[i][j] += state->PH_[i][k] * state->HPH_R_INV[k][j];
            }
        }
    }
}

VOID __insKalmanFilterCalculateGainTempInverse(INSKalmanFilterState *state)
{
    SIZE i, j, k;
    FLOAT temp;

    for (i = 0; i < INS_KALMAN_FILTER_MEASURE_DIMENSION - 1; i++)
    {
        SIZE pivot = i;
        FLOAT pivotSize = state->HPH_R[i][i];

        if (pivotSize < 0)
        {
            pivotSize = -pivotSize;
        }

        for (j = i + 1; j < INS_KALMAN_FILTER_MEASURE_DIMENSION; j++)
        {
            temp = state->HPH_R[j][i];

            if (temp < 0)
            {
                temp = -temp;
            }

            if (temp > pivotSize)
            {
                pivot = j;
                pivotSize = temp;
            }
        }

        if (pivotSize == 0)
        {
            return;
        }

        if (pivot != i)
        {
            for (j = 0; j < INS_KALMAN_FILTER_MEASURE_DIMENSION; j++)
            {
                temp = state->HPH_R[i][j];

                state->HPH_R[i][j] = state->HPH_R[pivot][j];
                state->HPH_R[pivot][j] = temp;

                temp = state->HPH_R_INV[i][j];

                state->HPH_R_INV[i][j] = state->HPH_R_INV[pivot][j];
                state->HPH_R_INV[pivot][j] = temp;
            }
        }

        for (j = i + 1; j < INS_KALMAN_FILTER_MEASURE_DIMENSION; j++)
        {
            temp = state->HPH_R[j][i] / state->HPH_R[i][i];

            for (k = 0; k < INS_KALMAN_FILTER_MEASURE_DIMENSION; k++)
            {
                state->HPH_R[j][k] -= temp * state->HPH_R[i][k];
                state->HPH_R_INV[j][k] -= temp * state->HPH_R_INV[i][k];
            }

            state->HPH_R[j][i] = 0.0f;
        }
    }

    for (i = 0; i < INS_KALMAN_FILTER_MEASURE_DIMENSION; i++)
    {
        temp = state->HPH_R[i][i];

        for (j = 0; j < INS_KALMAN_FILTER_MEASURE_DIMENSION; j++)
        {
            state->HPH_R[i][j] = state->HPH_R[i][j] / temp;
            state->HPH_R_INV[i][j] = state->HPH_R_INV[i][j] / temp;
        }

        state->HPH_R[i][i] = 1.0f;
    }

    for (i = 0; i < INS_KALMAN_FILTER_MEASURE_DIMENSION - 1; i++)
    {
        for (j = i + 1; j < INS_KALMAN_FILTER_MEASURE_DIMENSION; j++)
        {
            temp = state->HPH_R[i][j];

            for (k = 0; k < INS_KALMAN_FILTER_MEASURE_DIMENSION; k++)
            {
                state->HPH_R[i][k] -= state->HPH_R[j][k] * temp;
                state->HPH_R_INV[i][k] -= state->HPH_R_INV[j][k] * temp;
            }

            state->HPH_R[i][j] = 0.0f;
        }
    }
}

VOID __insKalmanFilterUpdateStateRevolution(INSKalmanFilterState *state)
{
    for (SIZE i = 0; i < INS_KALMAN_FILTER_STATE_DIMENSION; i++)
    {
        for (SIZE j = 0; j < INS_KALMAN_FILTER_MEASURE_DIMENSION; j++)
        {
            state->x[i] += state->K[i][j] * state->v[j];
        }
    }
}

VOID __insKalmanFilterRecalculateFJacobian(INSKalmanFilterState *state)
{
    memset(state->F, 0, sizeof(state->F));

    for (SIZE i = 0; i < INS_KALMAN_FILTER_STATE_DIMENSION; i++)
    {
        state->F[i][i] = 1.0f;
    }

    state->F[0][3] = state->dt;
    state->F[1][4] = state->dt;
    state->F[2][5] = state->dt;
}

VOID __insKalmanFilterRecalculateHJacobian(INSKalmanFilterState *state)
{
    memset(state->H, 0, sizeof(state->H));

    state->H[0][0] = 1.0f;
    state->H[1][1] = 1.0f;
    state->H[2][2] = 1.0f;
    state->H[3][2] = 12.0131f * powf(1.0f - (STANDARD_LAPSE_RATE * state->stateVector.pos.z) / SEA_LEVEL_TEMPERATURE, 4.25588f);
    state->H[4][2] = -STANDARD_LAPSE_RATE;
}