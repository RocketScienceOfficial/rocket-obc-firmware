#include "mission_control.h"
#include "measurements.h"
#include "config.h"
#include "drivers/tools/time_tracker.h"

static BOOL s_GPSInitialized;
static TIME s_MissionStartTime;

VOID updateMission(VOID)
{
    if (checkGPS() && !s_GPSInitialized)
    {
        s_GPSInitialized = TRUE;
        s_MissionStartTime = getMsSinceBoot();
    }
}

BOOL isMissionReady(VOID)
{
    return s_GPSInitialized;
}

BOOL isMissionDone(VOID)
{
    return getMsSinceBoot() - s_MissionStartTime > MISSION_TIME_MS;
}