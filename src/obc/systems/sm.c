#include "sm.h"
#include "sensors.h"
#include "../middleware/events.h"
#include "../middleware/syslog.h"
#include "lib/maths/math_utils.h"
#include "hal/serial_driver.h"
#include <stddef.h>

#define SYSTEM_NAME "sm"

#define ACCEL_EPS 0.2f
#define FLIGHT_SM_ACCEL_THRESHOLD 80.0f
#define FLIGHT_SM_FREE_FLIGHT_THRESHOLD -3.0f
#define LAST_ALT_VERIFICATION_COUNT 10

static flight_state_type_t s_State;
static float s_Apogee;
static bool s_ApogeeReached;
static float s_LastAlt;
static size_t s_LastAltInd;

void sm_init(void)
{
}

void sm_update(void)
{
    vec3_t accel = sensors_get_frame()->acc1;
    float acc = vec3_mag(&accel);
    float alt = sensors_get_frame()->pos.alt;

    if (acc >= FLIGHT_SM_ACCEL_THRESHOLD)
    {
        s_State = FLIGHT_STATE_ACCELERATING;

        SYS_LOG("State: Accelerating");
    }
    else if (accel.z < FLIGHT_SM_FREE_FLIGHT_THRESHOLD)
    {
        s_State = FLIGHT_STATE_FREE_FLIGHT;

        SYS_LOG("State: Free Flight");
    }
    else if (value_approx_eql(acc, 0.0f, ACCEL_EPS))
    {
        s_State = FLIGHT_STATE_LANDED;

        SYS_LOG("State: Landed");
    }

    if (alt <= s_LastAlt)
    {
        if (s_LastAltInd == 0)
        {
            s_Apogee = alt;
        }

        s_LastAltInd++;

        if (s_LastAltInd == LAST_ALT_VERIFICATION_COUNT)
        {
            s_State = FLIGHT_STATE_FREE_FALL;
            s_ApogeeReached = true;

            SYS_LOG("State: Free Fall");
        }
    }
    else
    {
        s_LastAltInd = 0;
    }

    s_LastAlt = alt;
}

flight_state_type_t sm_get_state(void)
{
    return s_State;
}