#include "modules/ekf/ekf.h"

void ekf_predict_state(ekf_data_t *data, ekf_controls_t *controls)
{
    data->state.orientation.w = -data->config.dt * data->state.orientation.x * (controls->gyro.x - data->state.gyroBias.x) / 2.0f - data->config.dt * data->state.orientation.y * (controls->gyro.y - data->state.gyroBias.y) / 2.0f - data->config.dt * data->state.orientation.z * (controls->gyro.z - data->state.gyroBias.z) / 2.0f + data->state.orientation.w;
    data->state.orientation.x = +data->config.dt * data->state.orientation.w * (controls->gyro.x - data->state.gyroBias.x) / 2.0f + data->config.dt * data->state.orientation.y * (controls->gyro.z - data->state.gyroBias.z) / 2.0f - data->config.dt * data->state.orientation.z * (controls->gyro.y - data->state.gyroBias.y) / 2.0f + data->state.orientation.x;
    data->state.orientation.y = +data->config.dt * data->state.orientation.w * (controls->gyro.y - data->state.gyroBias.y) / 2.0f - data->config.dt * data->state.orientation.x * (controls->gyro.z - data->state.gyroBias.z) / 2.0f + data->config.dt * data->state.orientation.z * (controls->gyro.x - data->state.gyroBias.x) / 2.0f + data->state.orientation.y;
    data->state.orientation.z = +data->config.dt * data->state.orientation.w * (controls->gyro.z - data->state.gyroBias.z) / 2.0f + data->config.dt * data->state.orientation.x * (controls->gyro.y - data->state.gyroBias.y) / 2.0f - data->config.dt * data->state.orientation.y * (controls->gyro.x - data->state.gyroBias.x) / 2.0f + data->state.orientation.z;
}