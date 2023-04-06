#include "test_nmea.h"
#include "drivers/gps/nmea_parser.h"

MY_TEST_FUNC(NMEA_TEST_NAME, 1)
{
    STRING sentence = "$GPGGA,092725.00,4717.11399,N,00833.91590,E,1,08,1.01,499.6,M,48.0,M,,*5B\r\n";

    MY_ASSERT(nmeaCheckSentence(sentence));
    MY_ASSERT(nmeaGetSentenceId(sentence) == NMEA_SENTENCE_GGA);
    MY_ASSERT(nmeaGetTalkerId(sentence) == NMEA_TALKER_GPS_SBAS);

    NMEASentence_GGA frame = {0};

    MY_ASSERT(nmeaParse_GGA(sentence, &frame));
    MY_ASSERT(frame.time.hour == 9);
    MY_ASSERT(frame.time.minute == 27);
    MY_ASSERT(frame.time.second == 25);
    MY_ASSERT(frame.time.hundredth == 0);
    MY_ASSERT(frame.lat == 47.2852331667f);
    MY_ASSERT(frame.lon == 8.565265f);
    MY_ASSERT(frame.NS == 'N');
    MY_ASSERT(frame.EW == 'E');
    MY_ASSERT(frame.quality == 1);
    MY_ASSERT(frame.numSV == 8);
    MY_ASSERT(frame.HDOP == 1.01f);
    MY_ASSERT(frame.alt == 499.6f);
    MY_ASSERT(frame.altUnit == 'M');
    MY_ASSERT(frame.sep == 48.0f);
    MY_ASSERT(frame.sepUnit == 'M');
    MY_ASSERT(frame.diffAge == 0);
    MY_ASSERT(frame.diffStation == 0);

    MY_TEST_END();
}