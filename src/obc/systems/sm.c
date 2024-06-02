#include "sm.h"
#include "sensors.h"
#include "../middleware/events.h"
#include "../middleware/syslog.h"
#include "lib/maths/math_utils.h"
#include "lib/geo/geo_utils.h"
#include "lib/geo/physical_constants.h"
#include "hal/serial_driver.h"
#include <stddef.h>
#include <math.h>

#define SYSTEM_NAME "sm"

#define START_ACC_THRESHOLD 25
#define APOGEE_MAX_DELTA 4
#define LAND_MAX_DELTA 4
#define LAST_ALT_APOGEE_VERIFICATION_COUNT 400
#define LAST_ALT_LAND_VERIFICATION_COUNT 400

static flight_state_type_t s_State;
static float s_Apogee;
static bool s_ApogeeReached;
static size_t s_LastAltApogeeVertificationIndex;
static float s_LandingAlt;
static size_t s_LastAltLandVerificationIndex;

void sm_init(void)
{
}

void sm_update(void)
{
    if (s_State == FLIGHT_STATE_STANDING)
    {
        if (events_poll(MSG_SENSORS_NORMAL_READ))
        {
            vec3_t accel = sensors_get_frame()->acc1;
            float acc_mag = vec3_mag(&accel);

            if (acc_mag >= START_ACC_THRESHOLD)
            {
                s_State = FLIGHT_STATE_ACCELERATING;

                SYS_LOG("State: Accelerating");
            }
        }
    }
    else if (s_State == FLIGHT_STATE_ACCELERATING)
    {
        if (events_poll(MSG_SENSORS_NORMAL_READ))
        {
            vec3_t accel = sensors_get_frame()->acc1;
            float acc_mag = vec3_mag(&accel);

            if (acc_mag < -EARTH_GRAVITY) // TODO: Change to positive
            {
                s_State = FLIGHT_STATE_FREE_FLIGHT;

                SYS_LOG("State: Free Flight");
            }
        }
    }
    else if (s_State == FLIGHT_STATE_FREE_FLIGHT)
    {
        if (events_poll(MSG_SENSORS_BARO_READ))
        {
            float alt = height_from_baro_formula(sensors_get_frame()->press);

            if (alt <= s_Apogee || alt - s_Apogee <= APOGEE_MAX_DELTA)
            {
                s_LastAltApogeeVertificationIndex++;

                if (s_LastAltApogeeVertificationIndex == LAST_ALT_APOGEE_VERIFICATION_COUNT)
                {
                    s_State = FLIGHT_STATE_FREE_FALL;
                    s_ApogeeReached = true;

                    SYS_LOG("Apogee reached: %f\n", s_Apogee);
                    SYS_LOG("State: Free Fall");
                }
            }
            else
            {
                s_Apogee = alt;
                s_LastAltApogeeVertificationIndex = 0;
            }
        }
    }
    else if (s_State == FLIGHT_STATE_FREE_FALL)
    {
        if (events_poll(MSG_SENSORS_BARO_READ))
        {
            float alt = height_from_baro_formula(sensors_get_frame()->press);
            float delta = fabsf(s_LandingAlt - alt);

            if (delta > LAND_MAX_DELTA)
            {
                s_LandingAlt = alt;
                s_LastAltLandVerificationIndex = 0;
            }
            else
            {
                s_LastAltLandVerificationIndex++;

                if (s_LastAltLandVerificationIndex == LAST_ALT_LAND_VERIFICATION_COUNT)
                {
                    s_State = FLIGHT_STATE_LANDED;

                    SYS_LOG("State: Landed");
                }
            }
        }
    }
}

flight_state_type_t sm_get_state(void)
{
    return s_State;
}