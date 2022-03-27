#ifndef KALMAN_H
#define KALMAN_H

struct KalmanFilter
{
    double Q_angle, Q_bias, R_measure;
    double K_angle, K_bias, K_rate;
    double P[2][2], K[2];
    double S, y;
    double dt, kt;
};

void kalman_init(struct KalmanFilter *filter, double angle, double bias, double measure);
double kalman_update(struct KalmanFilter *filter, double newValue, double newRate);

#endif