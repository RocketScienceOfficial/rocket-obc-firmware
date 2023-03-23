#include "drivers/gps/nmea_parser.h"

BOOL nmeaCheckSentence(const BYTE *sentence, SIZE length)
{
}

BOOL nmeaScan(const BYTE *sentence, SIZE length, const CHAR *format, ...)
{
}

NMEASentenceId nmeaGetSentenceId(const BYTE *sentence, SIZE length)
{
}

NMEATalker nmeaGetTalkerId(const BYTE *sentence, SIZE length)
{
}

BOOL nmeaParse_GBS(const BYTE *sentence, SIZE length, NMEASentence_GBS *gbs)
{
}

BOOL nmeaParse_GGA(const BYTE *sentence, SIZE length, NMEASentence_GGA *gga)
{
}

BOOL nmeaParse_GLL(const BYTE *sentence, SIZE length, NMEASentence_GLL *gll)
{
}

BOOL nmeaParse_GNS(const BYTE *sentence, SIZE length, NMEASentence_GNS *gns)
{
}

BOOL nmeaParse_GSA(const BYTE *sentence, SIZE length, NMEASentence_GSA *gsa)
{
}

BOOL nmeaParse_GST(const BYTE *sentence, SIZE length, NMEASentence_GST *gst)
{
}

BOOL nmeaParse_GSV(const BYTE *sentence, SIZE length, NMEASentence_GSV *gsv)
{
}

BOOL nmeaParse_RMC(const BYTE *sentence, SIZE length, NMEASentence_RMC *rmc)
{
}

BOOL nmeaParse_VTG(const BYTE *sentence, SIZE length, NMEASentence_VTG *vtg)
{
}

BOOL nmeaParse_ZDA(const BYTE *sentence, SIZE length, NMEASentence_ZDA *zda)
{
}

BYTE __nmeaCalculateChecksum(const BYTE *sentence, SIZE length)
{
    BYTE checksum = 0;

    for (SIZE i = 0; i < length; i++)
    {
        checksum ^= sentence[i];
    }

    return checksum;
}