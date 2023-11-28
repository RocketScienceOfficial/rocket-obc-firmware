#include "modules/drivers/gps/ublox_neo_m9n_driver.h"
#include <string.h>

void ublox_neo_m9n_init_spi(ublox_neo_m9n_config_t *config, hal_spi_instance_t spi, hal_pin_number_t miso, hal_pin_number_t mosi, hal_pin_number_t cs, hal_pin_number_t sck)
{
    config->gpioConfig = (hal_gpio_communication_config_t){
        .protocol = GPIO_PROTOCOL_SPI,
        .spi = spi,
        .cs = cs,
    };

    hal_spi_init_pins(spi, miso, mosi, sck, cs);
}

void ublox_neo_m9n_read_data(ublox_neo_m9n_config_t *config, ublox_neo_m9n_data_t *data)
{
    uint8_t b = hal_gpio_single_read(&config->gpioConfig);

    if (b == 0xFF)
    {
        return;
    }

    if (b == '$')
    {
        data->currentIndex = 0;
        data->sentence.isFinished = false;

        memset(data->sentence.data, 0, sizeof(data->sentence.data));
    }
    else if (b == '\n')
    {
        data->sentence.data[data->currentIndex] = b;
        data->sentence.data[data->currentIndex + 1] = '\0';
        data->sentence.isFinished = true;

        return;
    }

    data->sentence.data[data->currentIndex++] = b;
}