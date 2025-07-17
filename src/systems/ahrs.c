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
#define ORIENTATION_MADGWICK_BETA 0.033f
#define ORIENTATION_ACCELERATION_SWITCH_THRESHOLD (2.0f * EARTH_GRAVITY)
#define ORIENTATION_HIGH_GAIN_DISABLE_TIME_MS 3000
#define EKF_ACC_VARIANCE 0.02f
#define EKF_GPS_VARIANCE 1.7f
#define EKF_BARO_VARIANCE 0.25f
#define EKF_OUTDATED_MEASUREMENT_VARIANCE 1000000

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

#define EKF_NUM_STATES 2
#define EKF_NUM_CONTROLS 1
#define EKF_NUM_MEASUREMENTS 2

typedef union ekf_state
{
    struct
    {
        float pos;
        float vel;
    };

    float data[EKF_NUM_STATES];
} ekf_state_t;

typedef union ekf_controls
{
    struct
    {
        float acc;
    };

    float data[EKF_NUM_CONTROLS];
} ekf_controls_t;

typedef union ekf_measurements
{
    struct
    {
        float gps_alt;
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
static madgwick_data_t s_Madgwick;
static ekf_data_t s_EKF;
static geo_position_wgs84_t s_BaseGPSPos;
static vec3_prec_t s_NEDPos;
static float s_BaroHeightOffset;
static float s_BaroHeight;
static hal_msec_t s_HighGainDisableTimer;
static bool s_IsStreamingEKFData;
static hal_msec_t s_StreamTimer;

static void _madgwick_init(madgwick_data_t *data, float beta, float dt);
static void _madgwick_update_imu(madgwick_data_t *data, quat_t *q, vec3_t gyroVec, vec3_t accVec);
static void _madgwick_update_marg(madgwick_data_t *data, quat_t *q, vec3_t gyroVec, vec3_t accVec, vec3_t magVec);
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

    _madgwick_init(&s_Madgwick, 1.0f, 0.0025f);
    _ekf_init(&s_EKF);

    s_HighGainDisableTimer = hal_time_get_ms_since_boot();

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
        s_EKF.cfg.dt = sensors_get_frame()->measurementDt;
        s_Madgwick.dt = sensors_get_frame()->measurementDt;

        if (s_HighGainDisableTimer != 0 && hal_time_get_ms_since_boot() - s_HighGainDisableTimer >= ORIENTATION_HIGH_GAIN_DISABLE_TIME_MS)
        {
            s_HighGainDisableTimer = 0;
            s_Madgwick.beta = ORIENTATION_MADGWICK_BETA;
        }

        if (vec3_mag_compare(&sensors_get_frame()->acc1, ORIENTATION_ACCELERATION_SWITCH_THRESHOLD) < 0)
        {
            _madgwick_update_imu(&s_Madgwick, &s_CurrentData.orientation, sensors_get_frame()->gyro1, sensors_get_frame()->acc1);
        }
        else
        {
            _integrate_gyro(&s_CurrentData.orientation, sensors_get_frame()->gyro1, sensors_get_frame()->measurementDt);
        }

        if (s_HighGainDisableTimer == 0)
        {
            vec3_t newAcc = sensors_get_frame()->acc1;
            quat_rotate_vec(&newAcc, &s_CurrentData.orientation);
            newAcc.x *= -1;
            newAcc.y *= -1;
            newAcc.z -= EARTH_GRAVITY;

            s_CurrentData.acceleration = newAcc;

            ekf_controls_t controls = {
                .acc = newAcc.z,
            };

            ekf_measurements_t measurements = {
                .gps_alt = s_NEDPos.z,
                .baroHeight = s_BaroHeight,
            };

            _ekf_update(&s_EKF, &controls, &measurements);

            s_CurrentData.position.x = s_NEDPos.x;
            s_CurrentData.position.y = s_NEDPos.y;
            s_CurrentData.position.z = s_EKF.x.pos;
            s_CurrentData.velocity.x = 0;
            s_CurrentData.velocity.y = 0;
            s_CurrentData.velocity.z = s_EKF.x.vel;

            s_EKF.cfg.varGPS = EKF_OUTDATED_MEASUREMENT_VARIANCE;
            s_EKF.cfg.varBar = EKF_OUTDATED_MEASUREMENT_VARIANCE;
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

static void _madgwick_init(madgwick_data_t *data, float beta, float dt)
{
    data->beta = beta;
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
        .pos = data->x.pos + data->x.vel * dt + 0.5f * controls->acc * dt * dt,
        .vel = data->x.vel + controls->acc * dt,
    };

    data->x = newState;
}

static void _ekf_predict_covariance(ekf_data_t *data, const ekf_controls_t *controls)
{
    const float var_acc = data->cfg.varAcc;
    const float dt = data->cfg.dt;

    const float PS0 = data->P[1][1] * dt;
    const float PS1 = PS0 + data->P[0][1];
    const float PS2 = (1.0f / 2.0f) * var_acc * (dt * dt * dt);

    data->P_Next[0][0] = PS1 * dt + data->P[0][0] + data->P[1][0] * dt + (1.0f / 4.0f) * var_acc * (dt * dt * dt * dt);
    data->P_Next[0][1] = PS1 + PS2;
    data->P_Next[1][1] = data->P[1][1] + var_acc * (dt * dt);

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
    H[1][0] = 1;
    H[1][1] = 0;

    float K[EKF_NUM_STATES][EKF_NUM_MEASUREMENTS];

    const float KS0 = 1.0f / (data->P[0][0] + var_gps);
    const float KS1 = 1.0f / (data->P[0][0] + var_baro);

    K[0][0] = KS0 * data->P[0][0];
    K[0][1] = KS1 * data->P[0][0];
    K[1][0] = KS0 * data->P[1][0];
    K[1][1] = KS1 * data->P[1][0];

    float innov[EKF_NUM_MEASUREMENTS];

    innov[0] = measurements->data[0] - data->x.data[0];
    innov[1] = measurements->data[1] - data->x.data[0];

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