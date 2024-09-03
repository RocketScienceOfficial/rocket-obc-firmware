#include "sm.h"
#include "sensors.h"
#include "ahrs.h"
#include "../middleware/events.h"
#include "../middleware/syslog.h"
#include "lib/maths/math_utils.h"
#include "lib/geo/physical_constants.h"
#include "hal/serial_driver.h"
#include <stddef.h>
#include <math.h>

#define SYSTEM_NAME "sm"
#define START_ACC_THRESHOLD 35
#define START_ALT_THRESHOLD 3
#define START_ALT_VERIFICATION_COUNT 300
#define APOGEE_MAX_DELTA 2
#define LAND_MAX_DELTA 1
#define LAST_ALT_APOGEE_VERIFICATION_COUNT 200
#define LAST_ALT_LAND_VERIFICATION_COUNT 300

static flight_state_type_t s_State;
static float s_BaseAlt;
static bool s_VerifingStandingAlt;
static size_t s_StandingAltVerificationIndex;
static float s_Apogee;
static bool s_ApogeeReached;
static size_t s_LastAltApogeeVertificationIndex;
static float s_LandingAlt;
static size_t s_LastAltLandVerificationIndex;

void sm_init(void)
{
    SYS_LOG("READY");
}

void sm_update(void)
{
    if (s_State == FLIGHT_STATE_STANDING)
    {
        if (events_poll(MSG_SENSORS_NORMAL_READ) && !s_VerifingStandingAlt)
        {
            vec3_t accel = sensors_get_frame()->acc1;
            float acc_mag = vec3_mag(&accel);

            if (acc_mag >= START_ACC_THRESHOLD)
            {
                s_VerifingStandingAlt = true;

                SYS_LOG("Started verifing altitude");
            }
        }

        if (events_poll(MSG_SENSORS_NORMAL_READ) && s_VerifingStandingAlt)
        {
            float alt = ahrs_get_data()->position.z;

            if (s_BaseAlt == 0)
            {
                s_BaseAlt = alt;
            }
            else
            {
                if (alt - s_BaseAlt >= START_ALT_THRESHOLD)
                {
                    s_State = FLIGHT_STATE_ACCELERATING;

                    SYS_LOG("State: Accelerating");
                }
                else
                {
                    s_StandingAltVerificationIndex++;

                    if (s_StandingAltVerificationIndex == START_ALT_VERIFICATION_COUNT)
                    {
                        SYS_LOG("Acceleration state verification unsuccessfull");

                        s_BaseAlt = 0;
                        s_VerifingStandingAlt = false;
                        s_StandingAltVerificationIndex = 0;
                    }
                }
            }
        }
    }
    else if (s_State == FLIGHT_STATE_ACCELERATING)
    {
        if (events_poll(MSG_SENSORS_NORMAL_READ))
        {
            vec3_t accel = sensors_get_frame()->acc1;
            float acc_mag = vec3_mag(&accel);

            if (acc_mag < EARTH_GRAVITY)
            {
                s_State = FLIGHT_STATE_FREE_FLIGHT;

                SYS_LOG("State: Free Flight");
            }
        }
    }
    else if (s_State == FLIGHT_STATE_FREE_FLIGHT)
    {
        if (events_poll(MSG_SENSORS_NORMAL_READ))
        {
            float alt = ahrs_get_data()->position.z - s_BaseAlt;

            if (alt <= s_Apogee || alt - s_Apogee <= APOGEE_MAX_DELTA)
            {
                s_LastAltApogeeVertificationIndex++;

                if (s_LastAltApogeeVertificationIndex == LAST_ALT_APOGEE_VERIFICATION_COUNT)
                {
                    s_State = FLIGHT_STATE_FREE_FALL;
                    s_ApogeeReached = true;

                    SYS_LOG("Apogee reached: %f", s_Apogee);
                    SYS_LOG("State: Free Fall");

                    events_publish(MSG_SM_APOGEE_REACHED);
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
        if (events_poll(MSG_SENSORS_NORMAL_READ))
        {
            float alt = ahrs_get_data()->position.z - s_BaseAlt;
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

float sm_get_base_alt(void)
{
    return s_BaseAlt;
}

float sm_get_apogee(void)
{
    return s_ApogeeReached ? s_Apogee : 0;
}