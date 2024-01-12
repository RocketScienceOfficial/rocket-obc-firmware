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
#include "modules/database/params.h"
#include "modules/logger/logger.h"

static db_params_t s_Params;
static flight_sm_data_t s_SM;
static ign_data_t s_IGN;
static measurement_data_t s_MeasurementData;
static flight_sm_input_t s_SMInput;
static usec_t s_MeasurementTimeOffset;

static void _init(void);
static void _update(void);
static void _terminate(void);

static void _setup_board(void);
static void _setup_db(void);
static void _setup_sm(void);
static void _setup_ign(void);
static void _setup_meas(void);
static void _update_db(void);
static void _update_sm(void);

int main()
{
    _init();

    while (true)
    {
        _update();
    }

    _terminate();
}

static void _init(void)
{
    _setup_board();
    _setup_db();
    _setup_sm();
    _setup_ign();
    _setup_meas();

    OBC_INFO("Initialized systems!");
}

static void _update(void)
{
    if (hal_time_run_every_us(measurements_get_rate(&s_SM), &s_MeasurementTimeOffset))
    {
        measurements_get(&s_MeasurementData);

        _update_db();
        _update_sm();
    }

    ign_update(&s_IGN, &s_SM);
}

static void _terminate(void)
{
    dataman_flush();
}

static void _setup_board(void)
{
    hal_board_init(BOARD_STARTUP_TIME_MS);

    OBC_INFO("Initialized board!");
    OBC_INFO("Hardware version: 1.0");
    OBC_INFO("Author: Filip Gawlik");

    hal_spi_init_all(SPI_INST, SPI_FREQ);
    hal_i2c_init_all(I2C_INST, I2C_FREQ);
    hal_adc_init_all();
}

static void _setup_db(void)
{
    dataman_clear();

    if (!params_get(&s_Params))
    {
        s_Params.vbat = 3.3f;
        s_Params.ignMainAlt = 500;
        s_Params.ignSecondDelay = 1000;

        params_save(&s_Params);
    }
}

static void _setup_sm(void)
{
    flight_sm_init(&s_SM);
}

static void _setup_ign(void)
{
    ign_pins_t ignPins = {
        .main = IGN_1,
        .drouge = IGN_2,
        .sep = IGN_3,
        .second = IGN_4,
        .checkMain = MAX1161X_CHANNEL_AIN0,
        .checkDrouge = MAX1161X_CHANNEL_AIN1,
        .checkSep = MAX1161X_CHANNEL_AIN2,
        .checkSecond = MAX1161X_CHANNEL_AIN3,
    };
    ign_settings_t ignSettings = {
        .vbat = s_Params.vbat,
        .mainAlt = s_Params.ignMainAlt,
        .secondIgnDelay = s_Params.ignSecondDelay,
    };

    ign_init(&s_IGN, &ignPins, &ignSettings);
    ign_arm(&s_IGN);
}

static void _setup_meas(void)
{
    measurements_init();
}

static void _update_db(void)
{
    if (measurements_is_save(&s_SM))
    {
        dataman_save_sensors_frame((dataman_sensor_frame_t *)&s_MeasurementData);
    }
}

static void _update_sm(void)
{
    s_SMInput = (flight_sm_input_t){
        .accel = s_MeasurementData.acc1,
        .alt = s_MeasurementData.alt,
    };

    flight_sm_update(&s_SM, &s_SMInput);
}