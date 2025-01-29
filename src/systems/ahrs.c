#include "ahrs.h"
#include "sensors.h"
#include "serial.h"
#include "../middleware/events.h"
#include <hal/time_tracker.h>
#include <lib/maths/math_utils.h>
#include <lib/maths/math_constants.h>
#include <lib/geo/geo.h>
#include <lib/geo/physical_constants.h>
#include <math.h>
#include <string.h>

#define STREAM_DELTA_TIME_MS 10
#define ORIENTATION_SWITCH_THRESHOLD (2.0f * EARTH_GRAVITY)
#define EKF_ACC_VARIANCE 0.1f
#define EKF_GPS_VARIANCE 0.5f
#define EKF_BARO_VARIANCE 0.2f
#define EKF_OUTDATED_MEASUREMENT_VARIANCE_MULTIPLIER 5 // 5 // 1000
#define EKF_ACC_CHANGING_MULTIPLIER 1                   // 10 // 1000
#define EKF_ACC_CHANGE_THRESHOLD (0.5f * EARTH_GRAVITY) // 0.2

/**
 * REF: https://ahrs.readthedocs.io/en/latest/filters/madgwick.html
 * REF: https://github.com/adafruit/Adafruit_AHRS/blob/master/src/Adafruit_AHRS_Madgwick.cpp
 * REF: https://github.com/arduino-libraries/MadgwickAHRS
 */
typedef struct madgwick_data
{
    float beta;
    float dt;
} madgwick_data_t;

/**
 * REF: https://github.com/PaulStoffregen/MahonyAHRS/blob/master/src/MahonyAHRS.cpp
 * REF: https://nitinjsanket.github.io/tutorials/attitudeest/mahony.html
 */
typedef struct mahony_data
{
    float Kp;
    float Ki;
    float integralFBx;
    float integralFBy;
    float integralFBz;
    float dt;
} mahony_data_t;

#define EKF_NUM_STATES 6
#define EKF_NUM_CONTROLS 3
#define EKF_NUM_MEASUREMENTS 4

typedef union ekf_state
{
    struct
    {
        float pos_n;
        float pos_e;
        float pos_d;
        float vel_n;
        float vel_e;
        float vel_d;
    };

    float data[EKF_NUM_STATES];
} ekf_state_t;

typedef union ekf_controls
{
    struct
    {
        float acc_n;
        float acc_e;
        float acc_d;
    };

    float data[EKF_NUM_CONTROLS];
} ekf_controls_t;

typedef union ekf_measurements
{
    struct
    {
        float gps_n;
        float gps_e;
        float gps_d;
        float baroHeight;
    };

    float data[EKF_NUM_MEASUREMENTS];
} ekf_measurements_t;

typedef struct ekf_config
{
    float dt;
    float varAcc;
    float varGPS;
    float varBar;
} ekf_config_t;

typedef struct ekf_data
{
    ekf_config_t cfg;
    ekf_state_t x;
    float P[EKF_NUM_STATES][EKF_NUM_STATES];
    float P_Next[EKF_NUM_STATES][EKF_NUM_STATES];
} ekf_data_t;

static ahrs_data_t s_CurrentData;
static bool s_Calibrated;
static madgwick_data_t s_Madgwick;
static mahony_data_t s_Mahony;
static ekf_data_t s_EKF;
static geo_position_wgs84_t s_BaseGPSPos;
static vec3_prec_t s_NEDPos;
static float s_BaroHeightOffset;
static float s_BaroHeight;
static vec3_t s_LastAcc;
static bool s_IsStreamingEKFData;
static hal_msec_t s_StreamTimer;

static void _madgwick_init(madgwick_data_t *data, float omegaError, float dt);
static void _madgwick_update_imu(madgwick_data_t *data, quat_t *q, vec3_t gyroVec, vec3_t accVec);
static void _madgwick_update_marg(madgwick_data_t *data, quat_t *q, vec3_t gyroVec, vec3_t accVec, vec3_t magVec);
static void _mahony_init(mahony_data_t *data, float kp, float ki, float dt);
static void _mahony_update_imu(mahony_data_t *data, quat_t *q, vec3_t gyroVec, vec3_t accVec);
static void _mahony_update_marg(mahony_data_t *data, quat_t *q, vec3_t gyroVec, vec3_t accVec, vec3_t magVec);
static void _mahony_update_base(mahony_data_t *data, quat_t *q, vec3_t gyroVec, float ex, float ey, float ez);
static void _integrate_gyro(quat_t *q, vec3_t gyro, float dt);
static void _ekf_init(ekf_data_t *data);
static void _ekf_predict_state(ekf_data_t *data, const ekf_controls_t *controls);
static void _ekf_predict_covariance(ekf_data_t *data, const ekf_controls_t *controls);
static void _ekf_fusion(ekf_data_t *data, const ekf_controls_t *controls, const ekf_measurements_t *measurements);
static void _ekf_force_cov_symmetry(ekf_data_t *data);
static void _ekf_update(ekf_data_t *data, const ekf_controls_t *controls, const ekf_measurements_t *measurements);

void ahrs_init(void)
{
    s_CurrentData.orientation = (quat_t){1.0f, 0.0f, 0.0f, 0.0f};

    _madgwick_init(&s_Madgwick, 6.0f, 0.0025f);
    _mahony_init(&s_Mahony, 6.0f, 0.0f, 0.0025f);
    _ekf_init(&s_EKF);

    SERIAL_DEBUG_LOG("READY");
}

void ahrs_update(void)
{
    if (events_poll(MSG_SENSORS_GPS_READ))
    {
        if (s_BaseGPSPos.lat == 0 && sensors_get_frame()->pos.lat != 0)
        {
            s_BaseGPSPos = sensors_get_frame()->pos;
        }
        else
        {
            s_NEDPos = geo_to_ned(sensors_get_frame()->pos, s_BaseGPSPos);
        }

        s_EKF.cfg.varGPS = EKF_GPS_VARIANCE;
    }

    if (events_poll(MSG_SENSORS_BARO_READ))
    {
        if (s_BaroHeightOffset == 0 && sensors_get_frame()->baroHeight != 0)
        {
            s_BaroHeightOffset = sensors_get_frame()->baroHeight;
        }
        else
        {
            s_BaroHeight = sensors_get_frame()->baroHeight - s_BaroHeightOffset;
        }

        s_EKF.cfg.varBar = EKF_BARO_VARIANCE;
    }

    if (events_poll(MSG_SENSORS_NORMAL_READ))
    {
        if (fabsf(sensors_get_frame()->acc1.x - s_LastAcc.x) >= EKF_ACC_CHANGE_THRESHOLD || fabsf(sensors_get_frame()->acc1.y - s_LastAcc.y) >= EKF_ACC_CHANGE_THRESHOLD || fabsf(sensors_get_frame()->acc1.z - s_LastAcc.z) >= EKF_ACC_CHANGE_THRESHOLD)
        {
            s_EKF.cfg.varAcc = EKF_ACC_VARIANCE * EKF_ACC_CHANGING_MULTIPLIER;
        }
        else
        {
            s_EKF.cfg.varAcc = EKF_ACC_VARIANCE;
        }
        s_LastAcc = sensors_get_frame()->acc1;

        s_EKF.cfg.dt = sensors_get_frame()->measurementDt;
        s_Mahony.dt = sensors_get_frame()->measurementDt;
        s_Madgwick.dt = sensors_get_frame()->measurementDt;

        if (vec3_mag_compare(&sensors_get_frame()->acc1, ORIENTATION_SWITCH_THRESHOLD) < 0)
        {
            _mahony_update_marg(&s_Mahony, &s_CurrentData.orientation, sensors_get_frame()->gyro1, sensors_get_frame()->acc1, sensors_get_frame()->mag1);
            // _madgwick_update_marg(&s_Madgwick, &s_CurrentData.orientation, sensors_get_frame()->gyro1, sensors_get_frame()->acc1, sensors_get_frame()->mag1);
        }
        else
        {
            _integrate_gyro(&s_CurrentData.orientation, sensors_get_frame()->gyro1, sensors_get_frame()->measurementDt);
        }

        vec3_t newAcc = sensors_get_frame()->acc1;
        quat_rotate_vec(&newAcc, &s_CurrentData.orientation);
        newAcc.x *= -1;
        newAcc.y *= -1;
        newAcc.z -= EARTH_GRAVITY;

        s_CurrentData.acceleration = newAcc;

        if (s_Calibrated)
        {
            ekf_controls_t controls = {
                .acc_n = newAcc.x,
                .acc_e = newAcc.y,
                .acc_d = newAcc.z,
            };

            ekf_measurements_t measurements = {
                .gps_n = s_NEDPos.x,
                .gps_e = s_NEDPos.y,
                .gps_d = s_NEDPos.z,
                .baroHeight = s_BaroHeight,
            };

            _ekf_update(&s_EKF, &controls, &measurements);

            s_CurrentData.position.x = s_EKF.x.pos_n;
            s_CurrentData.position.y = s_EKF.x.pos_e;
            s_CurrentData.position.z = s_EKF.x.pos_d;
            s_CurrentData.velocity.x = s_EKF.x.vel_n;
            s_CurrentData.velocity.y = s_EKF.x.vel_e;
            s_CurrentData.velocity.z = s_EKF.x.vel_d;

            s_EKF.cfg.varGPS = EKF_GPS_VARIANCE * EKF_OUTDATED_MEASUREMENT_VARIANCE_MULTIPLIER;
            s_EKF.cfg.varBar = EKF_BARO_VARIANCE * EKF_OUTDATED_MEASUREMENT_VARIANCE_MULTIPLIER;
        }
        else
        {
            if (value_approx_eql(newAcc.x, 0.0f, 0.2f) && value_approx_eql(newAcc.y, 0.0f, 0.2f) && value_approx_eql(newAcc.z, 0.0f, 0.2f))
            {
                SERIAL_DEBUG_LOG("Orientation calibrated!");

                s_Calibrated = true;
            }
        }
    }

    if (events_poll(MSG_SERIAL_MESSAGE_RECEIVED))
    {
        const datalink_frame_structure_serial_t *msg = serial_get_current_message();

        if (msg)
        {
            if (msg->msgId == DATALINK_MESSAGE_OBC_APP_EKF_TRANSMIT_START)
            {
                s_IsStreamingEKFData = true;
            }
            else if (msg->msgId == DATALINK_MESSAGE_OBC_APP_EKF_TRANSMIT_END)
            {
                s_IsStreamingEKFData = false;
            }
        }
    }

    if (s_IsStreamingEKFData)
    {
        if (hal_time_get_ms_since_boot() - s_StreamTimer >= STREAM_DELTA_TIME_MS)
        {
            datalink_frame_obc_app_ekf_data_t payload = {
                .qw = s_CurrentData.orientation.w,
                .qx = s_CurrentData.orientation.x,
                .qy = s_CurrentData.orientation.y,
                .qz = s_CurrentData.orientation.z,
                .positionN = s_CurrentData.position.x,
                .positionE = s_CurrentData.position.y,
                .positionD = s_CurrentData.position.z,
                .velocityN = s_CurrentData.velocity.x,
                .velocityE = s_CurrentData.velocity.y,
                .velocityD = s_CurrentData.velocity.z,
                .accelerationN = s_CurrentData.acceleration.x,
                .accelerationE = s_CurrentData.acceleration.y,
                .accelerationD = s_CurrentData.acceleration.z,
            };
            datalink_frame_structure_serial_t frame = {
                .msgId = DATALINK_MESSAGE_OBC_APP_EKF_DATA,
                .len = sizeof(payload),
            };
            memcpy(frame.payload, &payload, sizeof(payload));
            serial_send_message(&frame);

            s_StreamTimer = hal_time_get_ms_since_boot();
        }
    }
}

const ahrs_data_t *ahrs_get_data(void)
{
    return &s_CurrentData;
}

static void _madgwick_init(madgwick_data_t *data, float omegaError, float dt)
{
    data->beta = sqrtf(3.0f) / 2.0f * DEG_2_RAD(omegaError);
    data->dt = dt;
}

static void _madgwick_update_imu(madgwick_data_t *data, quat_t *q, vec3_t gyroVec, vec3_t accVec)
{
    vec3_normalize(&accVec);

    const float _2qw = 2.0f * q->w;
    const float _2qx = 2.0f * q->x;
    const float _2qy = 2.0f * q->y;
    const float _2qz = 2.0f * q->z;

    const vec3_t func_g = (vec3_t){
        _2qx * q->z - _2qw * q->y - accVec.x,
        _2qw * q->x + _2qy * q->z - accVec.y,
        1.0f - _2qx * q->x - _2qy * q->y - accVec.z,
    };

    quat_t grad = (quat_t){
        -_2qy * func_g.x + _2qx * func_g.y,
        +_2qz * func_g.x + _2qw * func_g.y - 2.0f * _2qx * func_g.z,
        -_2qw * func_g.x + _2qz * func_g.y - 2.0f * _2qy * func_g.z,
        +_2qx * func_g.x + _2qy * func_g.y,
    };
    quat_normalize(&grad);

    quat_t derivative = quat_gyro_derivative(q, &gyroVec);
    derivative.w -= data->beta * grad.w;
    derivative.x -= data->beta * grad.x;
    derivative.y -= data->beta * grad.y;
    derivative.z -= data->beta * grad.z;

    q->w += derivative.w * data->dt;
    q->x += derivative.x * data->dt;
    q->y += derivative.y * data->dt;
    q->z += derivative.z * data->dt;

    quat_normalize(q);
}

static void _madgwick_update_marg(madgwick_data_t *data, quat_t *q, vec3_t gyroVec, vec3_t accVec, vec3_t magVec)
{
    vec3_normalize(&accVec);
    vec3_normalize(&magVec);

    const float _2qw = 2 * q->w;
    const float _2qx = 2 * q->x;
    const float _2qy = 2 * q->y;
    const float _2qz = 2 * q->z;
    const float qwqw = q->w * q->w;
    const float qxqx = q->x * q->x;
    const float qyqy = q->y * q->y;
    const float qzqz = q->z * q->z;
    const float qxqy = q->x * q->y;
    const float qxqz = q->x * q->z;
    const float qwqy = q->w * q->y;
    const float qwqz = q->w * q->z;
    const float qyqz = q->y * q->z;
    const float qwqx = q->w * q->x;
    const float hx = qxqx * magVec.x + 2 * qxqy * magVec.z + 2 * qxqz * magVec.z + qwqw * magVec.x + 2 * qwqy * magVec.z - 2 * qwqz * magVec.y - qzqz * magVec.x - qyqy * magVec.x;
    const float hy = 2 * qxqy * magVec.x + qyqy * magVec.y + 2 * qyqz * magVec.z + 2 * qwqz * magVec.x - qzqz * magVec.y - qwqw * magVec.y - 2 * qwqx * magVec.z - qxqx * magVec.y;
    const float _2bx = 2 * sqrtf(hx * hx + hy * hy);
    const float _2bz = 2 * (2 * qxqz * magVec.x + 2 * qyqz * magVec.y + qzqz * magVec.z - 2 * qwqy * magVec.x - qyqy * magVec.z + 2 * qwqx * magVec.y - qxqx * magVec.z + qwqw * magVec.z);
    const float _2bxqz = _2bx * q->z;
    const float _2bxqy = _2bx * q->y;
    const float _2bzqx = _2bz * q->x;
    const float _2bzqy = _2bz * q->y;

    const vec3_t func_g = (vec3_t){
        _2qx * q->z - _2qw * q->y - accVec.x,
        _2qw * q->x + _2qy * q->z - accVec.y,
        1.0f - _2qx * q->x - _2qy * q->y - accVec.z,
    };
    const vec3_t func_b = (vec3_t){
        _2bx * (0.5f - qyqy - qzqz) + _2bz * (qxqz - qwqy) - magVec.x,
        _2bx * (qxqy - qwqz) + _2bz * (qwqx + qyqz) - magVec.y,
        _2bx * (qwqy + qxqz) + _2bz * (0.5f - qxqx - qyqy) - magVec.z,
    };

    quat_t grad = (quat_t){
        -_2qy * func_g.x + _2qx * func_g.y - _2bzqy * func_b.x + (-_2bxqz + _2bzqx) * func_b.y + _2bxqy * func_b.z,
        +_2qz * func_g.x + _2qw * func_g.y - 2 * _2qx * func_g.z + _2bz * q->z * func_b.x + (_2bxqy + _2bz * q->w) * func_b.y + (_2bxqz - 2 * _2bzqx) * func_b.z,
        -_2qw * func_g.x + _2qz * func_g.y - 2 * _2qy * func_g.z + (-2 * _2bxqy - _2bz * q->w) * func_b.x + (_2bx * q->x + _2bz * q->z) * func_b.y + (_2bx * q->w - 2 * _2bzqy) * func_b.z,
        +_2qx * func_g.x + _2qy * func_g.y + (-2 * _2bxqz + _2bzqx) * func_b.x + (-_2bx * q->w + _2bzqy) * func_b.y + _2bx * q->x * func_b.z,
    };
    quat_normalize(&grad);

    quat_t derivative = quat_gyro_derivative(q, &gyroVec);
    derivative.w -= data->beta * grad.w;
    derivative.x -= data->beta * grad.x;
    derivative.y -= data->beta * grad.y;
    derivative.z -= data->beta * grad.z;

    q->w += derivative.w * data->dt;
    q->x += derivative.x * data->dt;
    q->y += derivative.y * data->dt;
    q->z += derivative.z * data->dt;

    quat_normalize(q);
}

static void _mahony_init(mahony_data_t *data, float kp, float ki, float dt)
{
    data->Kp = kp;
    data->Ki = ki;
    data->integralFBx = 0.0f;
    data->integralFBy = 0.0f;
    data->integralFBz = 0.0f;
    data->dt = dt;
}

static void _mahony_update_imu(mahony_data_t *data, quat_t *q, vec3_t gyroVec, vec3_t accVec)
{
    vec3_normalize(&accVec);

    const float vx = 2.0f * (q->x * q->z - q->w * q->y);
    const float vy = 2.0f * (q->w * q->x + q->y * q->z);
    const float vz = q->w * q->w - q->x * q->x - q->y * q->y + q->z * q->z;

    _mahony_update_base(
        data,
        q,
        gyroVec,
        accVec.y * vz - accVec.z * vy,
        accVec.z * vx - accVec.x * vz,
        accVec.x * vy - accVec.y * vx);
}

static void _mahony_update_marg(mahony_data_t *data, quat_t *q, vec3_t gyroVec, vec3_t accVec, vec3_t magVec)
{
    vec3_normalize(&accVec);
    vec3_normalize(&magVec);

    const float qwqw = q->w * q->w;
    const float qwqx = q->w * q->x;
    const float qwqy = q->w * q->y;
    const float qwqz = q->w * q->z;
    const float qxqx = q->x * q->x;
    const float qxqy = q->x * q->y;
    const float qxqz = q->x * q->z;
    const float qyqy = q->y * q->y;
    const float qyqz = q->y * q->z;
    const float qzqz = q->z * q->z;

    const float hx = 2.0f * (magVec.x * (0.5f - qyqy - qzqz) + magVec.y * (qxqy - qwqz) + magVec.z * (qxqz + qwqy));
    const float hy = 2.0f * (magVec.x * (qxqy + qwqz) + magVec.y * (0.5f - qxqx - qzqz) + magVec.z * (qyqz - qwqx));
    const float bx = sqrtf(hx * hx + hy * hy);
    const float bz = 2.0f * (magVec.x * (qxqz - qwqy) + magVec.y * (qyqz + qwqx) + magVec.z * (0.5f - qxqx - qyqy));

    const float vx = 2.0f * (qxqz - qwqy);
    const float vy = 2.0f * (qwqx + qyqz);
    const float vz = qwqw - qxqx - qyqy + qzqz;
    const float wx = 2.0f * (bx * (0.5f - qyqy - qzqz) + bz * (qxqz - qwqy));
    const float wy = 2.0f * (bx * (qxqy - qwqz) + bz * (qwqx + qyqz));
    const float wz = 2.0f * (bx * (qwqy + qxqz) + bz * (0.5f - qxqx - qyqy));

    _mahony_update_base(
        data,
        q,
        gyroVec,
        (accVec.y * vz - accVec.z * vy) + (magVec.y * wz - magVec.z * wy),
        (accVec.z * vx - accVec.x * vz) + (magVec.z * wx - magVec.x * wz),
        (accVec.x * vy - accVec.y * vx) + (magVec.x * wy - magVec.y * wx));
}

static void _mahony_update_base(mahony_data_t *data, quat_t *q, vec3_t gyroVec, float ex, float ey, float ez)
{
    if (data->Ki > 0.0f)
    {
        data->integralFBx += data->Ki * ex * data->dt;
        data->integralFBy += data->Ki * ey * data->dt;
        data->integralFBz += data->Ki * ez * data->dt;

        gyroVec.x += data->integralFBx;
        gyroVec.y += data->integralFBy;
        gyroVec.z += data->integralFBz;
    }

    gyroVec.x += data->Kp * ex;
    gyroVec.y += data->Kp * ey;
    gyroVec.z += data->Kp * ez;

    _integrate_gyro(q, gyroVec, data->dt);
}

/**
 * REF: https://ahrs.readthedocs.io/en/latest/filters/angular.html
 */
static void _integrate_gyro(quat_t *q, vec3_t gyro, float dt)
{
    quat_t derivative = quat_gyro_derivative(q, &gyro);

    q->w += derivative.w * dt;
    q->x += derivative.x * dt;
    q->y += derivative.y * dt;
    q->z += derivative.z * dt;

    quat_normalize(q);
}

static void _ekf_init(ekf_data_t *data)
{
    data->cfg = (ekf_config_t){
        .dt = 0.0025f,
        .varAcc = EKF_ACC_VARIANCE,
        .varGPS = EKF_GPS_VARIANCE,
        .varBar = EKF_BARO_VARIANCE,
    };

    memset(data->x.data, 0, sizeof(data->x.data));

    for (size_t i = 0; i < EKF_NUM_STATES; i++)
    {
        for (size_t j = 0; j < EKF_NUM_STATES; j++)
        {
            data->P[i][j] = i == j ? 10.0f : 0.0f;
            data->P_Next[i][j] = 0.0f;
        }
    }
}

static void _ekf_predict_state(ekf_data_t *data, const ekf_controls_t *controls)
{
    const float dt = data->cfg.dt;

    ekf_state_t newState = {
        .pos_n = data->x.pos_n + data->x.vel_n * dt + 0.5f * controls->acc_n * dt * dt,
        .pos_e = data->x.pos_e + data->x.vel_e * dt + 0.5f * controls->acc_e * dt * dt,
        .pos_d = data->x.pos_d + data->x.vel_d * dt + 0.5f * controls->acc_d * dt * dt,
        .vel_n = data->x.vel_n + controls->acc_n * dt,
        .vel_e = data->x.vel_e + controls->acc_e * dt,
        .vel_d = data->x.vel_d + controls->acc_d * dt,
    };

    data->x = newState;
}

static void _ekf_predict_covariance(ekf_data_t *data, const ekf_controls_t *controls)
{
    const float var_acc = data->cfg.varAcc;
    const float dt = data->cfg.dt;

    const float PS0 = (1.0f / 4.0f) * var_acc * (dt * dt * dt * dt);
    const float PS1 = data->P[3][3] * dt;
    const float PS2 = PS1 + data->P[0][3];
    const float PS3 = data->P[3][4] * dt;
    const float PS4 = PS3 + data->P[0][4];
    const float PS5 = data->P[3][5] * dt;
    const float PS6 = PS5 + data->P[0][5];
    const float PS7 = (1.0f / 2.0f) * var_acc * (dt * dt * dt);
    const float PS8 = data->P[4][3] * dt;
    const float PS9 = PS8 + data->P[1][3];
    const float PS10 = data->P[4][4] * dt;
    const float PS11 = PS10 + data->P[1][4];
    const float PS12 = data->P[4][5] * dt;
    const float PS13 = PS12 + data->P[1][5];
    const float PS14 = data->P[5][3] * dt;
    const float PS15 = PS14 + data->P[2][3];
    const float PS16 = data->P[5][4] * dt;
    const float PS17 = PS16 + data->P[2][4];
    const float PS18 = data->P[5][5] * dt;
    const float PS19 = PS18 + data->P[2][5];
    const float PS20 = var_acc * (dt * dt);

    data->P_Next[0][0] = PS0 + PS2 * dt + data->P[0][0] + data->P[3][0] * dt;
    data->P_Next[0][1] = PS4 * dt + data->P[0][1] + data->P[3][1] * dt;
    data->P_Next[0][2] = PS6 * dt + data->P[0][2] + data->P[3][2] * dt;
    data->P_Next[0][3] = PS2 + PS7;
    data->P_Next[0][4] = PS4;
    data->P_Next[0][5] = PS6;
    data->P_Next[1][1] = PS0 + PS11 * dt + data->P[1][1] + data->P[4][1] * dt;
    data->P_Next[1][2] = PS13 * dt + data->P[1][2] + data->P[4][2] * dt;
    data->P_Next[1][3] = PS9;
    data->P_Next[1][4] = PS11 + PS7;
    data->P_Next[1][5] = PS13;
    data->P_Next[2][2] = PS0 + PS19 * dt + data->P[2][2] + data->P[5][2] * dt;
    data->P_Next[2][3] = PS15;
    data->P_Next[2][4] = PS17;
    data->P_Next[2][5] = PS19 + PS7;
    data->P_Next[3][3] = PS20 + data->P[3][3];
    data->P_Next[3][4] = data->P[3][4];
    data->P_Next[3][5] = data->P[3][5];
    data->P_Next[4][4] = PS20 + data->P[4][4];
    data->P_Next[4][5] = data->P[4][5];
    data->P_Next[5][5] = PS20 + data->P[5][5];

    for (size_t i = 0; i < EKF_NUM_STATES; i++)
    {
        for (size_t j = 0; j < EKF_NUM_STATES; j++)
        {
            if (j >= i)
            {
                data->P[i][j] = data->P_Next[i][j];
                data->P[j][i] = data->P_Next[i][j];
            }
        }
    }
}

static void _ekf_fusion(ekf_data_t *data, const ekf_controls_t *controls, const ekf_measurements_t *measurements)
{
    const float var_gps = data->cfg.varGPS;
    const float var_baro = data->cfg.varBar;

    float H[EKF_NUM_MEASUREMENTS][EKF_NUM_STATES];

    H[0][0] = 1;
    H[0][1] = 0;
    H[0][2] = 0;
    H[0][3] = 0;
    H[0][4] = 0;
    H[0][5] = 0;
    H[1][0] = 0;
    H[1][1] = 1;
    H[1][2] = 0;
    H[1][3] = 0;
    H[1][4] = 0;
    H[1][5] = 0;
    H[2][0] = 0;
    H[2][1] = 0;
    H[2][2] = 1;
    H[2][3] = 0;
    H[2][4] = 0;
    H[2][5] = 0;
    H[3][0] = 0;
    H[3][1] = 0;
    H[3][2] = 1;
    H[3][3] = 0;
    H[3][4] = 0;
    H[3][5] = 0;

    float K[EKF_NUM_STATES][EKF_NUM_MEASUREMENTS];

    const float KS0 = 1.0f / (data->P[0][0] + var_gps);
    const float KS1 = 1.0f / (data->P[1][1] + var_gps);
    const float KS2 = data->P[2][2] * var_baro;
    const float KS3 = data->P[2][2] * var_gps;
    const float KS4 = 1.0f / (KS2 + KS3 + var_baro * var_gps);
    const float KS5 = KS4 * data->P[0][2];
    const float KS6 = KS4 * data->P[1][2];
    const float KS7 = KS4 * data->P[3][2];
    const float KS8 = KS4 * data->P[4][2];
    const float KS9 = KS4 * data->P[5][2];

    K[0][0] = KS0 * data->P[0][0];
    K[0][1] = KS1 * data->P[0][1];
    K[0][2] = KS5 * var_baro;
    K[0][3] = KS5 * var_gps;
    K[1][0] = KS0 * data->P[1][0];
    K[1][1] = KS1 * data->P[1][1];
    K[1][2] = KS6 * var_baro;
    K[1][3] = KS6 * var_gps;
    K[2][0] = KS0 * data->P[2][0];
    K[2][1] = KS1 * data->P[2][1];
    K[2][2] = KS2 * KS4;
    K[2][3] = KS3 * KS4;
    K[3][0] = KS0 * data->P[3][0];
    K[3][1] = KS1 * data->P[3][1];
    K[3][2] = KS7 * var_baro;
    K[3][3] = KS7 * var_gps;
    K[4][0] = KS0 * data->P[4][0];
    K[4][1] = KS1 * data->P[4][1];
    K[4][2] = KS8 * var_baro;
    K[4][3] = KS8 * var_gps;
    K[5][0] = KS0 * data->P[5][0];
    K[5][1] = KS1 * data->P[5][1];
    K[5][2] = KS9 * var_baro;
    K[5][3] = KS9 * var_gps;

    float innov[EKF_NUM_MEASUREMENTS];

    innov[0] = measurements->data[0] - data->x.data[0];
    innov[1] = measurements->data[1] - data->x.data[1];
    innov[2] = measurements->data[2] - data->x.data[2];
    innov[3] = measurements->data[3] - data->x.data[2];

    float *states = data->x.data;

    for (size_t i = 0; i < EKF_NUM_STATES; i++)
    {
        for (size_t j = 0; j < EKF_NUM_MEASUREMENTS; j++)
        {
            states[i] += K[i][j] * innov[j];
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
                tmp += K[i][k] * H[k][j];
            }

            I_KH[i][j] = (i == j ? 1.0f : 0.0f) - tmp;
        }
    }

    for (size_t i = 0; i < EKF_NUM_STATES; i++)
    {
        for (size_t j = 0; j < EKF_NUM_STATES; j++)
        {
            float tmp = 0.0f;

            for (size_t k = 0; k < EKF_NUM_STATES; k++)
            {
                tmp += I_KH[i][k] * data->P[k][j];
            }

            data->P_Next[i][j] = tmp;
        }
    }

    for (size_t i = 0; i < EKF_NUM_STATES; i++)
    {
        for (size_t j = 0; j < EKF_NUM_STATES; j++)
        {
            data->P[i][j] = data->P_Next[i][j];
        }
    }
}

static void _ekf_force_cov_symmetry(ekf_data_t *data)
{
    for (size_t i = 0; i < EKF_NUM_STATES; i++)
    {
        for (size_t j = 0; j <= i; j++)
        {
            float tmp = (data->P[i][j] + data->P[j][i]) / 2.0f;

            data->P[i][j] = tmp;
            data->P[j][i] = tmp;
        }
    }
}

static void _ekf_update(ekf_data_t *data, const ekf_controls_t *controls, const ekf_measurements_t *measurements)
{
    _ekf_predict_state(data, controls);
    _ekf_predict_covariance(data, controls);

    _ekf_fusion(data, controls, measurements);

    _ekf_force_cov_symmetry(data);
}