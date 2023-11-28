#include <stdbool.h>

#include "pinout.h"
#include "measurements.h"

#include "modules/flight_sm/flight_sm_control.h"
#include "modules/drivers/hal/board_control.h"
#include "modules/drivers/hal/time_tracker.h"
#include "modules/drivers/hal/multicore.h"
#include "modules/drivers/hal/spi_driver.h"
#include "modules/drivers/hal/i2c_driver.h"
#include "modules/drivers/hal/adc_driver.h"

static usec_t s_MeasurementsTimeOffset;

int main()
{
    hal_board_init(BOARD_STARTUP_TIME_MS);

    hal_spi_init_all(SPI, SPI_FREQ);
    hal_i2c_init_all(I2C, I2C_FREQ);
    hal_adc_init_all();

    measurements_init();

    while (true)
    {
        if (hal_time_run_every_us(MEASUREMENTS_DELAY_US, &s_MeasurementsTimeOffset))
        {
            measurements_get(NULL);
        }
    }

    return 0;
}