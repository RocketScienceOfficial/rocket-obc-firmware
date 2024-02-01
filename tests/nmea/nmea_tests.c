#include "tests/test_framework.h"
#include "modules/nmea/nmea_parser.h"
#include "modules/nmea/nmea_wrapper.h"
#include "modules/drivers/gps/gps_defines.h"

int main()
{
    TEST_START();

    TEST_START_SECTION("GGA Sentence");
    {
        const char *sentence = "$GPGGA,092725.00,4717.11399,N,00833.91590,E,1,08,1.01,499.6,M,48.0,M,,*5B\r\n";

        TEST_ASSERT(nmea_check_sentence(sentence));
        TEST_ASSERT(nmea_get_sentence_id(sentence) == NMEA_SENTENCE_GGA);
        TEST_ASSERT(nmea_get_talker_id(sentence) == NMEA_TALKER_GPS_SBAS);

        nmea_sentence_gga_t frame = {0};

        TEST_ASSERT(nmea_parse_gga(sentence, &frame));
        TEST_ASSERT(frame.time.hour == 9);
        TEST_ASSERT(frame.time.minute == 27);
        TEST_ASSERT(frame.time.second == 25);
        TEST_ASSERT(frame.time.hundredth == 0);
        TEST_ASSERT(frame.lat == 47.28523316666666432);
        TEST_ASSERT(frame.lon == 8.565265);
        TEST_ASSERT(frame.NS == 'N');
        TEST_ASSERT(frame.EW == 'E');
        TEST_ASSERT(frame.quality == 1);
        TEST_ASSERT(frame.numSV == 8);
        TEST_ASSERT(frame.HDOP == 1.01f);
        TEST_ASSERT(frame.alt == 499.6f);
        TEST_ASSERT(frame.altUnit == 'M');
        TEST_ASSERT(frame.sep == 48.0f);
        TEST_ASSERT(frame.sepUnit == 'M');
        TEST_ASSERT(frame.diffAge == 0);
        TEST_ASSERT(frame.diffStation == 0);
    }
    TEST_END_SECTION();

    TEST_START_SECTION("GGA Sentence, Invalid checksum");
    {
        const char *sentence = "$GPGGA,092725.00,4717.11399,N,00833.91590,E,1,08,1.01,499.6,M,48.0,M,,*5F\r\n";

        TEST_ASSERT(!nmea_check_sentence(sentence));
    }
    TEST_END_SECTION();

    TEST_START_SECTION("GGA Sentence, Unknown sentence & talker");
    {
        const char *sentence = "$GRGYU,092725.00,4717.11399,N,00833.91590,E,1,08,1.01,499.6,M,48.0,M,,*5B\r\n";

        TEST_ASSERT(!nmea_check_sentence(sentence));
    }
    TEST_END_SECTION();

    TEST_START_SECTION("NMEA Wrapper");
    {
        gps_nmea_sentence_t data = {
            .data = "$GPGGA,092725.00,4717.11399,N,00833.91590,E,1,08,1.01,499.6,M,48.0,M,,*5B\r\n",
            .isFinished = true,
        };
        gnss_data_t gnss = {0};

        TEST_ASSERT(gnss_update(&gnss, &data));
        TEST_ASSERT(gnss.lat == 47.28523316666666432);
        TEST_ASSERT(gnss.lon == 8.565265);
        TEST_ASSERT(gnss.alt == 499.6f);
    }
    TEST_END_SECTION();

    TEST_END();
}