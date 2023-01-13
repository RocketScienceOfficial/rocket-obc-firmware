#include "ahrs/madgwick_filter.h"
#include "maths/constants.h"
#include "maths/matrix.h"
#include <math.h>

VOID madgwickInit(MadgiwckFilterData *data, FLOAT omegaError, FLOAT samplePeriod, BOOL useMagnetometer)
{
    data->q = (quat){0};
    data->beta = SQRT_3 / 2 * omegaError;
    data->samplePeriod = samplePeriod;
}

VOID madgwickUpdate(MadgiwckFilterData *data, vec3 gyroVec, vec3 accVec)
{
    vec3 normalizedAcc = vec3Normalize(accVec);

    quat q = data->q;
    quat gyro = {0, gyroVec.x, gyroVec.y, gyroVec.z};
    quat acc = {0, normalizedAcc.x, normalizedAcc.y, normalizedAcc.z};

    vec3 func_g = {
        .x = 2 * (q.x * q.z - q.w * q.y) - acc.x,
        .y = 2 * (q.w * q.x + q.y * q.z) - acc.y,
        .z = 2 * (0.5f - q.x * q.x - q.y * q.y) - acc.z,
    };

    vecN func_g_vec = vec3ToVecN(func_g);
    matMxN func_g_mat = vecToMat(func_g_vec);

    matMxN jacobian_g = matMxNCreate(3, 4);

    jacobian_g.data[0][0] = -2 * q.y;
    jacobian_g.data[0][1] = 2 * q.z;
    jacobian_g.data[0][2] = -2 * q.w;
    jacobian_g.data[0][3] = 2 * q.x;
    jacobian_g.data[1][0] = 2 * q.x;
    jacobian_g.data[1][1] = 2 * q.w;
    jacobian_g.data[1][2] = 2 * q.z;
    jacobian_g.data[1][3] = 2 * q.y;
    jacobian_g.data[2][0] = 0;
    jacobian_g.data[2][1] = -4 * q.x;
    jacobian_g.data[2][2] = -4 * q.y;
    jacobian_g.data[2][3] = 0;

    matMxN jacobian_g_Transpose = matMxNTranspose(jacobian_g);
    matMxN grad = matMxNMul(jacobian_g_Transpose, func_g_mat);
    quat gradQuat = {
        .w = grad.data[0][0],
        .x = grad.data[1][0],
        .y = grad.data[2][0],
        .z = grad.data[3][0],
    };
    quat gradUnit = quatNormalize(gradQuat);
    quat gyroMinusGrad = quatSub(gyro, quatMulNum(data->beta, gradUnit));

    data->q = quatAdd(data->q, quatMulNum(data->samplePeriod, gyroMinusGrad));

    vecNFree(func_g_vec);
    matMxNFree(func_g_mat);
    matMxNFree(jacobian_g);
    matMxNFree(jacobian_g_Transpose);
    matMxNFree(grad);
}

VOID madgwickUpdateMag(MadgiwckFilterData *data, vec3 gyroVec, vec3 accVec, vec3 magVec)
{
    vec3 normalizedAcc = vec3Normalize(accVec);
    vec3 normalizedMag = vec3Normalize(magVec);

    quat q = data->q;
    quat gyro = {0, gyroVec.x, gyroVec.y, gyroVec.z};
    quat acc = {0, normalizedAcc.x, normalizedAcc.y, normalizedAcc.z};
    quat mag = {0, normalizedMag.x, 0, normalizedMag.z};
    quat h = quatMul(q, quatMul(mag, quatConjugate(q)));
    quat b = {
        .w = 0,
        .x = sqrt(h.x * h.x + h.y * h.y),
        .y = 0,
        .z = h.z,
    };

    vec3 func_g = {
        .x = 2 * (q.x * q.z - q.w * q.y) - acc.x,
        .y = 2 * (q.w * q.x + q.y * q.z) - acc.y,
        .z = 2 * (0.5f - q.x * q.x - q.y * q.y) - acc.z,
    };

    vec3 func_b = {
        .x = 2 * b.x * (0.5f - q.y * q.y - q.z * q.z) + 2 * b.z * (q.x * q.z - q.w * q.y) - mag.x,
        .y = 2 * b.x * (q.x * q.y - q.w * q.z) + 2 * b.z * (q.w * q.x + q.y * q.z) - mag.y,
        .z = 2 * b.x * (q.w * q.y + q.x * q.z) + 2 * b.z * (0.5f - q.x * q.x - q.y * q.y) - mag.z,
    };

    matMxN jacobian_g = matMxNCreate(3, 4);

    jacobian_g.data[0][0] = -2 * q.y;
    jacobian_g.data[0][1] = 2 * q.z;
    jacobian_g.data[0][2] = -2 * q.w;
    jacobian_g.data[0][3] = 2 * q.x;
    jacobian_g.data[1][0] = 2 * q.x;
    jacobian_g.data[1][1] = 2 * q.w;
    jacobian_g.data[1][2] = 2 * q.z;
    jacobian_g.data[1][3] = 2 * q.y;
    jacobian_g.data[2][0] = 0;
    jacobian_g.data[2][1] = -4 * q.x;
    jacobian_g.data[2][2] = -4 * q.y;
    jacobian_g.data[2][3] = 0;

    matMxN jacobian_b = matMxNCreate(3, 4);

    jacobian_b.data[0][0] = -2 * b.z * q.y;
    jacobian_b.data[0][1] = 2 * b.z * q.z;
    jacobian_b.data[0][2] = -4 * b.x * q.y - 2 * b.z * q.w;
    jacobian_b.data[0][3] = -4 * b.x * q.z + 2 * b.z * q.x;
    jacobian_b.data[1][0] = -2 * b.x * q.z + 2 * b.z * q.x;
    jacobian_b.data[1][1] = 2 * b.x * q.y + 2 * b.z * q.w;
    jacobian_b.data[1][2] = 2 * b.x * q.x + 2 * b.z * q.z;
    jacobian_b.data[1][3] = -2 * b.x * q.w + 2 * b.z * q.y;
    jacobian_b.data[2][0] = 2 * b.x * q.y;
    jacobian_b.data[2][1] = 2 * b.x * q.z - 4 * b.z * q.x;
    jacobian_b.data[2][2] = 2 * b.x * q.w - 4 * b.z * q.y;
    jacobian_b.data[2][3] = 2 * b.x * q.x;

    vecN func_g_b = vecNCreate(6);

    func_g_b.data[0] = func_g.x;
    func_g_b.data[1] = func_g.y;
    func_g_b.data[2] = func_g.z;
    func_g_b.data[3] = func_b.x;
    func_g_b.data[4] = func_b.y;
    func_g_b.data[5] = func_b.z;

    matMxN func_g_b_mat = vecToMat(func_g_b);
    matMxN jacobian_g_b = matMxNCreate(6, 4);

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            jacobian_g_b.data[i][j] = jacobian_g.data[i][j];
            jacobian_g_b.data[i + 3][j] = jacobian_b.data[i + 3][j];
        }
    }

    matMxN jacobian_g_b_Transpose = matMxNTranspose(jacobian_g_b);
    matMxN grad = matMxNMul(jacobian_g_b_Transpose, func_g_b_mat);
    quat gradQuat = {
        .w = grad.data[0][0],
        .x = grad.data[1][0],
        .y = grad.data[2][0],
        .z = grad.data[3][0],
    };
    quat gradUnit = quatNormalize(gradQuat);
    quat gyroMinusGrad = quatSub(gyro, quatMulNum(data->beta, gradUnit));

    data->q = quatAdd(data->q, quatMulNum(data->samplePeriod, gyroMinusGrad));

    matMxNFree(jacobian_g);
    matMxNFree(jacobian_b);
    vecNFree(func_g_b);
    matMxNFree(func_g_b_mat);
    matMxNFree(jacobian_g_b);
    matMxNFree(jacobian_g_b_Transpose);
    matMxNFree(grad);
}