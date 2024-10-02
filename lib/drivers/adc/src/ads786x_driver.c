#include "lib/drivers/adc/ads786x_driver.h"
#include "hal/time_tracker.h"

void ads786x_init(ads786x_config_t *config, hal_spi_instance_t spi, hal_pin_number_t cs, ads786x_type_t type, float vRef)
{
    config->spi = spi;
    config->cs = cs;
    config->adcBits = type == ADS786X_TYPE_6   ? 12
                      : type == ADS786X_TYPE_7 ? 10
                                               : 8;
    config->vRef = vRef;

    hal_spi_init_cs(cs);
}

float ads786x_read(const ads786x_config_t *config)
{
    hal_spi_cs_select(config->cs);

    hal_time_sleep_us(25);

    uint8_t buffer[2];
    hal_spi_read(config->spi, 0, buffer, sizeof(buffer));

    hal_spi_cs_deselect(config->cs);

    uint16_t data = ((buffer[0] << 8) | buffer[1]) >> (12 - config->adcBits);
    float result = config->vRef * (float)data / (1 << config->adcBits);

    return result;
}