#include "modules/nmea/nmea_wrapper.h"
#include "modules/nmea/nmea_parser.h"

bool gnss_update(geo_position_wgs84_t *data, gps_nmea_sentence_t *sentence)
{
    if (!sentence->isFinished)
    {
        return false;
    }

    if (nmea_check_sentence(sentence->data))
    {
        nmea_sentence_id_t s = nmea_get_sentence_id(sentence->data);

        if (s == NMEA_SENTENCE_GGA)
        {
            nmea_sentence_gga_t frame = {0};
            nmea_parse_gga(sentence->data, &frame);

            data->lat = frame.lat;
            data->lon = frame.lon;
            data->alt = frame.alt;

            return true;
        }
        else if (s == NMEA_SENTENCE_GLL)
        {
            nmea_sentence_gll_t frame = {0};
            nmea_parse_gll(sentence->data, &frame);

            data->lat = frame.lat;
            data->lon = frame.lon;

            return true;
        }
        else if (s == NMEA_SENTENCE_GNS)
        {
            nmea_sentence_gns_t frame = {0};
            nmea_parse_gns(sentence->data, &frame);

            data->lat = frame.lat;
            data->lon = frame.lon;
            data->alt = frame.alt;

            return true;
        }
    }

    return false;
}