#include "mission_control.h"
#include "measurements.h"

static BOOL s_GPSInitialized;

BOOL isMissionReady(VOID)
{
    return TRUE;
    
    if (checkGPS() && !s_GPSInitialized)
    {
        s_GPSInitialized = TRUE;
    }

    if (!s_GPSInitialized)
    {
        return FALSE;
    }

    return TRUE;
}

BOOL isMissionDone(VOID)
{
    return FALSE;
}