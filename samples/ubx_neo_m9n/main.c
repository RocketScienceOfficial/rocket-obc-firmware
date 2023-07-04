#include "drivers/gps/u_blox_neo_m9n_driver.h"
#include "drivers/gps/nmea_parser.h"
#include "drivers/tools/board_control.h"
#include "drivers/tools/time_tracker.h"
#include <stdio.h>

#define SPI 0
#define SCK 18
#define MOSI 19
#define MISO 16
#define CS 13

int main()
{
    boardInit(5000);

    spiInitAll(SPI, 1 * 1000 * 1000);

    UBloxNeoM9NConfig config = {0};
    UBloxNeoM9NData data = {0};
    uBloxNeoM9NInitSPI(&config, SPI, MISO, MOSI, CS, SCK);

    while (TRUE)
    {
        uBloxNeoM9NReadData(&config, &data);

        if (data.isFinishedSentence)
        {
            BOOL valid = nmeaCheckSentence(data.sentence);

            printf("Valid: %d\n", valid);

            if (valid)
            {
                NMEASentence s = nmeaGetSentenceId(data.sentence);

                printf("Sentence: %d\n", s);
                printf("Sentence: %s", data.sentence);

                if (s == NMEA_SENTENCE_GGA)
                {
                    NMEASentence_GGA frame = {0};
                    nmeaParse_GGA(data.sentence, &frame);

                    printf("Latitude: %f\n", frame.lat);
                    printf("Longtitude: %f\n", frame.lon);
                }
                else if (s == NMEA_SENTENCE_GLL)
                {
                    NMEASentence_GLL frame = {0};
                    nmeaParse_GLL(data.sentence, &frame);

                    printf("Latitude: %f\n", frame.lat);
                    printf("Longtitude: %f\n", frame.lon);
                }
                else if (s == NMEA_SENTENCE_GNS)
                {
                    NMEASentence_GNS frame = {0};
                    nmeaParse_GNS(data.sentence, &frame);
                    
                    printf("Latitude: %f\n", frame.lat);
                    printf("Longtitude: %f\n", frame.lon);
                }
            }
        }

        sleepMiliseconds(10);
    }

    return 0;
}