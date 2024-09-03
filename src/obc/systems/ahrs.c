#include "ahrs.h"
#include "sensors.h"
#include "../middleware/syslog.h"
#include "../middleware/events.h"
#include "lib/maths/math_constants.h"
#include <math.h>
#include <string.h>

#define SYSTEM_NAME "ahrs"
#define MADGWICK_BETA_CONSTANT 0.8660254f

/**
 * REF: https://ahrs.readthedocs.io/en/latest/filters/madgwick.html
 * REF: https://github.com/adafruit/Adafruit_AHRS/blob/master/src/Adafruit_AHRS_Madgwick.cpp
 * REF: https://github.com/arduino-libraries/MadgwickAHRS
 */
typedef struct madgwick_data
{
    quat_t q;
    float beta;
    float samplePeriod;
} madgwick_data_t;

/**
 * REF: https://github.com/PaulStoffregen/MahonyAHRS/blob/master/src/MahonyAHRS.cpp
 * REF: https://nitinjsanket.github.io/tutorials/attitudeest/mahony.html
 */
typedef struct mahony_data
{
    quat_t q;
    float Kp;
    float Ki;
    float integralFBx;
    float integralFBy;
    float integralFBz;
    float samplePeriod;
} mahony_data_t;

#define EKF_NUM_STATES 1
#define EKF_NUM_CONTROLS 0
#define EKF_NUM_MEASUREMENTS 2

typedef union ekf_state
{
    struct
    {
        float height;
    };

    float data[EKF_NUM_STATES];
} ekf_state_t;

typedef union ekf_measurements
{
    struct
    {
        float baroHeight;
        float gpsAlt;
    };

    float data[EKF_NUM_MEASUREMENTS];
} ekf_measurements_t;

typedef struct ekf_config
{
    float dt;
    float hVar;
    float gpsVar;
    float barVar;
} ekf_config_t;

typedef struct ekf_data
{
    ekf_config_t cfg;
    ekf_state_t x;
    float P[EKF_NUM_STATES][EKF_NUM_STATES];
} ekf_data_t;

static ahrs_data_t s_CurrentData;
static madgwick_data_t s_FilterData;
static ekf_data_t s_EKF;
static float s_GPSAltOffset;
static float s_BaroHeightOffset;

static void _madgwick_init(madgwick_data_t *data, float omegaError, float samplePeriod);
static void _madgwick_update_imu(madgwick_data_t *data, vec3_t gyroVec, vec3_t accVec);
static void _madgwick_update_marg(madgwick_data_t *data, vec3_t gyroVec, vec3_t accVec, vec3_t magVec);
static void _mahony_init(mahony_data_t *data, float kp, float ki, float samplePeriod);
static void _mahony_update_imu(mahony_data_t *data, vec3_t gyroVec, vec3_t accVec);
static void _mahony_update_marg(mahony_data_t *data, vec3_t gyroVec, vec3_t accVec, vec3_t magVec);
static void _mahony_update_base(mahony_data_t *data, vec3_t gyroVec, float ex, float ey, float ez);
static void _ekf_init(void);
static void _ekf_update(void);

void ahrs_init(void)
{
    _ekf_init();
    _madgwick_init(&s_FilterData, 2.2f, 0.0025f);

    SYS_LOG("READY");
}

void ahrs_update(void)
{
    if (events_poll(MSG_SENSORS_NORMAL_READ))
    {
        _ekf_update();
        _madgwick_update_imu(&s_FilterData, sensors_get_frame()->gyro1, sensors_get_frame()->acc1);

        s_CurrentData.orientation = s_FilterData.q;
        s_CurrentData.position.z = s_EKF.x.height;
    }
}

const ahrs_data_t *ahrs_get_data(void)
{
    return &s_CurrentData;
}

static void _madgwick_init(madgwick_data_t *data, float omegaError, float samplePeriod)
{
    data->q = (quat_t){1, 0, 0, 0};
    data->beta = MADGWICK_BETA_CONSTANT * DEG_2_RAD(omegaError);
    data->samplePeriod = samplePeriod;
}

static void _madgwick_update_imu(madgwick_data_t *data, vec3_t gyroVec, vec3_t accVec)
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

static void _madgwick_update_marg(madgwick_data_t *data, vec3_t gyroVec, vec3_t accVec, vec3_t magVec)
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

static void _mahony_init(mahony_data_t *data, float kp, float ki, float samplePeriod)
{
    data->q = (quat_t){1.0f, 0.0f, 0.0f, 0.0f};
    data->Kp = kp;
    data->Ki = ki;
    data->integralFBx = 0.0f;
    data->integralFBy = 0.0f;
    data->integralFBz = 0.0f;
    data->samplePeriod = samplePeriod;
}

static void _mahony_update_imu(mahony_data_t *data, vec3_t gyroVec, vec3_t accVec)
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

static void _mahony_update_marg(mahony_data_t *data, vec3_t gyroVec, vec3_t accVec, vec3_t magVec)
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

static void _mahony_update_base(mahony_data_t *data, vec3_t gyroVec, float ex, float ey, float ez)
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

static void _ekf_init(void)
{
    s_EKF.cfg = (ekf_config_t){
        .dt = 0.0025,
        .hVar = 1.0f,
        .gpsVar = 1.6f,
        .barVar = 0.9f,
    };

    memset(s_EKF.x.data, 0, sizeof(s_EKF.x.data));
    memset(s_EKF.P, 0, sizeof(s_EKF.P));

    for (size_t i = 0; i < EKF_NUM_STATES; i++)
    {
        s_EKF.P[i][i] = 10;
    }
}

static void _ekf_update(void)
{
    s_EKF.P[0][0] = s_EKF.P[0][0] + s_EKF.cfg.hVar;

    float H_Fusion[EKF_NUM_MEASUREMENTS][EKF_NUM_STATES];
    H_Fusion[0][0] = 1;
    H_Fusion[1][0] = 1;

    const float R_BARO = s_EKF.cfg.barVar;
    const float R_GPS = s_EKF.cfg.gpsVar;

    const float KS0 = s_EKF.P[0][0] * R_BARO;
    const float KS1 = s_EKF.P[0][0] * R_GPS;
    const float KS2 = 1.0F / (KS0 + KS1 + R_BARO * R_GPS);

    float K_Fusion[EKF_NUM_STATES][EKF_NUM_MEASUREMENTS];
    K_Fusion[0][0] = KS0 * KS2;
    K_Fusion[0][1] = KS1 * KS2;

    if (s_GPSAltOffset == 0)
    {
        s_GPSAltOffset = sensors_get_frame()->pos.alt;
    }

    if (s_BaroHeightOffset == 0)
    {
        s_BaroHeightOffset = sensors_get_frame()->baroHeight;
    }

    ekf_measurements_t meas = (ekf_measurements_t){
        .gpsAlt = sensors_get_frame()->pos.alt - s_GPSAltOffset,
        .baroHeight = sensors_get_frame()->baroHeight - s_BaroHeightOffset,
    };    

    float innov[EKF_NUM_MEASUREMENTS];
    innov[0] = meas.data[0] - s_EKF.x.data[0];
    innov[1] = meas.data[1] - s_EKF.x.data[0];

    float *states = s_EKF.x.data;

    for (size_t i = 0; i < EKF_NUM_STATES; i++)
    {
        for (size_t j = 0; j < EKF_NUM_MEASUREMENTS; j++)
        {
            states[i] += K_Fusion[i][j] * innov[j];
        }
    }

    float I_KH[EKF_NUM_STATES][EKF_NUM_STATES];

    for (size_t i = 0; i < EKF_NUM_STATES; i++)
    {
        for (size_t j = 0; j < EKF_NUM_STATES; j++)
        {
            float tmp = 0.0f;

            for (size_t k = 0; k < EKF_NUM_MEASUREMENTS; k++)
            {
                tmp += K_Fusion[i][k] * H_Fusion[k][j];
            }

            I_KH[i][j] = 1.0f - tmp;
        }
    }

    float P_Next[EKF_NUM_STATES][EKF_NUM_STATES];

    for (size_t i = 0; i < EKF_NUM_STATES; i++)
    {
        for (size_t j = 0; j < EKF_NUM_STATES; j++)
        {
            float tmp = 0.0f;

            for (size_t k = 0; k < EKF_NUM_STATES; k++)
            {
                tmp += I_KH[i][k] * s_EKF.P[k][j];
            }

            P_Next[i][j] = tmp;
        }
    }

    memcpy(s_EKF.P, P_Next, sizeof(P_Next));

    for (size_t i = 0; i < EKF_NUM_STATES; i++)
    {
        for (size_t j = 0; j < EKF_NUM_STATES; j++)
        {
            float tmp = (s_EKF.P[i][j] + s_EKF.P[j][i]) / 2.0f;
            s_EKF.P[i][j] = tmp;
            s_EKF.P[j][i] = tmp;
        }
    }
}