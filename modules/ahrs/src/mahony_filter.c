#include "modules/ahrs/mahony_filter.h"
#include <math.h>

void mahony_init(mahony_filter_data_t *data, float kp, float ki, float samplePeriod)
{
    data->q = (quat_t){1.0f, 0.0f, 0.0f, 0.0f};
    data->Kp = kp;
    data->Ki = ki;
    data->integralFBx = 0.0f;
    data->integralFBy = 0.0f;
    data->integralFBz = 0.0f;
    data->samplePeriod = samplePeriod;
}

void mahony_update_imu(mahony_filter_data_t *data, vec3_t gyroVec, vec3_t accVec)
{
    vec3_normalize(&accVec);

    float vx = 2.0f * (data->q.x * data->q.z - data->q.w * data->q.y);
    float vy = 2.0f * (data->q.w * data->q.x + data->q.y * data->q.z);
    float vz = data->q.w * data->q.w - data->q.x * data->q.x - data->q.y * data->q.y + data->q.z * data->q.z;

    _mahony_update_base(
        data,
        gyroVec,
        accVec.y * vz - accVec.z * vy,
        accVec.z * vx - accVec.x * vz,
        accVec.x * vy - accVec.y * vx);
}

void mahony_update_marg(mahony_filter_data_t *data, vec3_t gyroVec, vec3_t accVec, vec3_t magVec)
{
    vec3_normalize(&accVec);
    vec3_normalize(&magVec);

    float qwqw = data->q.w * data->q.w;
    float qwqx = data->q.w * data->q.x;
    float qwqy = data->q.w * data->q.y;
    float qwqz = data->q.w * data->q.z;
    float qxqx = data->q.x * data->q.x;
    float qxqy = data->q.x * data->q.y;
    float qxqz = data->q.x * data->q.z;
    float qyqy = data->q.y * data->q.y;
    float qyqz = data->q.y * data->q.z;
    float qzqz = data->q.z * data->q.z;

    float hx = 2.0f * (magVec.x * (0.5f - qyqy - qzqz) + magVec.y * (qxqy - qwqz) + magVec.z * (qxqz + qwqy));
    float hy = 2.0f * (magVec.x * (qxqy + qwqz) + magVec.y * (0.5f - qxqx - qzqz) + magVec.z * (qyqz - qwqx));
    float bx = sqrtf(hx * hx + hy * hy);
    float bz = 2.0f * (magVec.x * (qxqz - qwqy) + magVec.y * (qyqz + qwqx) + magVec.z * (0.5f - qxqx - qyqy));

    float vx = 2.0f * (qxqz - qwqy);
    float vy = 2.0f * (qwqx + qyqz);
    float vz = qwqw - qxqx - qyqy + qzqz;
    float wx = 2.0f * (bx * (0.5f - qyqy - qzqz) + bz * (qxqz - qwqy));
    float wy = 2.0f * (bx * (qxqy - qwqz) + bz * (qwqx + qyqz));
    float wz = 2.0f * (bx * (qwqy + qxqz) + bz * (0.5f - qxqx - qyqy));

    _mahony_update_base(
        data,
        gyroVec,
        (accVec.y * vz - accVec.z * vy) + (magVec.y * wz - magVec.z * wy),
        (accVec.z * vx - accVec.x * vz) + (magVec.z * wx - magVec.x * wz),
        (accVec.x * vy - accVec.y * vx) + (magVec.x * wy - magVec.y * wx));
}

void _mahony_update_base(mahony_filter_data_t *data, vec3_t gyroVec, float ex, float ey, float ez)
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

    quat_normalize(&data->q);
}