#include "ahrs/madgwick_filter.h"
#include "maths/constants.h"
#include <math.h>

#define MADGWICK_BETA_CONSTANT 0.8660254f

static quat s_Q;
static quat s_GyroDerivative;
static vec3 s_FuncG;
static vec3 s_FuncB;
static quat s_Grad;

static FLOAT s_2qw;
static FLOAT s_2qx;
static FLOAT s_2qy;
static FLOAT s_2qz;
static FLOAT s_qwqw;
static FLOAT s_qxqx;
static FLOAT s_qyqy;
static FLOAT s_qzqz;
static FLOAT s_qxqy;
static FLOAT s_qxqz;
static FLOAT s_qwqy;
static FLOAT s_qwqz;
static FLOAT s_qyqz;
static FLOAT s_qwqx;
static FLOAT s_Hx;
static FLOAT s_Hy;
static FLOAT s_2bx;
static FLOAT s_2bz;
static FLOAT s_2bxqz;
static FLOAT s_2bxqy;
static FLOAT s_2bzqx;
static FLOAT s_2bzqy;

VOID madgwickInit(MadgiwckFilterData *data, FLOAT omegaError, FLOAT samplePeriod)
{
    data->q = (quat){1, 0, 0, 0};
    data->beta = MADGWICK_BETA_CONSTANT * DEG_2_RAD(omegaError);
    data->samplePeriod = samplePeriod;
}

VOID madgwickUpdateIMU(MadgiwckFilterData *data, vec3 gyroVec, vec3 accVec)
{
    s_Q = data->q;
    s_GyroDerivative = (quat){
        0.5f * (-gyroVec.x * s_Q.x - gyroVec.y * s_Q.y - gyroVec.z * s_Q.z),
        0.5f * (gyroVec.x * s_Q.w + gyroVec.z * s_Q.y - gyroVec.y * s_Q.z),
        0.5f * (gyroVec.y * s_Q.w - gyroVec.z * s_Q.x + gyroVec.x * s_Q.z),
        0.5f * (gyroVec.z * s_Q.w + gyroVec.y * s_Q.x - gyroVec.x * s_Q.y),
    };

    vec3Normalize(&accVec);

    s_2qw = 2.0f * s_Q.w;
    s_2qx = 2.0f * s_Q.x;
    s_2qy = 2.0f * s_Q.y;
    s_2qz = 2.0f * s_Q.z;

    s_FuncG = (vec3){
        s_2qx * s_Q.z - s_2qw * s_Q.y - accVec.x,
        s_2qw * s_Q.x + s_2qy * s_Q.z - accVec.y,
        1.0f - s_2qx * s_Q.x - s_2qy * s_Q.y - accVec.z,
    };

    s_Grad = (quat){
        -s_2qy * s_FuncG.x + s_2qx * s_FuncG.y,
        s_2qz * s_FuncG.x + s_2qw * s_FuncG.y - 2.0f * s_2qx * s_FuncG.z,
        -s_2qw * s_FuncG.x + s_2qz * s_FuncG.y - 2.0f * s_2qy * s_FuncG.z,
        s_2qx * s_FuncG.x + s_2qy * s_FuncG.y,
    };

    quatNormalize(&s_Grad);

    s_GyroDerivative.w -= data->beta * s_Grad.w;
    s_GyroDerivative.x -= data->beta * s_Grad.x;
    s_GyroDerivative.y -= data->beta * s_Grad.y;
    s_GyroDerivative.z -= data->beta * s_Grad.z;

    s_Q.w += s_GyroDerivative.w * data->samplePeriod;
    s_Q.x += s_GyroDerivative.x * data->samplePeriod;
    s_Q.y += s_GyroDerivative.y * data->samplePeriod;
    s_Q.z += s_GyroDerivative.z * data->samplePeriod;

    quatNormalize(&s_Q);

    data->q = s_Q;
}

VOID madgwickUpdateMARG(MadgiwckFilterData *data, vec3 gyroVec, vec3 accVec, vec3 magVec)
{
    s_Q = data->q;
    s_GyroDerivative = (quat){
        0.5f * (-gyroVec.x * s_Q.x - gyroVec.y * s_Q.y - gyroVec.z * s_Q.z),
        0.5f * (gyroVec.x * s_Q.w + gyroVec.z * s_Q.y - gyroVec.y * s_Q.z),
        0.5f * (gyroVec.y * s_Q.w - gyroVec.z * s_Q.x + gyroVec.x * s_Q.z),
        0.5f * (gyroVec.z * s_Q.w + gyroVec.y * s_Q.x - gyroVec.x * s_Q.y),
    };

    vec3Normalize(&accVec);
    vec3Normalize(&magVec);

    s_2qw = 2 * s_Q.w;
    s_2qx = 2 * s_Q.x;
    s_2qy = 2 * s_Q.y;
    s_2qz = 2 * s_Q.z;
    s_qwqw = s_Q.w * s_Q.w;
    s_qxqx = s_Q.x * s_Q.x;
    s_qyqy = s_Q.y * s_Q.y;
    s_qzqz = s_Q.z * s_Q.z;
    s_qxqy = s_Q.x * s_Q.y;
    s_qxqz = s_Q.x * s_Q.z;
    s_qwqy = s_Q.w * s_Q.y;
    s_qwqz = s_Q.w * s_Q.z;
    s_qyqz = s_Q.y * s_Q.z;
    s_qwqx = s_Q.w * s_Q.x;

    s_Hx = s_qxqx * magVec.x + 2 * s_qxqy * magVec.z + 2 * s_qxqz * magVec.z + s_qwqw * magVec.x + 2 * s_qwqy * magVec.z - 2 * s_qwqz * magVec.y - s_qzqz * magVec.x - s_qyqy * magVec.x;
    s_Hy = 2 * s_qxqy * magVec.x + s_qyqy * magVec.y + 2 * s_qyqz * magVec.z + 2 * s_qwqz * magVec.x - s_qzqz * magVec.y - s_qwqw * magVec.y - 2 * s_qwqx * magVec.z - s_qxqx * magVec.y;

    s_2bx = 2 * sqrt(s_Hx * s_Hx + s_Hy * s_Hy);
    s_2bz = 2 * (2 * s_qxqz * magVec.x + 2 * s_qyqz * magVec.y + s_qzqz * magVec.z - 2 * s_qwqy * magVec.x - s_qyqy * magVec.z + 2 * s_qwqx * magVec.y - s_qxqx * magVec.z + s_qwqw * magVec.z);

    s_2bxqz = s_2bx * s_Q.z;
    s_2bxqy = s_2bx * s_Q.y;
    s_2bzqx = s_2bz * s_Q.x;
    s_2bzqy = s_2bz * s_Q.y;

    s_FuncG = (vec3){
        s_2qx * s_Q.z - s_2qw * s_Q.y - accVec.x,
        s_2qw * s_Q.x + s_2qy * s_Q.z - accVec.y,
        1.0f - s_2qx * s_Q.x - s_2qy * s_Q.y - accVec.z,
    };

    s_FuncB = (vec3){
        s_2bx * (0.5f - s_qyqy - s_qzqz) + s_2bz * (s_qxqz - s_qwqy) - magVec.x,
        s_2bx * (s_qxqy - s_qwqz) + s_2bz * (s_qwqx + s_qyqz) - magVec.y,
        s_2bx * (s_qwqy + s_qxqz) + s_2bz * (0.5f - s_qxqx - s_qyqy) - magVec.z,
    };

    s_Grad = (quat){
        -s_2qy * s_FuncG.x + s_2qx * s_FuncG.y - s_2bzqy * s_FuncB.x + (-s_2bxqz + s_2bzqx) * s_FuncB.y + s_2bxqy * s_FuncB.z,
        s_2qz * s_FuncG.x + s_2qw * s_FuncG.y - 2 * s_2qx * s_FuncG.z + s_2bz * s_Q.z * s_FuncB.x + (s_2bxqy + s_2bz * s_Q.w) * s_FuncB.y + (s_2bxqz - 2 * s_2bzqx) * s_FuncB.z,
        -s_2qw * s_FuncG.x + s_2qz * s_FuncG.y - 2 * s_2qy * s_FuncG.z + (-2 * s_2bxqy - s_2bz * s_Q.w) * s_FuncB.x + (s_2bx * s_Q.x + s_2bz * s_Q.z) * s_FuncB.y + (s_2bx * s_Q.w - 2 * s_2bzqy) * s_FuncB.z,
        s_2qx * s_FuncG.x + s_2qy * s_FuncG.y + (-2 * s_2bxqz + s_2bzqx) * s_FuncB.x + (-s_2bx * s_Q.w + s_2bzqy) * s_FuncB.y + s_2bx * s_Q.x * s_FuncB.z,
    };

    quatNormalize(&s_Grad);

    s_GyroDerivative.w -= data->beta * s_Grad.w;
    s_GyroDerivative.x -= data->beta * s_Grad.x;
    s_GyroDerivative.y -= data->beta * s_Grad.y;
    s_GyroDerivative.z -= data->beta * s_Grad.z;

    s_Q.w += s_GyroDerivative.w * data->samplePeriod;
    s_Q.x += s_GyroDerivative.x * data->samplePeriod;
    s_Q.y += s_GyroDerivative.y * data->samplePeriod;
    s_Q.z += s_GyroDerivative.z * data->samplePeriod;

    quatNormalize(&s_Q);

    data->q = s_Q;
}