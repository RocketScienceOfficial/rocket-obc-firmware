#include "lib/drivers/gps/gps_driver.h"
#include "lib/drivers/gps/ubx.h"
#include "lib/geo/physical_constants.h"
#include <string.h>

#define GPS_COOLDOWN_MS 10

static uint8_t GPS_RATE_CONFIG[] = {0x28, 0x00, 0x01, 0x00, 0x01, 0x00};
static uint8_t GPS_ENABLE_GBS_CONFIG[] = {0xf0, 0x09, 0x01};
static uint8_t GPS_ENABLE_GNS_CONFIG[] = {0xf0, 0x0d, 0x01};
static uint8_t GPS_ENABLE_GST_CONFIG[] = {0xf0, 0x07, 0x01};
static uint8_t GPS_ENABLE_ZDA_CONFIG[] = {0xf0, 0x08, 0x01};
static uint8_t GPS_DYNAMIC_MODEL_CONFIG[] = {0x00, 0x01, 0x00, 0x00, 0x21, 0x00, 0x11, 0x20, 0x08};

static void _write_config(gps_config_t *config, uint8_t class, uint8_t id, const uint8_t *cfg, uint16_t cfgLen);
static void _parse_sentence(gps_config_t *config);
static void _reset_sentence(gps_config_t *config);
static float _get_lat_sign(char ns);
static float _get_lon_sign(char ew);

void gps_init_spi(gps_config_t *config, hal_spi_instance_t spi, hal_pin_number_t cs)
{
    config->gpioConfig = (gpio_utils_communication_config_t){
        .useSPI = true,
        .spi = spi,
        .cs = cs,
    };

    memset(&config->sentence, 0, sizeof(config->sentence));
    memset(&config->data, 0, sizeof(config->data));

    config->timeOffset = 0;

    hal_spi_init_cs(cs);

    _write_config(config, 0x06, 0x08, GPS_RATE_CONFIG, sizeof(GPS_RATE_CONFIG));
    _write_config(config, 0x06, 0x01, GPS_ENABLE_GBS_CONFIG, sizeof(GPS_ENABLE_GBS_CONFIG));
    _write_config(config, 0x06, 0x01, GPS_ENABLE_GNS_CONFIG, sizeof(GPS_ENABLE_GNS_CONFIG));
    _write_config(config, 0x06, 0x01, GPS_ENABLE_GST_CONFIG, sizeof(GPS_ENABLE_GST_CONFIG));
    _write_config(config, 0x06, 0x01, GPS_ENABLE_ZDA_CONFIG, sizeof(GPS_ENABLE_ZDA_CONFIG));
    _write_config(config, 0x06, 0x8a, GPS_DYNAMIC_MODEL_CONFIG, sizeof(GPS_DYNAMIC_MODEL_CONFIG));
}

bool gps_read(gps_config_t *config)
{
    if (hal_time_get_ms_since_boot() - config->timeOffset < GPS_COOLDOWN_MS)
    {
        return false;
    }

    uint8_t b = gpio_utils_single_read(&config->gpioConfig);

    if (b == 0xFF)
    {
        if (config->sentence.currentIndex > 0)
        {
            _reset_sentence(config);
        }

        config->timeOffset = hal_time_get_ms_since_boot();

        return false;
    }

    if (b == '\n')
    {
        config->sentence.data[config->sentence.currentIndex] = b;
        config->sentence.data[config->sentence.currentIndex + 1] = '\0';
        config->sentence.isFinished = true;

        _parse_sentence(config);

        return true;
    }
    else
    {
        if (b == '$')
        {
            _reset_sentence(config);
        }

        config->sentence.data[config->sentence.currentIndex++] = b;

        return false;
    }
}

static void _write_config(gps_config_t *config, uint8_t class, uint8_t id, const uint8_t *cfg, uint16_t cfgLen)
{
    uint8_t buffer[128];
    size_t len = ubx_create_frame(buffer, class, id, cfg, cfgLen);

    hal_spi_cs_select(config->gpioConfig.cs);
    hal_spi_write(config->gpioConfig.spi, buffer, len);
    hal_spi_cs_deselect(config->gpioConfig.cs);
}

static void _parse_sentence(gps_config_t *config)
{
    if (config->sentence.isFinished && nmea_check_sentence(config->sentence.data))
    {
        nmea_sentence_id_t s = nmea_get_sentence_id(config->sentence.data);

        if (s == NMEA_SENTENCE_GBS)
        {
            nmea_sentence_gbs_t frame = {0};
            nmea_parse_gbs(config->sentence.data, &frame);

            config->data.hour = frame.time.hour;
            config->data.minute = frame.time.minute;
            config->data.second = frame.time.second;
            config->data.hundredth = frame.time.hundredth;
        }
        else if (s == NMEA_SENTENCE_GGA)
        {
            nmea_sentence_gga_t frame = {0};
            nmea_parse_gga(config->sentence.data, &frame);

            config->data.hour = frame.time.hour;
            config->data.minute = frame.time.minute;
            config->data.second = frame.time.second;
            config->data.hundredth = frame.time.hundredth;
            config->data.position.lat = frame.lat * _get_lat_sign(frame.NS);
            config->data.position.lon = frame.lon * _get_lon_sign(frame.EW);
            config->data.position.alt = frame.alt;
            config->data.hdop = frame.HDOP;
        }
        else if (s == NMEA_SENTENCE_GLL)
        {
            nmea_sentence_gll_t frame = {0};
            nmea_parse_gll(config->sentence.data, &frame);

            config->data.position.lat = frame.lat * _get_lat_sign(frame.NS);
            config->data.position.lon = frame.lon * _get_lon_sign(frame.EW);
            config->data.hour = frame.time.hour;
            config->data.minute = frame.time.minute;
            config->data.second = frame.time.second;
            config->data.hundredth = frame.time.hundredth;
        }
        else if (s == NMEA_SENTENCE_GNS)
        {
            nmea_sentence_gns_t frame = {0};
            nmea_parse_gns(config->sentence.data, &frame);

            config->data.hour = frame.time.hour;
            config->data.minute = frame.time.minute;
            config->data.second = frame.time.second;
            config->data.hundredth = frame.time.hundredth;
            config->data.position.lat = frame.lat * _get_lat_sign(frame.NS);
            config->data.position.lon = frame.lon * _get_lon_sign(frame.EW);
            config->data.position.alt = frame.alt;
            config->data.hdop = frame.HDOP;
            config->data.activeSatellitesCount = frame.numSV;
            config->data._activeSatellitesIndex = 0;
        }
        else if (s == NMEA_SENTENCE_GSA)
        {
            nmea_sentence_gsa_t frame = {0};
            nmea_parse_gsa(config->sentence.data, &frame);

            config->data.fix = frame.navMode == 2 || frame.navMode == 3;
            config->data.is3dFix = frame.navMode == 3;
            config->data.hdop = frame.HDOP;
            config->data.pdop = frame.PDOP;
            config->data.vdop = frame.VDOP;

            for (int i = 0; i < 12; i++)
            {
                if (frame.svid[i] == 0)
                {
                    break;
                }

                config->data.activeSatellitesIds[config->data._activeSatellitesIndex++] = frame.svid[i];
            }
        }
        else if (s == NMEA_SENTENCE_GST)
        {
            nmea_sentence_gst_t frame = {0};
            nmea_parse_gst(config->sentence.data, &frame);

            config->data.hour = frame.time.hour;
            config->data.minute = frame.time.minute;
            config->data.second = frame.time.second;
            config->data.hundredth = frame.time.hundredth;
        }
        else if (s == NMEA_SENTENCE_RMC)
        {
            nmea_sentence_rmc_t frame = {0};
            nmea_parse_rmc(config->sentence.data, &frame);

            config->data.hour = frame.time.hour;
            config->data.minute = frame.time.minute;
            config->data.second = frame.time.second;
            config->data.hundredth = frame.time.hundredth;
            config->data.day = frame.date.day;
            config->data.month = frame.date.month;
            config->data.year = frame.date.year;
            config->data.position.lat = frame.lat * _get_lat_sign(frame.NS);
            config->data.position.lon = frame.lon * _get_lon_sign(frame.EW);
            config->data.speed = KNOTS_TO_KMH(frame.spd);
            config->data.course = frame.cog;
        }
        else if (s == NMEA_SENTENCE_VTG)
        {
            nmea_sentence_vtg_t frame = {0};
            nmea_parse_vtg(config->sentence.data, &frame);

            config->data.course = frame.cogt;
            config->data.speed = frame.sogk;
        }
        else if (s == NMEA_SENTENCE_ZDA)
        {
            nmea_sentence_zda_t frame = {0};
            nmea_parse_zda(config->sentence.data, &frame);

            config->data.hour = frame.time.hour;
            config->data.minute = frame.time.minute;
            config->data.second = frame.time.second;
            config->data.hundredth = frame.time.hundredth;
            config->data.day = frame.day;
            config->data.month = frame.month;
            config->data.year = frame.year - 2000;
        }
    }
}

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