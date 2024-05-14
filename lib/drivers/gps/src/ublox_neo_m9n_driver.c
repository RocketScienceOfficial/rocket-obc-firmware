#include "lib/drivers/gps/ublox_neo_m9n_driver.h"
#include <string.h>

void ublox_neo_m9n_init_spi(ublox_neo_m9n_config_t *config, hal_spi_instance_t spi, hal_pin_number_t cs)
{
    config->gpioConfig = (hal_gpio_communication_config_t){
        .protocol = GPIO_PROTOCOL_SPI,
        .spi = spi,
        .cs = cs,
    };

    hal_spi_init_cs(spi, cs);
}

void ublox_neo_m9n_read_data(ublox_neo_m9n_config_t *config, gps_nmea_sentence_t *sentence)
{
    uint8_t b = hal_gpio_single_read(&config->gpioConfig);

    if (b == 0xFF) // ????
    {
        return;
    }

    if (b == '$')
    {
        sentence->currentIndex = 0;
        sentence->isFinished = false;

        memset(&sentence->data, 0, sizeof(sentence->data));
    }
    else if (b == '\n')
    {
        sentence->data[sentence->currentIndex] = b;
        sentence->data[sentence->currentIndex + 1] = '\0';
        sentence->isFinished = true;

        return;
    }

    sentence->data[sentence->currentIndex++] = b;
}