#pragma once

/**
 * REF: https://github.com/kosma/minmea/tree/master
 */

#include <obc/api.h>

/**
 * @brief NMEA sentence IDs
 */
typedef enum NMEASentenceId
{
    NMEA_SENTENCE_UNKNOWN = -1,
    NMEA_SENTENCE_GBS,
    NMEA_SENTENCE_GGA,
    NMEA_SENTENCE_GLL,
    NMEA_SENTENCE_GNS,
    NMEA_SENTENCE_GSA,
    NMEA_SENTENCE_GST,
    NMEA_SENTENCE_GSV,
    NMEA_SENTENCE_RMC,
    NMEA_SENTENCE_VTG,
    NMEA_SENTENCE_ZDA,
} NMEASentenceId;

/**
 * @brief NMEA talker IDs
 */
typedef enum NMEATalker
{
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
    BYTE hour;
    BYTE minute;
    BYTE second;
    BYTE hundredth;
} NMEATime;

/**
 * @brief NMEA structure for GBS sentence
 */
typedef struct NMEASentence_GBS
{
    NMEATime time; /** UTC Time */
    FLOAT errLat;  /** Expected error in latitude */
    FLOAT errLon;  /** Expected error in longtitude */
    FLOAT errAlt;  /** Expected error in altitude */
    INT32 svid;    /** Satellite ID of most likely failed satellite */
    FLOAT prob;    /** Probability of missed detection */
    FLOAT bias;    /** Estimated bias of satellite */
    FLOAT stddev;  /** Standard deviation of estimated bias */
} NMEASentence_GBS;

/**
 * @brief NMEA structure for GGA sentence
 */
typedef struct NMEASentence_GGA
{
    NMEATime time; /** UTC Time */
    FLOAT lat;     /** Latitude */
    CHAR NS;       /** North/South indicator */
    FLOAT lon;     /** Longtitude */
    CHAR EW;       /** East/West indicator */
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
    FLOAT lat;
    CHAR NS;
    FLOAT lon;
    CHAR EW;
    NMEATime time;
    CHAR status;
    CHAR posMode;
} NMEASentence_GLL;

typedef struct NMEASentence_GNS
{
    NMEATime time;
    FLOAT lat;
    CHAR NS;
    FLOAT lon;
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

typedef struct NMEASentence_GSV
{
    INT32 numMsg;
    INT32 msgNum;
    INT32 numSV;
    struct satellite
    {
        INT32 svid;
        INT32 elv;
        INT32 az;
        INT32 cno;
    } satellites[4];
    INT32 signalId;
} NMEASentence_GSV;

typedef struct NMEASentence_RMC
{
    NMEATime time;
    CHAR status;
    FLOAT lat;
    CHAR NS;
    FLOAT lon;
    CHAR EW;
    FLOAT spd;
    FLOAT cog;
    struct
    {
        BYTE day;
        BYTE month;
        BYTE year;
    } date;
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
 * @param length Length of the sentence
 * @return True if the sentence is valid, false otherwise
 */
BOOL nmeaCheckSentence(const BYTE *sentence, SIZE length);

/**
 * @brief Scan the sentence and parse it
 *
 * @param sentence Sentence to parse
 * @param length Length of the sentence
 * @param format Format string
 * @param ... Arguments
 * @return True if the sentence was parsed successfully, false otherwise
 */
BOOL nmeaScan(const BYTE *sentence, SIZE length, const CHAR *format, ...);

/**
 * @brief Get the sentence ID
 *
 * @param sentence Sentence to parse
 * @param length Length of the sentence
 * @return Sentence ID
 */
NMEASentenceId nmeaGetSentenceId(const BYTE *sentence, SIZE length);

/**
 * @brief Get the talker ID
 *
 * @param sentence Sentence to parse
 * @param length Length of the sentence
 * @return Talker ID
 */
NMEATalker nmeaGetTalkerId(const BYTE *sentence, SIZE length);

/**
 * @brief Parse the GBS sentence
 *
 * @param sentence Sentence to parse
 * @param length Length of the sentence
 * @param gbs GBS sentence structure
 * @return True if the sentence was parsed successfully, false otherwise
 */
BOOL nmeaParse_GBS(const BYTE *sentence, SIZE length, NMEASentence_GBS *gbs);

/**
 * @brief Parse the GGA sentence
 *
 * @param sentence Sentence to parse
 * @param length Length of the sentence
 * @param gga GGA sentence structure
 * @return True if the sentence was parsed successfully, false otherwise
 */
BOOL nmeaParse_GGA(const BYTE *sentence, SIZE length, NMEASentence_GGA *gga);

/**
 * @brief Parse the GLL sentence
 *
 * @param sentence Sentence to parse
 * @param length Length of the sentence
 * @param gll GLL sentence structure
 * @return True if the sentence was parsed successfully, false otherwise
 */
BOOL nmeaParse_GLL(const BYTE *sentence, SIZE length, NMEASentence_GLL *gll);

/**
 * @brief Parse the GNS sentence
 *
 * @param sentence Sentence to parse
 * @param length Length of the sentence
 * @param gns GNS sentence structure
 * @return True if the sentence was parsed successfully, false otherwise
 */
BOOL nmeaParse_GNS(const BYTE *sentence, SIZE length, NMEASentence_GNS *gns);

/**
 * @brief Parse the GSA sentence
 *
 * @param sentence Sentence to parse
 * @param length Length of the sentence
 * @param gsa GSA sentence structure
 * @return True if the sentence was parsed successfully, false otherwise
 */
BOOL nmeaParse_GSA(const BYTE *sentence, SIZE length, NMEASentence_GSA *gsa);

/**
 * @brief Parse the GST sentence
 *
 * @param sentence Sentence to parse
 * @param length Length of the sentence
 * @param gst GST sentence structure
 * @return True if the sentence was parsed successfully, false otherwise
 */
BOOL nmeaParse_GST(const BYTE *sentence, SIZE length, NMEASentence_GST *gst);

/**
 * @brief Parse the GSV sentence
 *
 * @param sentence Sentence to parse
 * @param length Length of the sentence
 * @param gsv GSV sentence structure
 * @return True if the sentence was parsed successfully, false otherwise
 */
BOOL nmeaParse_GSV(const BYTE *sentence, SIZE length, NMEASentence_GSV *gsv);

/**
 * @brief Parse the RMC sentence
 *
 * @param sentence Sentence to parse
 * @param length Length of the sentence
 * @param rmc RMC sentence structure
 * @return True if the sentence was parsed successfully, false otherwise
 */
BOOL nmeaParse_RMC(const BYTE *sentence, SIZE length, NMEASentence_RMC *rmc);

/**
 * @brief Parse the VTG sentence
 *
 * @param sentence Sentence to parse
 * @param length Length of the sentence
 * @param vtg VTG sentence structure
 * @return True if the sentence was parsed successfully, false otherwise
 */
BOOL nmeaParse_VTG(const BYTE *sentence, SIZE length, NMEASentence_VTG *vtg);

/**
 * @brief Parse the ZDA sentence
 *
 * @param sentence Sentence to parse
 * @param length Length of the sentence
 * @param zda ZDA sentence structure
 * @return True if the sentence was parsed successfully, false otherwise
 */
BOOL nmeaParse_ZDA(const BYTE *sentence, SIZE length, NMEASentence_ZDA *zda);

/**
 * @brief Calculate the checksum of the sentence
 *
 * @param sentence Sentence to calculate the checksum
 * @param length Length of the sentence
 * @return Checksum
 */
BYTE __nmeaCalculateChecksum(const BYTE *sentence, SIZE length);