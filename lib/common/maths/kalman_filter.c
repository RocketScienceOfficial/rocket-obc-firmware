#include "kalman_filter.h"

double kalman(kalman_filter_data_t *data, double u)
{
    data->K = data->P * data->H / (data->H * data->P * data->H + data->R);
    data->U_hat = data->U_hat + data->K * (u - data->H * data->U_hat);
    data->P = (1 - data->K * data->H) * data->P + data->Q;

    return data->U_hat;
}