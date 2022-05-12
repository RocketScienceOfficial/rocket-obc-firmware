#pragma once

typedef struct kalman_filter_data
{
    union
    {
        double noiseCovariance;
        double measurementMapScalar;
        double initialErrorCovariance;
        double initialEstimateErrorCovariance;
        double initialEstimateState;
        double initialKalmanGain;
    };

    union
    {
        double R;
        double H;
        double Q;
        double P;
        double U_hat;
        double K;
    };

} kalman_filter_data_t;

double kalman(kalman_filter_data_t *data, double u);