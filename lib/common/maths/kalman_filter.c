#include "kalman_filter.h"

void kalmanInit(kalman_filter_data_t *data)
{
    data->R = 40;
    data->H = 1.00;
    data->Q = 10;
    data->P = 0;
    data->U_hat = 0;
    data->K = 0;
}

double kalman(kalman_filter_data_t *data, double u)
{
    data->K = data->P * data->H / (data->H * data->P * data->H + data->R);
    data->U_hat = data->U_hat + data->K * (u - data->H * data->U_hat);
    data->P = (1 - data->K * data->H) * data->P + data->Q;

    return data->U_hat;
}