#include "config.h"
#include "radio_utils.h"
#include "measurements.h"
#include "saver.h"
#include "commands.h"
#include "algorithms.h"
#include "driver_calling.h"
#include "mission_control.h"

#include "drivers/gpio/spi_driver.h"
#include "drivers/gpio/i2c_driver.h"
#include "drivers/gpio/adc_driver.h"
#include "drivers/tools/time_tracker.h"
#include "drivers/tools/board_control.h"

static RawMeasurementData s_CurrentRawMeasurement;
static MeasurementData s_LastNormalMeasurement;

static TIME s_MeasurementTimerOffset;
static TIME s_RadioTimerOffset;

int main(VOID)
{
    boardInit(0);

    checkCMD();

    DRIVER_CALL(spiInitAll(SPI, 1000000));
    DRIVER_CALL(i2cInitAll(I2C, 400000));
    DRIVER_CALL(adcInitAll());

    initStorage();
    initSensors();
    // initRadio();
    initAlgorithms();

    while (TRUE)
    {
        if (isMissionReady())
        {
            if (runEveryUs(MEASUREMENTS_UPDATE_RATE_MS * 1000, &s_MeasurementTimerOffset))
            {
                takeMeasurements(&s_CurrentRawMeasurement);
                convertRawData(&s_CurrentRawMeasurement, &s_LastNormalMeasurement);
                saveData(&s_CurrentRawMeasurement);
            }

            if (runEveryUs(RADIO_SEND_RATE_MS * 1000, &s_RadioTimerOffset))
            {
                // sendMeasurementData(&s_LastNormalMeasurement);
            }
        }
    }

    return 0;
}