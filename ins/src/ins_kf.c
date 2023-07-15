#include "ins/ins_kf.h"
#include "maths/constants.h"
#include <math.h>
#include <string.h>

VOID insKalmanFilterInit(INSKalmanFilterState *state, INSKalmanFilterConfig *config)
{
    state->config = *config;

    memset(&state->stateVector, 0, sizeof(state->stateVector));
    memset(&state->P, 0, sizeof(state->P));
    memset(&state->R, 0, sizeof(state->R));
    memset(&state->K, 0, sizeof(state->K));
    memset(&state->H, 0, sizeof(state->H));
    memset(&state->I_KH, 0, sizeof(state->I_KH));
    memset(&state->I_KHP, 0, sizeof(state->I_KHP));
    memset(&state->I_KHP_I_HK_, 0, sizeof(state->I_KHP_I_HK_));
    memset(&state->KR, 0, sizeof(state->KR));
    memset(&state->PH_, 0, sizeof(state->PH_));
    memset(&state->HPH_R, 0, sizeof(state->HPH_R));
    memset(&state->HPH_R_INV, 0, sizeof(state->HPH_R_INV));

    state->stateVector.pos.z = 110.0f;

    for (SIZE i = 0; i < 6; i++)
    {
        state->P[i][i] = 10000.0f;
    }

    state->dt2 = state->config.dt * state->config.dt;
    state->dt3 = state->dt2 * state->config.dt;
    state->dt4 = state->dt3 * state->config.dt;
    state->dt5 = state->dt4 * state->config.dt;

    state->R[0][0] = state->config.gpsPosVariance;
    state->R[1][1] = state->config.gpsPosVariance;
    state->R[2][2] = state->config.gpsPosVariance;
    state->R[3][3] = state->config.pressureVariance;
    state->R[4][4] = state->config.temperatureVariance;

    INSKalmanFilterInputData tempData = {0};

    __insKalmanFilterPredictState(state, &tempData);
    __insKalmanFilterPredictCovariance(state);
}

VOID insKalmanFilterUpdate(INSKalmanFilterState *state, INSKalmanFilterInputData *inputData, INSKalmanFilterOutputData *pOutputData)
{
    __insKalmanFilterUpdateGain(state, inputData);
    __insKalmanFilterUpdateState(state, inputData);
    __insKalmanFilterUpdateCovariance(state);

    *pOutputData = state->stateVector;

    __insKalmanFilterPredictState(state, inputData);
    __insKalmanFilterPredictCovariance(state);
}

VOID __insKalmanFilterPredictState(INSKalmanFilterState *state, INSKalmanFilterInputData *inputData)
{
    vec3 rotatedAcc = inputData->acc;

    rotateVectorThroughQuaternion(&rotatedAcc, &inputData->orientation);

    rotatedAcc.z -= EARTH_GRAVITY;

    state->stateVector.pos.x += state->stateVector.vel.x * state->config.dt + 0.5f * rotatedAcc.x * state->dt2;
    state->stateVector.pos.y += state->stateVector.vel.y * state->config.dt + 0.5f * rotatedAcc.y * state->dt2;
    state->stateVector.pos.z += state->stateVector.vel.z * state->config.dt + 0.5f * rotatedAcc.z * state->dt2;
    state->stateVector.vel.x += rotatedAcc.x * state->config.dt;
    state->stateVector.vel.y += rotatedAcc.y * state->config.dt;
    state->stateVector.vel.z += rotatedAcc.z * state->config.dt;
}

VOID __insKalmanFilterPredictCovariance(INSKalmanFilterState *state)
{
    state->P[0][0] += 2 * state->P[0][3] * state->config.dt + state->P[3][3] * state->dt2 + state->dt5 * state->config.accelVariance / 20.0f;
    state->P[0][3] += state->P[3][3] * state->config.dt + state->dt4 * state->config.accelVariance / 8.0f;
    state->P[1][1] += 2 * state->P[1][4] * state->config.dt + state->P[4][4] * state->dt2 + state->dt5 * state->config.accelVariance / 20.0f;
    state->P[1][4] += state->P[4][4] * state->config.dt + state->dt4 * state->config.accelVariance / 8.0f;
    state->P[2][2] += 2 * state->P[2][5] * state->config.dt + state->P[5][5] * state->dt2 + state->dt5 * state->config.accelVariance / 20.0f;
    state->P[2][5] += state->P[5][5] * state->config.dt + state->dt4 * state->config.accelVariance / 8.0f;
    state->P[3][0] += state->P[3][3] * state->config.dt + state->dt4 * state->config.accelVariance / 8.0f;
    state->P[3][3] += state->dt3 * state->config.accelVariance / 3.0f;
    state->P[4][1] += state->P[4][4] * state->config.dt + state->dt4 * state->config.accelVariance / 8.0f;
    state->P[4][4] += state->dt3 * state->config.accelVariance / 3.0f;
    state->P[5][2] += state->P[5][5] * state->config.dt + state->dt4 * state->config.accelVariance / 8.0f;
    state->P[5][5] += state->dt3 * state->config.accelVariance / 3.0f;
}

VOID __insKalmanFilterUpdateState(INSKalmanFilterState *state, INSKalmanFilterInputData *inputData)
{
    FLOAT v[5];

    v[0] = inputData->gpsPos.x - state->stateVector.pos.x;
    v[1] = inputData->gpsPos.y - state->stateVector.pos.y;
    v[2] = inputData->gpsPos.z - state->stateVector.pos.z;
    v[3] = inputData->pressure - (101325.0f * powf(1.0f - (0.0065f * state->stateVector.pos.z) / 288.15f, 5.25588f));
    v[4] = inputData->temperature - (288.15f - 0.0065f * state->stateVector.pos.z);

    state->stateVector.pos.x += state->K[0][0] * v[0] + state->K[0][1] * v[1] + state->K[0][2] * v[2] + state->K[0][3] * v[3] + state->K[0][4] * v[4];
    state->stateVector.pos.y += state->K[1][0] * v[0] + state->K[1][1] * v[1] + state->K[1][2] * v[2] + state->K[1][3] * v[3] + state->K[1][4] * v[4];
    state->stateVector.pos.z += state->K[2][0] * v[0] + state->K[2][1] * v[1] + state->K[2][2] * v[2] + state->K[2][3] * v[3] + state->K[2][4] * v[4];
    state->stateVector.vel.x += state->K[3][0] * v[0] + state->K[3][1] * v[1] + state->K[3][2] * v[2] + state->K[3][3] * v[3] + state->K[3][4] * v[4];
    state->stateVector.vel.y += state->K[4][0] * v[0] + state->K[4][1] * v[1] + state->K[4][2] * v[2] + state->K[4][3] * v[3] + state->K[4][4] * v[4];
    state->stateVector.vel.z += state->K[5][0] * v[0] + state->K[5][1] * v[1] + state->K[5][2] * v[2] + state->K[5][3] * v[3] + state->K[5][4] * v[4];

    state->H[0][0] = 1.0f;
    state->H[1][1] = 1.0f;
    state->H[2][2] = 1.0f;
    state->H[3][2] = 12.0131f * powf(1.0f - (0.0065f * state->stateVector.pos.z) / 288.15f, 4.25588f);
    state->H[4][2] = -0.0065f;
}

VOID __insKalmanFilterUpdateCovariance(INSKalmanFilterState *state)
{
    for (SIZE i = 0; i < 6; i++)
    {
        for (SIZE j = 0; j < 6; j++)
        {
            state->I_KH[i][j] = 0.0f;

            for (SIZE k = 0; k < 5; k++)
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

    for (SIZE i = 0; i < 6; i++)
    {
        for (SIZE j = 0; j < 6; j++)
        {
            state->I_KHP[i][j] = 0.0f;

            for (SIZE k = 0; k < 6; k++)
            {
                state->I_KHP[i][j] += state->I_KH[i][k] * state->P[k][j];
            }
        }
    }

    for (SIZE i = 0; i < 6; i++)
    {
        for (SIZE j = 0; j < 6; j++)
        {
            state->I_KHP_I_HK_[i][j] = 0.0f;

            for (SIZE k = 0; k < 6; k++)
            {
                state->I_KHP_I_HK_[i][j] += state->I_KHP[i][k] * state->I_KH[j][k];
            }
        }
    }

    for (SIZE i = 0; i < 6; i++)
    {
        for (SIZE j = 0; j < 5; j++)
        {
            state->KR[i][j] = 0.0f;

            for (SIZE k = 0; k < 5; k++)
            {
                state->KR[i][j] += state->K[i][k] * state->R[k][j];
            }
        }
    }

    for (SIZE i = 0; i < 6; i++)
    {
        for (SIZE j = 0; j < 6; j++)
        {
            FLOAT temp = 0.0f;

            for (SIZE k = 0; k < 5; k++)
            {
                temp += state->KR[i][k] * state->K[j][k];
            }

            state->P[i][j] = state->I_KHP_I_HK_[i][j] + temp;
        }
    }
}

VOID __insKalmanFilterUpdateGain(INSKalmanFilterState *state, INSKalmanFilterInputData *inputData)
{
    for (SIZE i = 0; i < 6; i++)
    {
        for (SIZE j = 0; j < 5; j++)
        {
            state->PH_[i][j] = 0.0f;

            for (SIZE k = 0; k < 6; k++)
            {
                state->PH_[i][j] += state->P[i][k] * state->H[j][k];
            }
        }
    }

    for (SIZE i = 0; i < 5; i++)
    {
        for (SIZE j = 0; j < 5; j++)
        {
            state->HPH_R[i][j] = 0.0f;
            state->HPH_R_INV[i][j] = 0.0f;

            for (SIZE k = 0; k < 6; k++)
            {
                state->HPH_R[i][j] += state->H[i][k] * state->PH_[k][j];
            }

            state->HPH_R[i][j] += state->R[i][j];
        }
    }

    for (SIZE i = 0; i < 5; i++)
    {
        if (state->HPH_R[i][i] == 0.0f)
        {
            SIZE big = i;

            for (SIZE j = 0; j < 5; j++)
            {
                if (fabsf(state->HPH_R[j][i]) > fabsf(state->HPH_R[big][i]))
                {
                    big = j;
                }
            }

            if (big != i)
            {
                for (SIZE j = 0; j < 5; j++)
                {
                    FLOAT temp = state->HPH_R[i][j];

                    state->HPH_R[i][j] = state->HPH_R[big][j];
                    state->HPH_R[big][j] = temp;

                    temp = state->HPH_R_INV[i][j];

                    state->HPH_R_INV[i][j] = state->HPH_R_INV[big][j];
                    state->HPH_R_INV[big][j] = temp;
                }
            }
        }
    }

    for (SIZE i = 0; i < 5; i++)
    {
        for (SIZE j = i + 1; j < 5; j++)
        {
            FLOAT k = state->HPH_R[j][i] / state->HPH_R[i][i];

            for (SIZE l = 0; l < 5; l++)
            {
                state->HPH_R[j][l] -= k * state->HPH_R[i][l];
                state->HPH_R_INV[j][l] -= k * state->HPH_R_INV[i][l];
            }

            state->HPH_R[j][i] = 0.0f;
        }
    }

    for (SIZE i = 0; i < 5; i++)
    {
        FLOAT d = state->HPH_R[i][i];

        for (SIZE j = 0; j < 5; j++)
        {
            state->HPH_R[i][j] /= d;
            state->HPH_R_INV[i][j] /= d;
        }

        state->HPH_R[i][i] = 1.0f;
    }

    for (SIZE i = 0; i < 5; i++)
    {
        for (SIZE j = i + 1; j < 5; j++)
        {
            FLOAT k = state->HPH_R[i][j];

            for (SIZE l = 0; l < 5; l++)
            {
                state->HPH_R[i][l] -= k * state->HPH_R[j][l];
                state->HPH_R_INV[i][l] -= k * state->HPH_R_INV[j][l];
            }

            state->HPH_R[i][j] = 0.0f;
        }
    }

    for (SIZE i = 0; i < 6; i++)
    {
        for (SIZE j = 0; j < 5; j++)
        {
            state->K[i][j] = 0.0f;

            for (SIZE k = 0; k < 5; k++)
            {
                state->K[i][j] += state->PH_[i][k] * state->HPH_R_INV[k][j];
            }
        }
    }
}