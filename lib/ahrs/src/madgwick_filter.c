#include "lib/ahrs/madgwick_filter.h"
#include "lib/maths/math_constants.h"
#include <math.h>

#define MADGWICK_BETA_CONSTANT 0.8660254f

void madgwick_init(madgiwck_filter_data_t *data, float omegaError, float samplePeriod)
{
    data->q = (quat_t){1, 0, 0, 0};
    data->beta = MADGWICK_BETA_CONSTANT * DEG_2_RAD(omegaError);
    data->samplePeriod = samplePeriod;
}

void madgwick_update_imu(madgiwck_filter_data_t *data, vec3_t gyroVec, vec3_t accVec)
{
    quat_t q = data->q;
    quat_t derivative = (quat_t){
        0.5f * (-gyroVec.x * q.x - gyroVec.y * q.y - gyroVec.z * q.z),
        0.5f * (gyroVec.x * q.w + gyroVec.z * q.y - gyroVec.y * q.z),
        0.5f * (gyroVec.y * q.w - gyroVec.z * q.x + gyroVec.x * q.z),
        0.5f * (gyroVec.z * q.w + gyroVec.y * q.x - gyroVec.x * q.y),
    };

    vec3_normalize(&accVec);

    float _2qw = 2.0f * q.w;
    float _2qx = 2.0f * q.x;
    float _2qy = 2.0f * q.y;
    float _2qz = 2.0f * q.z;

    vec3_t func_g = (vec3_t){
        _2qx * q.z - _2qw * q.y - accVec.x,
        _2qw * q.x + _2qy * q.z - accVec.y,
        1.0f - _2qx * q.x - _2qy * q.y - accVec.z,
    };

    quat_t grad = (quat_t){
        -_2qy * func_g.x + _2qx * func_g.y,
        _2qz * func_g.x + _2qw * func_g.y - 2.0f * _2qx * func_g.z,
        -_2qw * func_g.x + _2qz * func_g.y - 2.0f * _2qy * func_g.z,
        _2qx * func_g.x + _2qy * func_g.y,
    };

    quat_normalize(&grad);

    derivative.w -= data->beta * grad.w;
    derivative.x -= data->beta * grad.x;
    derivative.y -= data->beta * grad.y;
    derivative.z -= data->beta * grad.z;

    q.w += derivative.w * data->samplePeriod;
    q.x += derivative.x * data->samplePeriod;
    q.y += derivative.y * data->samplePeriod;
    q.z += derivative.z * data->samplePeriod;

    quat_normalize(&q);

    data->q = q;
}

void madgwick_update_marg(madgiwck_filter_data_t *data, vec3_t gyroVec, vec3_t accVec, vec3_t magVec)
{
    quat_t q = data->q;
    quat_t derivative = (quat_t){
        0.5f * (-gyroVec.x * q.x - gyroVec.y * q.y - gyroVec.z * q.z),
        0.5f * (gyroVec.x * q.w + gyroVec.z * q.y - gyroVec.y * q.z),
        0.5f * (gyroVec.y * q.w - gyroVec.z * q.x + gyroVec.x * q.z),
        0.5f * (gyroVec.z * q.w + gyroVec.y * q.x - gyroVec.x * q.y),
    };

    vec3_normalize(&accVec);
    vec3_normalize(&magVec);

    float _2qw = 2 * q.w;
    float _2qx = 2 * q.x;
    float _2qy = 2 * q.y;
    float _2qz = 2 * q.z;
    float qwqw = q.w * q.w;
    float qxqx = q.x * q.x;
    float qyqy = q.y * q.y;
    float qzqz = q.z * q.z;
    float qxqy = q.x * q.y;
    float qxqz = q.x * q.z;
    float qwqy = q.w * q.y;
    float qwqz = q.w * q.z;
    float qyqz = q.y * q.z;
    float qwqx = q.w * q.x;

    float hx = qxqx * magVec.x + 2 * qxqy * magVec.z + 2 * qxqz * magVec.z + qwqw * magVec.x + 2 * qwqy * magVec.z - 2 * qwqz * magVec.y - qzqz * magVec.x - qyqy * magVec.x;
    float hy = 2 * qxqy * magVec.x + qyqy * magVec.y + 2 * qyqz * magVec.z + 2 * qwqz * magVec.x - qzqz * magVec.y - qwqw * magVec.y - 2 * qwqx * magVec.z - qxqx * magVec.y;

    float _2bx = 2 * sqrtf(hx * hx + hy * hy);
    float _2bz = 2 * (2 * qxqz * magVec.x + 2 * qyqz * magVec.y + qzqz * magVec.z - 2 * qwqy * magVec.x - qyqy * magVec.z + 2 * qwqx * magVec.y - qxqx * magVec.z + qwqw * magVec.z);

    float _2bxqz = _2bx * q.z;
    float _2bxqy = _2bx * q.y;
    float _2bzqx = _2bz * q.x;
    float _2bzqy = _2bz * q.y;

    vec3_t func_g = (vec3_t){
        _2qx * q.z - _2qw * q.y - accVec.x,
        _2qw * q.x + _2qy * q.z - accVec.y,
        1.0f - _2qx * q.x - _2qy * q.y - accVec.z,
    };

    vec3_t func_b = (vec3_t){
        _2bx * (0.5f - qyqy - qzqz) + _2bz * (qxqz - qwqy) - magVec.x,
        _2bx * (qxqy - qwqz) + _2bz * (qwqx + qyqz) - magVec.y,
        _2bx * (qwqy + qxqz) + _2bz * (0.5f - qxqx - qyqy) - magVec.z,
    };

    quat_t grad = (quat_t){
        -_2qy * func_g.x + _2qx * func_g.y - _2bzqy * func_b.x + (-_2bxqz + _2bzqx) * func_b.y + _2bxqy * func_b.z,
        _2qz * func_g.x + _2qw * func_g.y - 2 * _2qx * func_g.z + _2bz * q.z * func_b.x + (_2bxqy + _2bz * q.w) * func_b.y + (_2bxqz - 2 * _2bzqx) * func_b.z,
        -_2qw * func_g.x + _2qz * func_g.y - 2 * _2qy * func_g.z + (-2 * _2bxqy - _2bz * q.w) * func_b.x + (_2bx * q.x + _2bz * q.z) * func_b.y + (_2bx * q.w - 2 * _2bzqy) * func_b.z,
        _2qx * func_g.x + _2qy * func_g.y + (-2 * _2bxqz + _2bzqx) * func_b.x + (-_2bx * q.w + _2bzqy) * func_b.y + _2bx * q.x * func_b.z,
    };

    quat_normalize(&grad);

    derivative.w -= data->beta * grad.w;
    derivative.x -= data->beta * grad.x;
    derivative.y -= data->beta * grad.y;
    derivative.z -= data->beta * grad.z;

    q.w += derivative.w * data->samplePeriod;
    q.x += derivative.x * data->samplePeriod;
    q.y += derivative.y * data->samplePeriod;
    q.z += derivative.z * data->samplePeriod;

    quat_normalize(&q);

    data->q = q;
}