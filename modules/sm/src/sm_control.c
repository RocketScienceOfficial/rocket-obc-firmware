#include "modules/sm/sm_control.h"
#include "lib/maths/math_utils.h"

#define ACCEL_EPS 0.2f
#define FLIGHT_SM_ACCEL_THRESHOLD 80.0f
#define FLIGHT_SM_FREE_FLIGHT_THRESHOLD -3.0f
#define LAST_ALT_VERIFICATION_COUNT 10

void sm_init(sm_data_t *data)
{
    data->state = FLIGHT_STATE_STANDING;
    data->apogee = 0;
    data->apogeeReached = false;
    data->lastAlt = 0;
    data->lastAltInd = 0;
}

void sm_update(sm_data_t *data, sm_input_t *input)
{
    float acc = vec3_mag(&input->accel);
    float alt = input->alt;

    if (acc >= FLIGHT_SM_ACCEL_THRESHOLD)
    {
        data->state = FLIGHT_STATE_ACCELERATING;
    }
    else if (input->accel.z < FLIGHT_SM_FREE_FLIGHT_THRESHOLD)
    {
        data->state = FLIGHT_STATE_FREE_FLIGHT;
    }
    else if (value_approx_eql(acc, 0.0f, ACCEL_EPS))
    {
        data->state = FLIGHT_STATE_LANDED;
    }

    if (alt <= data->lastAlt)
    {
        if (data->lastAltInd == 0)
        {
            data->apogee = alt;
        }

        data->lastAltInd++;

        if (data->lastAltInd == LAST_ALT_VERIFICATION_COUNT)
        {
            data->state = FLIGHT_STATE_FREE_FALL;
            data->apogeeReached = true;
        }
    }
    else
    {
        data->lastAltInd = 0;
    }

    data->lastAlt = alt;
}