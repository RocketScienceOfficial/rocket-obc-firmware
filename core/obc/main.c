#include <stdbool.h>

#include "pinout.h"
#include "measurements.h"

#include "modules/drivers/hal/board_control.h"
#include "modules/drivers/hal/time_tracker.h"
#include "modules/drivers/hal/multicore.h"
#include "modules/drivers/hal/spi_driver.h"
#include "modules/drivers/hal/i2c_driver.h"
#include "modules/drivers/hal/adc_driver.h"
#include "modules/flight_sm/flight_sm_control.h"
#include "modules/ign/ign_controller.h"
#include "modules/database/dataman.h"
#include "modules/logger/logger.h"

int main()
{
    hal_board_init(BOARD_STARTUP_TIME_MS);

    OBC_INFO("Initialized board!");

    hal_spi_init_all(SPI, SPI_FREQ);
    hal_i2c_init_all(I2C, I2C_FREQ);
    hal_adc_init_all();

    dataman_sensor_frame_t datamanFrame = {0};
    dataman_clear();

    flight_sm_data_t sm = {0};
    flight_sm_input_t smInput = {0};
    flight_sm_init(&sm);

    ign_data_t ignData = {0};
    ign_pins_t ignPins = {
        .main = IGN_1,
        .drouge = IGN_2,
        .sep = IGN_3,
        .second = IGN_4,
    };
    ign_settings_t ignSettings = {
        .mainAlt = 500,
        .secondIgnDelay = 1000,
    };
    ign_init(&ignData, &ignPins, &ignSettings);
    ign_arm(&ignData);

    measurement_data_t meas = {0};
    measurements_init();

    OBC_INFO("Initialized systems!");

    usec_t measurementTimeOffset;

    while (true)
    {
        if (hal_time_run_every_us(MEASUREMENTS_DELAY_US, &measurementTimeOffset))
        {
            measurements_get(&meas);

            datamanFrame = (dataman_sensor_frame_t){
                .acc1 = meas.acc1,
                .acc2 = meas.acc2,
                .acc3 = meas.acc3,
                .gyro1 = meas.gyro1,
                .gyro2 = meas.gyro2,
                .mag = meas.mag,
                .press = meas.press,
                .temp = meas.temp,
                .lat = meas.lat,
                .lon = meas.lon,
                .alt = meas.alt,
            };

            smInput = (flight_sm_input_t){
                .accel = meas.acc1,
                .alt = meas.alt,
            };

            dataman_save_sensors_frame(&datamanFrame);
        }

        flight_sm_update(&sm, &smInput);
        ign_update(&ignData, &sm);
    }

    dataman_flush();

    return 0;
}