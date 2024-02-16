#include <stdbool.h>

#include "config.h"
#include "measurements.h"
#include "commands.h"

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

static flight_sm_data_t s_SM;
static ign_data_t s_IGN;
static measurement_data_t s_MeasurementData;
static flight_sm_input_t s_SMInput;
static usec_t s_MeasurementTimeOffset;

static bool _init(void);
static bool _update(void);
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
    if (_init())
    {
        while (true)
        {
            if (!_update())
            {
                break;
            }
        }
    }

    _terminate();
}

static bool _init(void)
{
    _setup_board();
    _setup_sm();
    _setup_ign();
    _setup_meas();

    OBC_INFO("Initialized systems!");

    if (command_check())
    {
        _setup_db();

        return true;
    }
    else
    {
        return false;
    }
}

static bool _update(void)
{
    if (hal_time_run_every_us(measurements_get_rate(&s_SM), &s_MeasurementTimeOffset))
    {
        measurements_get(&s_MeasurementData);

        _update_db();
        _update_sm();
    }

    ign_update(&s_IGN, &s_SM);

    return true;
}

static void _terminate(void)
{
    dataman_flush();

    while (true)
    {
        hal_tight_loop();
    }
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

    hal_i2c_init_pins(I2C_INST, SDA, SCL);
}

static void _setup_db(void)
{
    dataman_clear();
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
        .mainAlt = MAIN_PARACHUTE_HEIGHT,
        .secondIgnDelay = IGN_SECOND_DELAY,
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