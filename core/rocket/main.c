#include "config.h"
#include "radio_utils.h"
#include "measurements_utils.h"
#include "saver.h"
#include "commands.h"
#include "algorithms.h"

#include "drivers/tools/time_tracker.h"
#include "drivers/tools/board_control.h"

static RawMeasurementData s_CurrentRawMeasurement;
static MeasurementData s_LastNormalMeasurement;
static BOOL s_GPSInitialized;

static TIME s_MeasurementTimerOffset;
static TIME s_RadioTimerOffset;

int main(VOID)
{
    boardInit(0);

    initStorage();
    initRadio();
    initSensors();
    initAlgorithms();

    while (TRUE)
    {
        if (checkGPS() && !s_GPSInitialized)
        {
            s_GPSInitialized = TRUE;
        }

        if (!s_GPSInitialized)
        {
            continue;
        }

        if (runEveryUs(MEASUREMENTS_UPDATE_RATE_MS * 1000, &s_MeasurementTimerOffset))
        {
            takeMeasurements(&s_CurrentRawMeasurement);
            convertRawData(&s_CurrentRawMeasurement, &s_LastNormalMeasurement);
            saveData(&s_CurrentRawMeasurement);
        }

        if (runEveryUs(RADIO_SEND_RATE_MS * 1000, &s_RadioTimerOffset))
        {
            sendMeasurementData(&s_LastNormalMeasurement);
        }
    }

    return 0;
}