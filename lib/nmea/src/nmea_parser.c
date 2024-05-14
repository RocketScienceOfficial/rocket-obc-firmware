#include "lib/nmea/nmea_parser.h"
#include "hal/serial_driver.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>

bool nmea_check_sentence(const char *sentence)
{
    if (sentence[0] != '$')
    {
        hal_serial_printf("Invalid sentence starting!\n");

        return false;
    }

    uint8_t length = strlen(sentence);
    uint8_t checksum = 0;
    uint8_t i = 0;

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
        uint8_t checksum1 = 0;
        uint8_t checksum2 = 0;

        if (i > length - 1 - 2)
        {
            return false;
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
            hal_serial_printf("Checksum of sentence is not valid!\n");

            return false;
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
            hal_serial_printf("Checksum of sentence is not valid!\n");

            return false;
        }

        if (checksum != (checksum1 << 4 | checksum2))
        {
            hal_serial_printf("Checksum of sentence is not valid!\n");

            return false;
        }

        i += 2;
    }
    else
    {
        return false;
    }

    if (i > length - 1 - 2)
    {
        return false;
    }

    if (sentence[i + 1] != '\r' || sentence[i + 2] != '\n')
    {
        return false;
    }

    i += 2;

    if (i != length - 1)
    {
        return false;
    }

    return true;
}

bool nmea_scan(const char *sentence, const char *format, ...)
{
    va_list ap;
    va_start(ap, format);

    char field[32];
    uint8_t fieldIndex = 0;
    uint8_t index = 1;
    bool valid = true;

    for (uint8_t i = 0; format[i] != '\0'; i++)
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
            valid = false;
        }

        index++;

        switch (format[i])
        {
        case 'i':
        {
            int *value = va_arg(ap, int *);
            *value = field != NULL ? atoi(field) : 0;
            break;
        }
        case 'f':
        {
            float *value = va_arg(ap, float *);
            *value = field != NULL ? atof(field) : 0.0f;
            break;
        }
        case 'c':
        {
            char *value = va_arg(ap, char *);
            *value = field != NULL ? field[0] : '\0';
            break;
        }
        case 's':
        {
            char *value = va_arg(ap, char *);

            if (field != NULL)
            {
                memcpy(value, field, fieldIndex);
            }
            break;
        }
        case 'l':
        {
            double *value = va_arg(ap, double *);

            if (field != NULL)
            {
                double raw = atof(field);
                double raw2 = raw / 100.0;
                double degs = floor(raw2);
                double mins = (raw2 - degs) * 100.0;
                double minsToDeg = mins / 60.0;

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
            nmea_date_t *value = va_arg(ap, nmea_date_t *);

            if (field != NULL)
            {
                char d[3] = {
                    field[0],
                    field[1],
                    '\0',
                };
                char m[3] = {
                    field[2],
                    field[3],
                    '\0',
                };
                char y[3] = {
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
            nmea_time_t *value = va_arg(ap, nmea_time_t *);

            if (field != NULL)
            {
                char h[3] = {
                    field[0],
                    field[1],
                    '\0',
                };
                char m[3] = {
                    field[2],
                    field[3],
                    '\0',
                };
                char s[3] = {
                    field[4],
                    field[5],
                    '\0',
                };
                char hs[3] = {
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
            return false;
        }
        }
    }

    va_end(ap);

    return true;
}

nmea_sentence_id_t nmea_get_sentence_id(const char *sentence)
{
    char s[6];

    if (!nmea_scan(sentence, "s", s))
    {
        return NMEA_SENTENCE_UNKNOWN;
    }

    char id[4] = {
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

nmea_talker_t nmea_get_talker_id(const char *sentence)
{
    char s[6];

    if (!nmea_scan(sentence, "s", s))
    {
        return NMEA_TALKER_UNKNOWN;
    }

    char id[3] = {
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

bool nmea_parse_gbs(const char *sentence, nmea_sentence_gbs_t *frame)
{
    if (!nmea_scan(sentence, "_tfffifffii",
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
        return false;
    }
}

bool nmea_parse_gga(const char *sentence, nmea_sentence_gga_t *frame)
{
    if (!nmea_scan(sentence, "_tlclciiffcfcii",
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
        return false;
    }
}

bool nmea_parse_gll(const char *sentence, nmea_sentence_gll_t *frame)
{
    if (!nmea_scan(sentence, "_lclctcc",
                   &frame->lat,
                   &frame->NS,
                   &frame->lon,
                   &frame->EW,
                   &frame->time,
                   &frame->status,
                   &frame->posMode))
    {
        return false;
    }
}

bool nmea_parse_gns(const char *sentence, nmea_sentence_gns_t *frame)
{
    if (!nmea_scan(sentence, "_tlclccifffiic",
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
        return false;
    }
}

bool nmea_parse_gsa(const char *sentence, nmea_sentence_gsa_t *frame)
{
    if (!nmea_scan(sentence, "_ciiiiiiiiiiiiifffi",
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
        return false;
    }
}

bool nmea_parse_gst(const char *sentence, nmea_sentence_gst_t *frame)
{
    if (!nmea_scan(sentence, "_tfffffff",
                   &frame->time,
                   &frame->rangeRms,
                   &frame->stdMajor,
                   &frame->stdMinor,
                   &frame->orient,
                   &frame->stdLat,
                   &frame->stdLong,
                   &frame->stdAlt))
    {
        return false;
    }
}

bool nmea_parse_rmc(const char *sentence, nmea_sentence_rmc_t *frame)
{
    if (!nmea_scan(sentence, "_tclclcffdfccc",
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
        return false;
    }
}

bool nmea_parse_vtg(const char *sentence, nmea_sentence_vtg_t *frame)
{
    if (!nmea_scan(sentence, "_fcfcfcfcc",
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
        return false;
    }
}

bool nmea_parse_zda(const char *sentence, nmea_sentence_zda_t *frame)
{
    if (!nmea_scan(sentence, "_tiiiii",
                   &frame->time,
                   &frame->day,
                   &frame->month,
                   &frame->year,
                   &frame->ltzh,
                   &frame->ltzn))
    {
        return false;
    }
}