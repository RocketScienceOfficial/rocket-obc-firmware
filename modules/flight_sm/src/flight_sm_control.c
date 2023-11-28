#include "modules/flight_sm/flight_sm_control.h"

void flight_sm_init(flight_sm_data_t *data)
{
    data->type = FLIGHT_STATE_STANDING;
}

void flight_sm_update(flight_sm_data_t *data, flight_sm_input_t *input)
{
}