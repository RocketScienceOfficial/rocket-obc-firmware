#include "drivers/gps/nmea_parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>

BOOL nmeaCheckSentence(const STRING sentence)
{
    if (sentence[0] != '$')
    {
        return FALSE;
    }

    SIZE8 length = strlen(sentence);
    UINT8 checksum = 0;
    SIZE8 i = 0;

    for (i = 1; i < length; i++)
    {
        if (sentence[i] != '*')
        {
            checksum ^= sentence[i];
        }
        else
        {
            break;
        }
    }

    if (sentence[i] == '*')
    {
        UINT8 checksum1 = 0;
        UINT8 checksum2 = 0;

        if (i > length - 1 - 2)
        {
            return FALSE;
        }

        if (sentence[i + 1] >= '0' && sentence[i + 1] <= '9')
        {
            checksum1 = sentence[i + 1] - '0';
        }
        else if (sentence[i + 1] >= 'A' && sentence[i + 1] <= 'F')
        {
            checksum1 = sentence[i + 1] - 'A' + 10;
        }
        else
        {
            return FALSE;
        }

        if (sentence[i + 2] >= '0' && sentence[i + 2] <= '9')
        {
            checksum2 = sentence[i + 2] - '0';
        }
        else if (sentence[i + 2] >= 'A' && sentence[i + 2] <= 'F')
        {
            checksum2 = sentence[i + 2] - 'A' + 10;
        }
        else
        {
            return FALSE;
        }

        if (checksum != (checksum1 << 4 | checksum2))
        {
            return FALSE;
        }

        i += 2;
    }
    else
    {
        return FALSE;
    }

    if (i > length - 1 - 2)
    {
        return FALSE;
    }

    if (sentence[i + 1] != '\r' || sentence[i + 2] != '\n')
    {
        return FALSE;
    }

    i += 2;

    if (i != length - 1)
    {
        return FALSE;
    }

    return TRUE;
}

BOOL nmeaScan(const STRING sentence, const STRING format, ...)
{
    va_list ap;
    va_start(ap, format);

    CHAR field[32];
    SIZE8 fieldIndex = 0;
    SIZE8 index = 1;
    BOOL valid = TRUE;

    for (SIZE8 i = 0; format[i] != '\0'; i++)
    {
        memset(field, 0, sizeof(field));

        fieldIndex = 0;

        if (!valid)
        {
            break;
        }

        for (; sentence[index] != ',' && sentence[index] != '*'; index++)
        {
            field[fieldIndex] = sentence[index];
            fieldIndex++;
        }

        if (sentence[index] == '*')
        {
            valid = FALSE;
        }

        index++;

        switch (format[i])
        {
        case 'i':
        {
            INT32 *value = va_arg(ap, INT32 *);
            *value = field != NULL ? atoi(field) : 0;
            break;
        }
        case 'f':
        {
            FLOAT *value = va_arg(ap, FLOAT *);
            *value = field != NULL ? atof(field) : 0.0f;
            break;
        }
        case 'c':
        {
            CHAR *value = va_arg(ap, CHAR *);
            *value = field != NULL ? field[0] : '\0';
            break;
        }
        case 's':
        {
            STRING value = va_arg(ap, STRING);

            if (field != NULL)
            {
                memcpy(value, field, fieldIndex);
            }
            break;
        }
        case 'l':
        {
            NMEALatLon *value = va_arg(ap, NMEALatLon *);

            if (field != NULL)
            {
                DOUBLE raw = atof(field);
                DOUBLE raw2 = raw / 100.0;
                DOUBLE degs = floor(raw2);
                DOUBLE mins = (raw2 - degs) * 100.0;
                DOUBLE minsToDeg = mins / 60.0;

                *value = degs + minsToDeg;
            }
            else
            {
                *value = 0.0f;
            }
            break;
        }
        case 'd':
        {
            NMEADate *value = va_arg(ap, NMEADate *);

            if (field != NULL)
            {
                CHAR d[3] = {
                    field[0],
                    field[1],
                    '\0',
                };
                CHAR m[3] = {
                    field[2],
                    field[3],
                    '\0',
                };
                CHAR y[3] = {
                    field[4],
                    field[5],
                    '\0',
                };
                value->day = atoi(d);
                value->month = atoi(m);
                value->year = atoi(y);
            }
            else
            {
                value->day = 0;
                value->month = 0;
                value->year = 0;
            }
            break;
        }
        case 't':
        {
            NMEATime *value = va_arg(ap, NMEATime *);

            if (field != NULL)
            {
                CHAR h[3] = {
                    field[0],
                    field[1],
                    '\0',
                };
                CHAR m[3] = {
                    field[2],
                    field[3],
                    '\0',
                };
                CHAR s[3] = {
                    field[4],
                    field[5],
                    '\0',
                };
                CHAR hs[3] = {
                    field[7],
                    field[8],
                    '\0',
                };
                value->hour = atoi(h);
                value->minute = atoi(m);
                value->second = atoi(s);
                value->hundredth = atoi(hs);
            }
            else
            {
                value->hour = 0;
                value->minute = 0;
                value->second = 0;
                value->hundredth = 0;
            }
            break;
        }
        case '_':
        {
            break;
        }
        default:
        {
            return FALSE;
        }
        }
    }

    va_end(ap);

    return TRUE;
}

NMEASentence nmeaGetSentenceId(const STRING sentence)
{
    CHAR s[6];

    if (!nmeaScan(sentence, "s", s))
    {
        return NMEA_SENTENCE_UNKNOWN;
    }

    CHAR id[4] = {
        s[2],
        s[3],
        s[4],
        '\0',
    };

    if (strcmp(id, "GBS") == 0)
    {
        return NMEA_SENTENCE_GBS;
    }
    else if (strcmp(id, "GGA") == 0)
    {
        return NMEA_SENTENCE_GGA;
    }
    else if (strcmp(id, "GLL") == 0)
    {
        return NMEA_SENTENCE_GLL;
    }
    else if (strcmp(id, "GNS") == 0)
    {
        return NMEA_SENTENCE_GNS;
    }
    else if (strcmp(id, "GSA") == 0)
    {
        return NMEA_SENTENCE_GSA;
    }
    else if (strcmp(id, "GST") == 0)
    {
        return NMEA_SENTENCE_GST;
    }
    else if (strcmp(id, "GSV") == 0)
    {
        return NMEA_SENTENCE_GSV;
    }
    else if (strcmp(id, "RMC") == 0)
    {
        return NMEA_SENTENCE_RMC;
    }
    else if (strcmp(id, "VTG") == 0)
    {
        return NMEA_SENTENCE_VTG;
    }
    else if (strcmp(id, "ZDA") == 0)
    {
        return NMEA_SENTENCE_ZDA;
    }
    else
    {
        return NMEA_SENTENCE_UNKNOWN;
    }
}

NMEATalker nmeaGetTalkerId(const STRING sentence)
{
    CHAR s[6];

    if (!nmeaScan(sentence, "s", s))
    {
        return NMEA_TALKER_UNKNOWN;
    }

    CHAR id[3] = {
        s[0],
        s[1],
        '\0',
    };

    if (strcmp(id, "GP") == 0)
    {
        return NMEA_TALKER_GPS_SBAS;
    }
    else if (strcmp(id, "GL") == 0)
    {
        return NMEA_TALKER_GLONASS;
    }
    else if (strcmp(id, "GA") == 0)
    {
        return NMEA_TALKER_GALILEO;
    }
    else if (strcmp(id, "GB") == 0)
    {
        return NMEA_TALKER_BEIDOU;
    }
    else if (strcmp(id, "GQ") == 0)
    {
        return NMEA_TALKER_QZSS;
    }
    else if (strcmp(id, "GN") == 0)
    {
        return NMEA_TALKER_ANY;
    }
    else
    {
        return NMEA_TALKER_UNKNOWN;
    }
}

BOOL nmeaParse_GBS(const STRING sentence, NMEASentence_GBS *frame)
{
    if (!nmeaScan(sentence, "_tfffifffii",
                  &frame->time,
                  &frame->errLat,
                  &frame->errLon,
                  &frame->errAlt,
                  &frame->svid,
                  &frame->prob,
                  &frame->bias,
                  &frame->stddev,
                  &frame->systemId,
                  &frame->signalId))
    {
        return FALSE;
    }
}

BOOL nmeaParse_GGA(const STRING sentence, NMEASentence_GGA *frame)
{
    if (!nmeaScan(sentence, "_tlclciiffcfcii",
                  &frame->time,
                  &frame->lat,
                  &frame->NS,
                  &frame->lon,
                  &frame->EW,
                  &frame->quality,
                  &frame->numSV,
                  &frame->HDOP,
                  &frame->alt,
                  &frame->altUnit,
                  &frame->sep,
                  &frame->sepUnit,
                  &frame->diffAge,
                  &frame->diffStation))
    {
        return FALSE;
    }
}

BOOL nmeaParse_GLL(const STRING sentence, NMEASentence_GLL *frame)
{
    if (!nmeaScan(sentence, "_lclctcc",
                  &frame->lat,
                  &frame->NS,
                  &frame->lon,
                  &frame->EW,
                  &frame->time,
                  &frame->status,
                  &frame->posMode))
    {
        return FALSE;
    }
}

BOOL nmeaParse_GNS(const STRING sentence, NMEASentence_GNS *frame)
{
    if (!nmeaScan(sentence, "_tlclccifffiic",
                  &frame->time,
                  &frame->lat,
                  &frame->NS,
                  &frame->lon,
                  &frame->EW,
                  &frame->posMode,
                  &frame->numSV,
                  &frame->HDOP,
                  &frame->alt,
                  &frame->sep,
                  &frame->diffAge,
                  &frame->diffStation,
                  &frame->navStatus))
    {
        return FALSE;
    }
}

BOOL nmeaParse_GSA(const STRING sentence, NMEASentence_GSA *frame)
{
    if (!nmeaScan(sentence, "_ciiiiiiiiiiiiifffi",
                  &frame->opMode,
                  &frame->navMode,
                  &frame->svid[0],
                  &frame->svid[1],
                  &frame->svid[2],
                  &frame->svid[3],
                  &frame->svid[4],
                  &frame->svid[5],
                  &frame->svid[6],
                  &frame->svid[7],
                  &frame->svid[8],
                  &frame->svid[9],
                  &frame->svid[10],
                  &frame->svid[11],
                  &frame->PDOP,
                  &frame->HDOP,
                  &frame->VDOP,
                  &frame->systemId))
    {
        return FALSE;
    }
}

BOOL nmeaParse_GST(const STRING sentence, NMEASentence_GST *frame)
{
    if (!nmeaScan(sentence, "_tfffffff",
                  &frame->time,
                  &frame->rangeRms,
                  &frame->stdMajor,
                  &frame->stdMinor,
                  &frame->orient,
                  &frame->stdLat,
                  &frame->stdLong,
                  &frame->stdAlt))
    {
        return FALSE;
    }
}

BOOL nmeaParse_RMC(const STRING sentence, NMEASentence_RMC *frame)
{
    if (!nmeaScan(sentence, "_tclclcffdfccc",
                  &frame->time,
                  &frame->status,
                  &frame->lat,
                  &frame->NS,
                  &frame->lon,
                  &frame->EW,
                  &frame->spd,
                  &frame->cog,
                  &frame->date,
                  &frame->mv,
                  &frame->mvEW,
                  &frame->posMode,
                  &frame->navStatus))
    {
        return FALSE;
    }
}

BOOL nmeaParse_VTG(const STRING sentence, NMEASentence_VTG *frame)
{
    if (!nmeaScan(sentence, "_fcfcfcfcc",
                  &frame->cogt,
                  &frame->cogtUnit,
                  &frame->cogm,
                  &frame->cogmUnit,
                  &frame->sogn,
                  &frame->sognUnit,
                  &frame->sogk,
                  &frame->sogkUnit,
                  &frame->posMode))
    {
        return FALSE;
    }
}

BOOL nmeaParse_ZDA(const STRING sentence, NMEASentence_ZDA *frame)
{
    if (!nmeaScan(sentence, "_tiiiii",
                  &frame->time,
                  &frame->day,
                  &frame->month,
                  &frame->year,
                  &frame->ltzh,
                  &frame->ltzn))
    {
        return FALSE;
    }
}

UINT8 __nmeaCalculateChecksum(const STRING sentence, SIZE length)
{
    UINT8 checksum = 0;

    for (UINT8 i = 0; i < length; i++)
    {
        checksum ^= sentence[i];
    }

    return checksum;
}