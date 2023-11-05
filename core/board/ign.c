#include "ign.h"
#include "config.h"
#include "mission_control.h"
#include "drivers/igniter/igniter_driver.h"

static IgniterData s_IgniterData;
static BOOL s_IgnitersFired;

VOID initIgn(VOID)
{
    s_IgniterData = (IgniterData){
        .pins = {IGNITER_1_PIN, IGNITER_2_PIN, IGNITER_3_PIN, IGNITER_4_PIN},
        .count = 4,
        .delay = IGNITER_DELAY_MS,
    };

    ignInit(&s_IgniterData);
}

VOID checkIgn(VOID)
{
    if (isMissionDone())
    {
        if (s_IgnitersFired)
        {
            ignFire(&s_IgniterData);

            s_IgnitersFired = TRUE;
        }
    }

    ignUpdate(&s_IgniterData);
}