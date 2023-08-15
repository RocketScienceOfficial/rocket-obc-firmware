#include "drivers/gps/u_blox_sam_m10q_driver.h"
#include "drivers/gps/nmea_parser.h"
#include "drivers/tools/board_control.h"
#include "drivers/tools/time_tracker.h"
#include <stdio.h>

#define I2C 1
#define SDA 14
#define SCL 13

int main()
{
    boardInit(5000);

    i2cInitAll(I2C, 400000);

    UBloxSamM10QConfig config = {0};
    UBloxSamM10QData data = {0};
    uBloxSamM10QInitI2C(&config, I2C, SDA, SCL);

    while (TRUE)
    {
        uBloxSamM10QReadData(&config, &data);

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

        sleepMiliseconds(1);
    }

    return 0;
}