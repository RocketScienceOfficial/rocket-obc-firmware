#include "lib/drivers/gps/gps_driver.h"
#include <string.h>

static void _reset_sentence(gps_config_t *config)
{
    config->sentence.currentIndex = 0;
    config->sentence.isFinished = false;

    memset(config->sentence.data, 0, sizeof(config->sentence.data));
}

static float _get_lat_sign(char ns)
{
    return ns == 'N' ? 1 : -1;
}

static float _get_lon_sign(char ew)
{
    return ew == 'E' ? 1 : -1;
}

void gps_init_spi(gps_config_t *config, hal_spi_instance_t spi, hal_pin_number_t cs)
{
    config->gpioConfig = (gpio_utils_communication_config_t){
        .useSPI = true,
        .spi = spi,
        .cs = cs,
    };

    hal_spi_init_cs(cs);

    const uint8_t configBuffer[] = {0xb5, 0x62, 0x06, 0x08, 0x06, 0x00, 0x28, 0x00, 0x01, 0x00, 0x01, 0x00, 0x3e, 0xaa};

    hal_spi_cs_select(cs);
    hal_spi_write(spi, configBuffer, sizeof(configBuffer));
    hal_spi_cs_deselect(cs);

    const uint8_t configBuffer2[] = {0xb5, 0x62, 0x06, 0x8a, 0x09, 0x00, 0x00, 0x01, 0x00, 0x00, 0x21, 0x00, 0x11, 0x20, 0x08, 0xf4, 0x51};

    hal_spi_cs_select(cs);
    hal_spi_write(spi, configBuffer2, sizeof(configBuffer2));
    hal_spi_cs_deselect(cs);
}

bool gps_read(gps_config_t *config)
{
    uint8_t b = gpio_utils_single_read(&config->gpioConfig);

    if (b == 0xFF)
    {
        if (config->sentence.currentIndex > 0)
        {
            _reset_sentence(config);
        }

        return false;
    }

    if (b == '\n')
    {
        config->sentence.data[config->sentence.currentIndex] = b;
        config->sentence.data[config->sentence.currentIndex + 1] = '\0';
        config->sentence.isFinished = true;
    }
    else
    {
        if (b == '$')
        {
            _reset_sentence(config);
        }

        config->sentence.data[config->sentence.currentIndex++] = b;
    }

    return true;
}

bool gps_get_pos(const gps_config_t *config, geo_position_wgs84_t *pos)
{
    if (config->sentence.isFinished && nmea_check_sentence(config->sentence.data))
    {
        nmea_sentence_id_t s = nmea_get_sentence_id(config->sentence.data);

        if (s == NMEA_SENTENCE_GGA)
        {
            nmea_sentence_gga_t frame = {0};
            nmea_parse_gga(config->sentence.data, &frame);

            pos->lat = frame.lat * _get_lat_sign(frame.NS);
            pos->lon = frame.lon * _get_lon_sign(frame.EW);
            pos->alt = frame.alt;

            return true;
        }
        else if (s == NMEA_SENTENCE_GLL)
        {
            nmea_sentence_gll_t frame = {0};
            nmea_parse_gll(config->sentence.data, &frame);

            pos->lat = frame.lat * _get_lat_sign(frame.NS);
            pos->lon = frame.lon * _get_lon_sign(frame.EW);

            return true;
        }
        else if (s == NMEA_SENTENCE_GNS)
        {
            nmea_sentence_gns_t frame = {0};
            nmea_parse_gns(config->sentence.data, &frame);

            pos->lat = frame.lat * _get_lat_sign(frame.NS);
            pos->lon = frame.lon * _get_lon_sign(frame.EW);
            pos->alt = frame.alt;

            return true;
        }
        else if (s == NMEA_SENTENCE_RMC)
        {
            nmea_sentence_rmc_t frame = {0};
            nmea_parse_rmc(config->sentence.data, &frame);

            pos->lat = frame.lat * _get_lat_sign(frame.NS);
            pos->lon = frame.lon * _get_lon_sign(frame.EW);

            return true;
        }
    }

    return false;
}