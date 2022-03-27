#include "kalman.h"
#include "pico/time.h"

int micros()
{
    return to_ms_since_boot(get_absolute_time());
}

void kalman_init(struct KalmanFilter *filter, double angle, double bias, double measure)
{
    filter->Q_angle = angle;
    filter->Q_bias = bias;
    filter->R_measure = measure;

    filter->K_angle = 0;
    filter->K_bias = 0;

    filter->P[0][0] = 0;
    filter->P[0][1] = 0;
    filter->P[1][0] = 0;
    filter->P[1][1] = 0;

    filter->kt = (double)micros();
}

double kalman_update(struct KalmanFilter *filter, double newValue, double newRate)
{
    filter->dt = (double)(micros() - filter->kt) / 1000;

    filter->K_rate = newRate - filter->K_bias;
    filter->K_angle += filter->dt * filter->K_rate;

    filter->P[0][0] += filter->dt * (filter->P[1][1] + filter->P[0][1]) + filter->Q_angle * filter->dt;
    filter->P[0][1] -= filter->dt * filter->P[1][1];
    filter->P[1][0] -= filter->dt * filter->P[1][1];
    filter->P[1][1] += filter->Q_bias * filter->dt;

    filter->S = filter->P[0][0] + filter->R_measure;

    filter->K[0] = filter->P[0][0] / filter->S;
    filter->K[1] = filter->P[1][0] / filter->S;

    filter->y = newValue - filter->K_angle;

    filter->K_angle += filter->K[0] * filter->y;
    filter->K_bias += filter->K[1] * filter->y;

    filter->P[0][0] -= filter->K[0] * filter->P[0][0];
    filter->P[0][1] -= filter->K[0] * filter->P[0][1];
    filter->P[1][0] -= filter->K[1] * filter->P[0][0];
    filter->P[1][1] -= filter->K[1] * filter->P[0][1];

    filter->kt = (double)micros();

    return filter->K_angle;
};