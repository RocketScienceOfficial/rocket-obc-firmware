#include "ahrs/mahony_filter.h"
#include <math.h>

static FLOAT s_qwqw;
static FLOAT s_qwqx;
static FLOAT s_qwqy;
static FLOAT s_qwqz;
static FLOAT s_qxqx;
static FLOAT s_qxqy;
static FLOAT s_qxqz;
static FLOAT s_qyqy;
static FLOAT s_qyqz;
static FLOAT s_qzqz;

static FLOAT s_vx;
static FLOAT s_vy;
static FLOAT s_vz;
static FLOAT s_wx;
static FLOAT s_wy;
static FLOAT s_wz;
static FLOAT s_hx;
static FLOAT s_hy;
static FLOAT s_bx;
static FLOAT s_bz;

VOID mahonyInit(MahonyFilterData *data, FLOAT kp, FLOAT ki, FLOAT samplePeriod)
{
    data->q = (quat){1.0f, 0.0f, 0.0f, 0.0f};
    data->Kp = kp;
    data->Ki = ki;
    data->integralFBx = 0.0f;
    data->integralFBy = 0.0f;
    data->integralFBz = 0.0f;
    data->samplePeriod = samplePeriod;
}

VOID mahonyUpdateIMU(MahonyFilterData *data, vec3 gyroVec, vec3 accVec)
{
    vec3Normalize(&accVec);

    s_vx = 2.0f * (data->q.x * data->q.z - data->q.w * data->q.y);
    s_vy = 2.0f * (data->q.w * data->q.x + data->q.y * data->q.z);
    s_vz = data->q.w * data->q.w - data->q.x * data->q.x - data->q.y * data->q.y + data->q.z * data->q.z;

    __mahonyUpdateBase(
        data,
        gyroVec,
        accVec.y * s_vz - accVec.z * s_vy,
        accVec.z * s_vx - accVec.x * s_vz,
        accVec.x * s_vy - accVec.y * s_vx);
}

VOID mahonyUpdateMARG(MahonyFilterData *data, vec3 gyroVec, vec3 accVec, vec3 magVec)
{
    vec3Normalize(&accVec);
    vec3Normalize(&magVec);

    s_qwqw = data->q.w * data->q.w;
    s_qwqx = data->q.w * data->q.x;
    s_qwqy = data->q.w * data->q.y;
    s_qwqz = data->q.w * data->q.z;
    s_qxqx = data->q.x * data->q.x;
    s_qxqy = data->q.x * data->q.y;
    s_qxqz = data->q.x * data->q.z;
    s_qyqy = data->q.y * data->q.y;
    s_qyqz = data->q.y * data->q.z;
    s_qzqz = data->q.z * data->q.z;

    s_hx = 2.0f * (magVec.x * (0.5f - s_qyqy - s_qzqz) + magVec.y * (s_qxqy - s_qwqz) + magVec.z * (s_qxqz + s_qwqy));
    s_hy = 2.0f * (magVec.x * (s_qxqy + s_qwqz) + magVec.y * (0.5f - s_qxqx - s_qzqz) + magVec.z * (s_qyqz - s_qwqx));
    s_bx = sqrtf(s_hx * s_hx + s_hy * s_hy);
    s_bz = 2.0f * (magVec.x * (s_qxqz - s_qwqy) + magVec.y * (s_qyqz + s_qwqx) + magVec.z * (0.5f - s_qxqx - s_qyqy));

    s_vx = 2.0f * (s_qxqz - s_qwqy);
    s_vy = 2.0f * (s_qwqx + s_qyqz);
    s_vz = s_qwqw - s_qxqx - s_qyqy + s_qzqz;
    s_wx = 2.0f * (s_bx * (0.5f - s_qyqy - s_qzqz) + s_bz * (s_qxqz - s_qwqy));
    s_wy = 2.0f * (s_bx * (s_qxqy - s_qwqz) + s_bz * (s_qwqx + s_qyqz));
    s_wz = 2.0f * (s_bx * (s_qwqy + s_qxqz) + s_bz * (0.5f - s_qxqx - s_qyqy));

    __mahonyUpdateBase(
        data,
        gyroVec,
        (accVec.y * s_vz - accVec.z * s_vy) + (magVec.y * s_wz - magVec.z * s_wy),
        (accVec.z * s_vx - accVec.x * s_vz) + (magVec.z * s_wx - magVec.x * s_wz),
        (accVec.x * s_vy - accVec.y * s_vx) + (magVec.x * s_wy - magVec.y * s_wx));
}

VOID __mahonyUpdateBase(MahonyFilterData *data, vec3 gyroVec, FLOAT ex, FLOAT ey, FLOAT ez)
{
    if (data->Ki > 0.0f)
    {
        data->integralFBx += data->Ki * ex * data->samplePeriod;
        data->integralFBy += data->Ki * ey * data->samplePeriod;
        data->integralFBz += data->Ki * ez * data->samplePeriod;

        gyroVec.x += data->integralFBx;
        gyroVec.y += data->integralFBy;
        gyroVec.z += data->integralFBz;
    }

    gyroVec.x += data->Kp * ex;
    gyroVec.y += data->Kp * ey;
    gyroVec.z += data->Kp * ez;

    gyroVec.x *= (0.5f * data->samplePeriod);
    gyroVec.y *= (0.5f * data->samplePeriod);
    gyroVec.z *= (0.5f * data->samplePeriod);

    data->q.w += (-data->q.x * gyroVec.x - data->q.y * gyroVec.y - data->q.z * gyroVec.z);
    data->q.x += (data->q.w * gyroVec.x + data->q.y * gyroVec.z - data->q.z * gyroVec.y);
    data->q.y += (data->q.w * gyroVec.y - data->q.x * gyroVec.z + data->q.z * gyroVec.x);
    data->q.z += (data->q.w * gyroVec.z + data->q.x * gyroVec.y - data->q.y * gyroVec.x);

    quatNormalize(&data->q);
}