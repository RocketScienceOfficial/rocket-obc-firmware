#pragma once

/**
 * REF: https://github.com/kosma/minmea
 */

#include <obc/api.h>

/**
 * @brief NMEA sentence IDs
 */
typedef enum NMEASentence
{
    NMEA_SENTENCE_UNKNOWN = -1,
    NMEA_SENTENCE_GBS,
    NMEA_SENTENCE_GGA,
    NMEA_SENTENCE_GLL,
    NMEA_SENTENCE_GNS,
    NMEA_SENTENCE_GSA,
    NMEA_SENTENCE_GST,
    NMEA_SENTENCE_RMC,
    NMEA_SENTENCE_VTG,
    NMEA_SENTENCE_ZDA,
} NMEASentence;

/**
 * @brief NMEA talker IDs
 */
typedef enum NMEATalker
{
    NMEA_TALKER_UNKNOWN = -1,
    NMEA_TALKER_GPS_SBAS,
    NMEA_TALKER_GLONASS,
    NMEA_TALKER_GALILEO,
    NMEA_TALKER_BEIDOU,
    NMEA_TALKER_QZSS,
    NMEA_TALKER_ANY,
} NMEATalker;

/**
 * @brief NMEA time structure
 */
typedef struct NMEATime
{
    UINT8 hour;
    UINT8 minute;
    UINT8 second;
    UINT8 hundredth;
} NMEATime;

/**
 * @brief NMEA date structure
 */
typedef struct NMEADate
{
    UINT8 day;
    UINT8 month;
    UINT8 year;
} NMEADate;

/**
 * @brief NMEA satellite structure
 */
typedef struct NMEASatellite
{
    INT32 svid;
    INT32 elv;
    INT32 az;
    INT32 cno;
} NMEASatellite;

/**
 * @brief NMEA latitude/longtitude type
 */
typedef DOUBLE NMEALatLon;

/**
 * @brief NMEA structure for GBS sentence
 */
typedef struct NMEASentence_GBS
{
    NMEATime time;
    FLOAT errLat;
    FLOAT errLon;
    FLOAT errAlt;
    INT32 svid;
    FLOAT prob;
    FLOAT bias;
    FLOAT stddev;
    INT32 systemId;
    INT32 signalId;
} NMEASentence_GBS;

/**
 * @brief NMEA structure for GGA sentence
 */
typedef struct NMEASentence_GGA
{
    NMEATime time;
    NMEALatLon lat;
    CHAR NS;
    NMEALatLon lon;
    CHAR EW;
    INT32 quality;
    INT32 numSV;
    FLOAT HDOP;
    FLOAT alt;
    CHAR altUnit;
    FLOAT sep;
    CHAR sepUnit;
    INT32 diffAge;
    INT32 diffStation;
} NMEASentence_GGA;

typedef struct NMEASentence_GLL
{
    NMEALatLon lat;
    CHAR NS;
    NMEALatLon lon;
    CHAR EW;
    NMEATime time;
    CHAR status;
    CHAR posMode;
} NMEASentence_GLL;

typedef struct NMEASentence_GNS
{
    NMEATime time;
    NMEALatLon lat;
    CHAR NS;
    NMEALatLon lon;
    CHAR EW;
    CHAR posMode;
    INT32 numSV;
    FLOAT HDOP;
    FLOAT alt;
    FLOAT sep;
    INT32 diffAge;
    INT32 diffStation;
    CHAR navStatus;
} NMEASentence_GNS;

typedef struct NMEASentence_GSA
{
    CHAR opMode;
    INT32 navMode;
    INT32 svid[12];
    FLOAT PDOP;
    FLOAT HDOP;
    FLOAT VDOP;
    INT32 systemId;
} NMEASentence_GSA;

typedef struct NMEASentence_GST
{
    NMEATime time;
    FLOAT rangeRms;
    FLOAT stdMajor;
    FLOAT stdMinor;
    FLOAT orient;
    FLOAT stdLat;
    FLOAT stdLong;
    FLOAT stdAlt;
} NMEASentence_GST;

typedef struct NMEASentence_RMC
{
    NMEATime time;
    CHAR status;
    NMEALatLon lat;
    CHAR NS;
    NMEALatLon lon;
    CHAR EW;
    FLOAT spd;
    FLOAT cog;
    NMEADate date;
    FLOAT mv;
    CHAR mvEW;
    CHAR posMode;
    CHAR navStatus;
} NMEASentence_RMC;

typedef struct NMEASentence_VTG
{
    FLOAT cogt;
    CHAR cogtUnit;
    FLOAT cogm;
    CHAR cogmUnit;
    FLOAT sogn;
    CHAR sognUnit;
    FLOAT sogk;
    CHAR sogkUnit;
    CHAR posMode;
} NMEASentence_VTG;

typedef struct NMEASentence_ZDA
{
    NMEATime time;
    INT32 day;
    INT32 month;
    INT32 year;
    INT32 ltzh;
    INT32 ltzn;
} NMEASentence_ZDA;

/**
 * @brief Check if the sentence is valid
 *
 * @param sentence Sentence to check
 * @return True if the sentence is valid, false otherwise
 */
BOOL nmeaCheckSentence(const STRING sentence);

/**
 * @brief Scan the sentence and parse it. Use following format specifiers:
 * - i: integer
 * - f: float
 * - c: char
 * - s: string
 * - l: latitude or longtitude
 * - d: date
 * - t: time
 * - _: skip
 *
 * @param sentence Sentence to parse
 * @param format Format string
 * @param ... Arguments
 * @return True if the sentence was parsed successfully, false otherwise
 */
BOOL nmeaScan(const STRING sentence, const STRING format, ...);

/**
 * @brief Get the sentence ID
 *
 * @param sentence Sentence to parse
 * @return Sentence ID
 */
NMEASentence nmeaGetSentenceId(const STRING sentence);

/**
 * @brief Get the talker ID
 *
 * @param sentence Sentence to parse
 * @return Talker ID
 */
NMEATalker nmeaGetTalkerId(const STRING sentence);

/**
 * @brief Parse the GBS sentence
 *
 * @param sentence Sentence to parse
 * @param frame GBS sentence structure
 * @return True if the sentence was parsed successfully, false otherwise
 */
BOOL nmeaParse_GBS(const STRING sentence, NMEASentence_GBS *frame);

/**
 * @brief Parse the GGA sentence
 *
 * @param sentence Sentence to parse
 * @param frame GGA sentence structure
 * @return True if the sentence was parsed successfully, false otherwise
 */
BOOL nmeaParse_GGA(const STRING sentence, NMEASentence_GGA *frame);

/**
 * @brief Parse the GLL sentence
 *
 * @param sentence Sentence to parse
 * @param frame GLL sentence structure
 * @return True if the sentence was parsed successfully, false otherwise
 */
BOOL nmeaParse_GLL(const STRING sentence, NMEASentence_GLL *frame);

/**
 * @brief Parse the GNS sentence
 *
 * @param sentence Sentence to parse
 * @param frame GNS sentence structure
 * @return True if the sentence was parsed successfully, false otherwise
 */
BOOL nmeaParse_GNS(const STRING sentence, NMEASentence_GNS *frame);

/**
 * @brief Parse the GSA sentence
 *
 * @param sentence Sentence to parse
 * @param frame GSA sentence structure
 * @return True if the sentence was parsed successfully, false otherwise
 */
BOOL nmeaParse_GSA(const STRING sentence, NMEASentence_GSA *frame);

/**
 * @brief Parse the GST sentence
 *
 * @param sentence Sentence to parse
 * @param frame GST sentence structure
 * @return True if the sentence was parsed successfully, false otherwise
 */
BOOL nmeaParse_GST(const STRING sentence, NMEASentence_GST *frame);

/**
 * @brief Parse the RMC sentence
 *
 * @param sentence Sentence to parse
 * @param frame RMC sentence structure
 * @return True if the sentence was parsed successfully, false otherwise
 */
BOOL nmeaParse_RMC(const STRING sentence, NMEASentence_RMC *frame);

/**
 * @brief Parse the VTG sentence
 *
 * @param sentence Sentence to parse
 * @param frame VTG sentence structure
 * @return True if the sentence was parsed successfully, false otherwise
 */
BOOL nmeaParse_VTG(const STRING sentence, NMEASentence_VTG *frame);

/**
 * @brief Parse the ZDA sentence
 *
 * @param sentence Sentence to parse
 * @param frame ZDA sentence structure
 * @return True if the sentence was parsed successfully, false otherwise
 */
BOOL nmeaParse_ZDA(const STRING sentence, NMEASentence_ZDA *frame);

/**
 * @brief Calculate the checksum of the sentence
 *
 * @param sentence Sentence to calculate the checksum
 * @param length Length of the sentence
 * @return Checksum
 */
UINT8 __nmeaCalculateChecksum(const STRING sentence, SIZE length);