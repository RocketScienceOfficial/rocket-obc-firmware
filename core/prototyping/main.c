#include "modules/drivers/hal/board_control.h"
#include "modules/drivers/hal/multicore.h"
#include "modules/drivers/hal/spi_driver.h"
#include "modules/drivers/hal/i2c_driver.h"
#include "modules/drivers/hal/adc_driver.h"
#include "modules/drivers/magnetometer/mmc5983ma_driver.h"
#include "modules/logger/logger.h"

#define BOARD_STARTUP_TIME_MS 5000
#define SPI_INST 0
#define SPI_FREQ 1 * 1000 * 1000
#define I2C_INST 1
#define I2C_FREQ 2 * 1000 * 100
#define SDA 2
#define SCL 3

static mmc5983ma_config_t s_MMC5983MAConfig;

int main()
{
    hal_board_init(BOARD_STARTUP_TIME_MS);

    OBC_INFO("Initialized board!");
    OBC_INFO("Hardware version: 1.0");
    OBC_INFO("Author: Filip Gawlik");

    hal_spi_init_all(SPI_INST, SPI_FREQ);
    hal_i2c_init_all(I2C_INST, I2C_FREQ);
    hal_adc_init_all();

    mmc5983ma_init_i2c(&s_MMC5983MAConfig, I2C_INST, SDA, SCL);
    mmc5983ma_set_continuous_mode_odr(&s_MMC5983MAConfig, MMC5983MA_ODR_1000HZ);

    while (true)
    {
        vec3_t v;
        mmc5983ma_read(&s_MMC5983MAConfig, &v);

        OBC_INFO("Mag: %f %f %f", v.x, v.y, v.z);

        hal_time_sleep_ms(50);
    }
}