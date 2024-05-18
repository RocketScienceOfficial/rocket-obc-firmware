#include "lib/drivers/gps/gps_driver.h"

void gps_init_spi(gps_config_t *config, hal_spi_instance_t spi, hal_pin_number_t cs)
{
    config->gpioConfig = (hal_gpio_communication_config_t){
        .protocol = GPIO_PROTOCOL_SPI,
        .spi = spi,
        .cs = cs,
    };

    hal_spi_init_cs(cs);
}

void gps_read(gps_config_t *config)
{
    uint8_t b = hal_gpio_single_read(&config->gpioConfig);

    if (b == 0xFF)
    {
        return;
    }

    if (b == '$')
    {
        config->sentence.currentIndex = 0;
        config->sentence.isFinished = false;

        memset(&config->sentence.data, 0, sizeof(config->sentence.data));
    }
    else if (b == '\n')
    {
        config->sentence.data[config->sentence.currentIndex] = b;
        config->sentence.data[config->sentence.currentIndex + 1] = '\0';
        config->sentence.isFinished = true;

        return;
    }

    config->sentence.data[config->sentence.currentIndex++] = b;
}

bool gps_get_pos(gps_config_t *config, geo_position_wgs84_t *pos)
{
    if (config->sentence.isFinished && nmea_check_sentence(config->sentence.data))
    {
        nmea_sentence_id_t s = nmea_get_sentence_id(config->sentence.data);

        if (s == NMEA_SENTENCE_GGA)
        {
            nmea_sentence_gga_t frame = {0};
            nmea_parse_gga(config->sentence.data, &frame);

            pos->lat = frame.lat;
            pos->lon = frame.lon;
            pos->alt = frame.alt;

            return true;
        }
        else if (s == NMEA_SENTENCE_GLL)
        {
            nmea_sentence_gll_t frame = {0};
            nmea_parse_gll(config->sentence.data, &frame);

            pos->lat = frame.lat;
            pos->lon = frame.lon;

            return true;
        }
        else if (s == NMEA_SENTENCE_GNS)
        {
            nmea_sentence_gns_t frame = {0};
            nmea_parse_gns(config->sentence.data, &frame);

            pos->lat = frame.lat;
            pos->lon = frame.lon;
            pos->alt = frame.alt;

            return true;
        }
    }

    return false;
}