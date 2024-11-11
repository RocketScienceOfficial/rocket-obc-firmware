#include "sm.h"
#include "sensors.h"
#include "ahrs.h"
#include "radio.h"
#include "serial.h"
#include "../middleware/events.h"
#include "lib/maths/math_utils.h"
#include "lib/geo/physical_constants.h"
#include "hal/serial_driver.h"
#include <stddef.h>
#include <math.h>

#define START_ACC_THRESHOLD 35
#define START_ALT_THRESHOLD 3
#define START_ALT_VERIFICATION_COUNT 300
#define APOGEE_MAX_DELTA 2
#define LAND_MAX_DELTA 1
#define LAST_ALT_APOGEE_VERIFICATION_COUNT 200
#define LAST_ALT_LAND_VERIFICATION_COUNT 300

static bool s_Armed;
static flight_state_type_t s_State;
static float s_BaseAlt;
static bool s_VerifingStandingAlt;
static size_t s_StandingAltVerificationIndex;
static float s_Apogee;
static bool s_ApogeeReached;
static size_t s_LastAltApogeeVertificationIndex;
static float s_LandingAlt;
static size_t s_LastAltLandVerificationIndex;

static void _handle_radio_packet(void);
static void _handle_state_standing(void);
static void _handle_state_accelerating(void);
static void _handle_state_free_flight(void);
static void _handle_state_free_fall(void);
static void _handle_state_landed(void);

void sm_init(void)
{
    SERIAL_DEBUG_PRINTF("READY");
}

void sm_update(void)
{
    if (s_State == FLIGHT_STATE_STANDING)
    {
        _handle_state_standing();
    }
    else if (s_State == FLIGHT_STATE_ACCELERATING)
    {
        _handle_state_accelerating();
    }
    else if (s_State == FLIGHT_STATE_FREE_FLIGHT)
    {
        _handle_state_free_flight();
    }
    else if (s_State == FLIGHT_STATE_FREE_FALL)
    {
        _handle_state_free_fall();
    }
    else if (s_State == FLIGHT_STATE_LANDED)
    {
        _handle_state_landed();
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

bool sm_is_armed(void)
{
    return s_Armed;
}

static void _handle_radio_packet(void)
{
    if (events_poll(MSG_RADIO_PACKET_RECEIVED))
    {
        const datalink_frame_structure_serial_t *msg = radio_get_current_message();

        if (msg && msg->msgId == DATALINK_MESSAGE_TELEMETRY_RESPONSE)
        {
            const datalink_frame_telemetry_response_t *payload = (const datalink_frame_telemetry_response_t *)msg->payload;
            bool arm = payload->controlFlags & DATALINK_FLAGS_TELEMETRY_RESPONSE_CONTROL_ARM_ENABLED;

            if (arm && !s_Armed)
            {
                s_Armed = true;

                SERIAL_DEBUG_PRINTF("Armed igniters!");
            }
            else if (!arm && s_Armed)
            {
                s_Armed = false;

                SERIAL_DEBUG_PRINTF("Igniters were disarmed!");
            }
        }
    }
}

static void _handle_state_standing(void)
{
    _handle_radio_packet();

    if (!s_Armed)
    {
        return;
    }

    if (events_poll(MSG_SENSORS_NORMAL_READ) && !s_VerifingStandingAlt)
    {
        vec3_t accel = sensors_get_frame()->acc1;
        float acc_mag = vec3_mag(&accel);

        if (acc_mag >= START_ACC_THRESHOLD)
        {
            s_VerifingStandingAlt = true;

            SERIAL_DEBUG_PRINTF("Started verifing altitude");
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
            SERIAL_DEBUG_PRINTF("TEST: %f", alt - s_BaseAlt);

            if (alt - s_BaseAlt >= START_ALT_THRESHOLD)
            {
                s_State = FLIGHT_STATE_ACCELERATING;

                SERIAL_DEBUG_PRINTF("State: Accelerating");
            }
            else
            {
                s_StandingAltVerificationIndex++;

                if (s_StandingAltVerificationIndex == START_ALT_VERIFICATION_COUNT)
                {
                    SERIAL_DEBUG_PRINTF("Acceleration state verification unsuccessfull");

                    s_BaseAlt = 0;
                    s_VerifingStandingAlt = false;
                    s_StandingAltVerificationIndex = 0;
                }
            }
        }
    }
}

static void _handle_state_accelerating(void)
{
    if (events_poll(MSG_SENSORS_NORMAL_READ))
    {
        vec3_t accel = sensors_get_frame()->acc1;
        float acc_mag = vec3_mag(&accel);

        if (acc_mag < EARTH_GRAVITY)
        {
            s_State = FLIGHT_STATE_FREE_FLIGHT;

            SERIAL_DEBUG_PRINTF("State: Free Flight");
        }
    }
}

static void _handle_state_free_flight(void)
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

                SERIAL_DEBUG_PRINTF("Apogee reached: %f", s_Apogee);
                SERIAL_DEBUG_PRINTF("State: Free Fall");

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

static void _handle_state_free_fall(void)
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

                SERIAL_DEBUG_PRINTF("State: Landed");
            }
        }
    }
}

static void _handle_state_landed(void)
{
}