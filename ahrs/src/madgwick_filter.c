#include "ahrs/madgwick_filter.h"
#include "maths/constants.h"

VOID madgwickInitIMU(MadgiwckFilterDataIMU *data, FLOAT omegaError, FLOAT samplePeriod)
{
    data->q = (quat){1, 0, 0, 0};
    data->beta = MADGWICK_BETA_CONSTANT * DEG_2_RAD(omegaError);
    data->samplePeriod = samplePeriod;
}

VOID madgwickInitMARG(MadgiwckFilterDataMARG *data, FLOAT omegaError, FLOAT samplePeriod)
{
    data->q = (quat){1, 0, 0, 0};
    data->beta = MADGWICK_BETA_CONSTANT * DEG_2_RAD(omegaError);
    data->samplePeriod = samplePeriod;
}

VOID madgwickUpdateIMU(MadgiwckFilterDataIMU *data, vec3 gyroVec, vec3 accVec)
{
    data->gyroQuat = (quat){0, gyroVec.x, gyroVec.y, gyroVec.z};
    data->gyroDerivative = (quat){0};

    quatMul(&data->gyroDerivative, &data->q, &data->gyroQuat);
    quatMulNum(0.5f, &data->gyroDerivative);
    vec3Normalize(&accVec);

    data->func_g = (vec3){
        .x = 2 * (data->q.x * data->q.z - data->q.w * data->q.y) - accVec.x,
        .y = 2 * (data->q.w * data->q.x + data->q.y * data->q.z) - accVec.y,
        .z = 2 * (0.5f - data->q.x * data->q.x - data->q.y * data->q.y) - accVec.z,
    };
    data->jt[0] = (vec3){-2 * data->q.y, 2 * data->q.x, 0};
    data->jt[1] = (vec3){2 * data->q.z, 2 * data->q.w, -4 * data->q.x};
    data->jt[2] = (vec3){-2 * data->q.w, 2 * data->q.z, -4 * data->q.y};
    data->jt[3] = (vec3){2 * data->q.x, 2 * data->q.y, 0};
    data->gradQuat = (quat){
        .w = vec3DotSum(&data->jt[0], &data->func_g),
        .x = vec3DotSum(&data->jt[1], &data->func_g),
        .y = vec3DotSum(&data->jt[2], &data->func_g),
        .z = vec3DotSum(&data->jt[3], &data->func_g),
    };

    quatNormalize(&data->gradQuat);
    quatMulNum(data->beta, &data->gradQuat);
    quatSub(&data->gyroDerivative, &data->gradQuat);
    quatMulNum(data->samplePeriod, &data->gyroDerivative);
    quatAdd(&data->q, &data->gyroDerivative);
}

VOID madgwickUpdateMARG(MadgiwckFilterDataMARG *data, vec3 gyroVec, vec3 accVec, vec3 magVec)
{
    // data->gyroQuat = (quat){0, gyroVec.x, gyroVec.y, gyroVec.z};
    // data->gyroDerivative = (quat){0};

    // quatMul(&data->gyroDerivative, &data->q, &data->gyroQuat);
    // quatMulNum(0.5f, &data->gyroDerivative);
    // vec3Normalize(&accVec);
    // vec3Normalize(&magVec);

    // data->magQuat = (quat){0, magVec.x, magVec.y, magVec.z};
    // data->qConj = data->q;
    // data->h = (quat){0};

    // quatConjugate(&data->qConj);
    // quatMul(&data->h, &data->q, &data->magQuat);
    // quatMul(&data->h, &data->h, &data->qConj);

    // data->b = (quat){
    //     .w = 0,
    //     .x = sqrt(data->h.x * data->h.x + data->h.y * data->h.y),
    //     .y = 0,
    //     .z = data->h.z,
    // };
    // data->func_g = (vec3){
    //     .x = 2 * (data->q.x * data->q.z - data->q.w * data->q.y) - accVec.x,
    //     .y = 2 * (data->q.w * data->q.x + data->q.y * data->q.z) - accVec.y,
    //     .z = 2 * (0.5f - data->q.x * data->q.x - data->q.y * data->q.y) - accVec.z,
    // };
    // data->func_b = (vec3){
    //     .x = 2 * data->b.x * (0.5f - data->q.y * data->q.y - data->q.z * data->q.z) + 2 * data->b.z * (data->q.x * data->q.z - data->q.w * data->q.y) - magVec.x,
    //     .y = 2 * data->b.x * (data->q.x * data->q.y - data->q.w * data->q.z) + 2 * data->b.z * (data->q.w * data->q.x + data->q.y * data->q.z) - magVec.y,
    //     .z = 2 * data->b.x * (data->q.w * data->q.y + data->q.x * data->q.z) + 2 * data->b.z * (0.5f - data->q.x * data->q.x - data->q.y * data->q.y) - magVec.z,
    // };

    // vec3 jt_g_0 = {-2 * data->q.y, 2 * data->q.x, 0};
    // vec3 jt_g_1 = {2 * data->q.z, 2 * data->q.w, -4 * data->q.x};
    // vec3 jt_g_2 = {-2 * data->q.w, 2 * data->q.z, -4 * data->q.y};
    // vec3 jt_g_3 = {2 * data->q.x, 2 * data->q.y, 0};
    // vec3 jt_b_0 = {-2 * data->b.z * data->q.y, -2 * data->b.x * data->q.z + 2 * data->b.x * data->q.x, 2 * data->b.x * data->q.y};
    // vec3 jt_b_1 = {2 * data->b.z * data->q.z, 2 * data->b.x * data->q.y + 2 * data->b.z * data->q.w, 2 * data->b.x * data->q.z - 4 * data->b.z * data->q.x};
    // vec3 jt_b_2 = {-4 * data->b.x * data->q.y - 2 * data->b.z * data->q.w, 2 * data->b.x * data->q.x + 2 * data->b.z * data->q.z, 2 * data->b.x * data->q.w - 4 * data->b.z * data->q.y};
    // vec3 jt_b_3 = {-4 * data->b.x * data->q.z + 2 * data->b.z * data->q.x, -2 * data->b.x * data->q.w + 2 * data->b.z * data->q.y, 2 * data->b.x * data->q.x};

    // quat gradQuat = {
    //     .w = vec3DotSum(jt_0, func_g),
    //     .x = vec3DotSum(jt_1, func_g),
    //     .y = vec3DotSum(jt_2, func_g),
    //     .z = vec3DotSum(jt_3, func_g),
    // };

    // quat gradUnitWithBeta = quatMulNum(data->beta, quatNormalize(gradQuat));
    // quat gyroMinusGrad = quatSub(gyroDerivative, gradUnitWithBeta);
    // quat gyroMinusGradWithSamplePeriod = quatMulNum(data->samplePeriod, gyroMinusGrad);
    // quat sum = quatAdd(data->q, gyroMinusGradWithSamplePeriod);

    // data->q = quatNormalize(sum);

    // vec3 normalizedAcc = vec3Normalize(accVec);
    // vec3 normalizedMag = vec3Normalize(magVec);

    // quat data->q = data->data->q;
    // quat gyro = {0, gyroVec.x, gyroVec.y, gyroVec.z};
    // quat acc = {0, normalizedAcc.x, normalizedAcc.y, normalizedAcc.z};
    // quat mag = {0, normalizedMag.x, 0, normalizedMag.z};
    // quat h = quatMul(data->q, quatMul(mag, quatConjugate(data->q)));
    // quat b = {
    //     .w = 0,
    //     .x = sqrt(h.x * h.x + h.y * h.y),
    //     .y = 0,
    //     .z = h.z,
    // };

    // vec3 func_g = {
    //     .x = 2 * (data->q.x * data->q.z - data->q.w * data->q.y) - acc.x,
    //     .y = 2 * (data->q.w * data->q.x + data->q.y * data->q.z) - acc.y,
    //     .z = 2 * (0.5f - data->q.x * data->q.x - data->q.y * data->q.y) - acc.z,
    // };

    // vec3 func_b = {
    //     .x = 2 * b.x * (0.5f - data->q.y * data->q.y - data->q.z * data->q.z) + 2 * b.z * (data->q.x * data->q.z - data->q.w * data->q.y) - mag.x,
    //     .y = 2 * b.x * (data->q.x * data->q.y - data->q.w * data->q.z) + 2 * b.z * (data->q.w * data->q.x + data->q.y * data->q.z) - mag.y,
    //     .z = 2 * b.x * (data->q.w * data->q.y + data->q.x * data->q.z) + 2 * b.z * (0.5f - data->q.x * data->q.x - data->q.y * data->q.y) - mag.z,
    // };

    // matMxN jacobian_g = matMxNCreate(3, 4);

    // jacobian_g.data[0][0] = -2 * data->q.y;
    // jacobian_g.data[0][1] = 2 * data->q.z;
    // jacobian_g.data[0][2] = -2 * data->q.w;
    // jacobian_g.data[0][3] = 2 * data->q.x;
    // jacobian_g.data[1][0] = 2 * data->q.x;
    // jacobian_g.data[1][1] = 2 * data->q.w;
    // jacobian_g.data[1][2] = 2 * data->q.z;
    // jacobian_g.data[1][3] = 2 * data->q.y;
    // jacobian_g.data[2][0] = 0;
    // jacobian_g.data[2][1] = -4 * data->q.x;
    // jacobian_g.data[2][2] = -4 * data->q.y;
    // jacobian_g.data[2][3] = 0;

    // matMxN jacobian_b = matMxNCreate(3, 4);

    // jacobian_b.data[0][0] = -2 * b.z * data->q.y;
    // jacobian_b.data[0][1] = 2 * b.z * data->q.z;
    // jacobian_b.data[0][2] = -4 * b.x * data->q.y - 2 * b.z * data->q.w;
    // jacobian_b.data[0][3] = -4 * b.x * data->q.z + 2 * b.z * data->q.x;
    // jacobian_b.data[1][0] = -2 * b.x * data->q.z + 2 * b.z * data->q.x;
    // jacobian_b.data[1][1] = 2 * b.x * data->q.y + 2 * b.z * data->q.w;
    // jacobian_b.data[1][2] = 2 * b.x * data->q.x + 2 * b.z * data->q.z;
    // jacobian_b.data[1][3] = -2 * b.x * data->q.w + 2 * b.z * data->q.y;
    // jacobian_b.data[2][0] = 2 * b.x * data->q.y;
    // jacobian_b.data[2][1] = 2 * b.x * data->q.z - 4 * b.z * data->q.x;
    // jacobian_b.data[2][2] = 2 * b.x * data->q.w - 4 * b.z * data->q.y;
    // jacobian_b.data[2][3] = 2 * b.x * data->q.x;

    // vecN func_g_b = vecNCreate(6);

    // func_g_b.data[0] = func_g.x;
    // func_g_b.data[1] = func_g.y;
    // func_g_b.data[2] = func_g.z;
    // func_g_b.data[3] = func_b.x;
    // func_g_b.data[4] = func_b.y;
    // func_g_b.data[5] = func_b.z;

    // matMxN func_g_b_mat = vecToMat(func_g_b);
    // matMxN jacobian_g_b = matMxNCreate(6, 4);

    // for (int i = 0; i < 3; i++)
    // {
    //     for (int j = 0; j < 4; j++)
    //     {
    //         jacobian_g_b.data[i][j] = jacobian_g.data[i][j];
    //         jacobian_g_b.data[i + 3][j] = jacobian_b.data[i + 3][j];
    //     }
    // }

    // matMxN jacobian_g_b_Transpose = matMxNTranspose(jacobian_g_b);
    // matMxN grad = matMxNMul(jacobian_g_b_Transpose, func_g_b_mat);
    // quat gradQuat = {
    //     .w = grad.data[0][0],
    //     .x = grad.data[1][0],
    //     .y = grad.data[2][0],
    //     .z = grad.data[3][0],
    // };
    // quat gradUnit = quatNormalize(gradQuat);
    // quat gyroMinusGrad = quatSub(gyro, quatMulNum(data->beta, gradUnit));

    // data->data->q = quatAdd(data->data->q, quatMulNum(data->samplePeriod, gyroMinusGrad));

    // matMxNFree(jacobian_g);
    // matMxNFree(jacobian_b);
    // vecNFree(func_g_b);
    // matMxNFree(func_g_b_mat);
    // matMxNFree(jacobian_g_b);
    // matMxNFree(jacobian_g_b_Transpose);
    // matMxNFree(grad);
}