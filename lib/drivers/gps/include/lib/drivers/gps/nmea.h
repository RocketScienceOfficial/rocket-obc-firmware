#ifndef _NMEA_PARSER_H
#define _NMEA_PARSER_H

/**
 * REF: https://github.com/kosma/minmea
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define NMEA_SENTENCE_MAX_LENGTH 83

/**
 * @brief NMEA sentence IDs
 */
typedef enum nmea_sentence_id
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
} nmea_sentence_id_t;

/**
 * @brief NMEA talker IDs
 */
typedef enum nmea_talker
{
    NMEA_TALKER_UNKNOWN = -1,
    NMEA_TALKER_GPS_SBAS,
    NMEA_TALKER_GLONASS,
    NMEA_TALKER_GALILEO,
    NMEA_TALKER_BEIDOU,
    NMEA_TALKER_QZSS,
    NMEA_TALKER_ANY,
} nmea_talker_t;

/**
 * @brief NMEA time structure
 */
typedef struct nmea_time
{
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t hundredth;
} nmea_time_t;

/**
 * @brief NMEA date structure
 */
typedef struct nmea_date
{
    uint8_t day;
    uint8_t month;
    uint8_t year;
} nmea_date_t;

/**
 * @brief NMEA satellite structure
 */
typedef struct nmea_satellite
{
    int svid;
    int elv;
    int az;
    int cno;
} nmea_satellite_t;

/**
 * @brief NMEA structure for GBS sentence
 */
typedef struct nmea_sentence_gbs
{
    nmea_time_t time;
    float errLat;
    float errLon;
    float errAlt;
    int svid;
    float prob;
    float bias;
    float stddev;
    int systemId;
    int signalId;
} nmea_sentence_gbs_t;

/**
 * @brief NMEA structure for GGA sentence
 */
typedef struct nmea_sentence_gga
{
    nmea_time_t time;
    double lat;
    char NS;
    double lon;
    char EW;
    int quality;
    int numSV;
    float HDOP;
    float alt;
    char altUnit;
    float sep;
    char sepUnit;
    int diffAge;
    int diffStation;
} nmea_sentence_gga_t;

/**
 * @brief NMEA structure for GLL sentence
 */
typedef struct nmea_sentence_gll
{
    double lat;
    char NS;
    double lon;
    char EW;
    nmea_time_t time;
    char status;
    char posMode;
} nmea_sentence_gll_t;

/**
 * @brief NMEA structure for GNS sentence
 */
typedef struct nmea_sentence_gns
{
    nmea_time_t time;
    double lat;
    char NS;
    double lon;
    char EW;
    char posMode[4];
    int numSV;
    float HDOP;
    float alt;
    float sep;
    int diffAge;
    int diffStation;
    char navStatus;
} nmea_sentence_gns_t;

/**
 * @brief NMEA structure for GSA sentence
 */
typedef struct nmea_sentence_gsa
{
    char opMode;
    int navMode;
    int svid[12];
    float PDOP;
    float HDOP;
    float VDOP;
    int systemId;
} nmea_sentence_gsa_t;

/**
 * @brief NMEA structure for GST sentence
 */
typedef struct nmea_sentence_gst
{
    nmea_time_t time;
    float rangeRms;
    float stdMajor;
    float stdMinor;
    float orient;
    float stdLat;
    float stdLong;
    float stdAlt;
} nmea_sentence_gst_t;

/**
 * @brief NMEA structure for GSV sentence
 */
typedef struct nmea_sentence_gsv
{
    int numMsg;
    int msgNum;
    int numSV;
    nmea_satellite_t sats[4];
    int signalId;
} nmea_sentence_gsv_t;

/**
 * @brief NMEA structure for RMC sentence
 */
typedef struct nmea_sentence_rmc
{
    nmea_time_t time;
    char status;
    double lat;
    char NS;
    double lon;
    char EW;
    float spd;
    float cog;
    nmea_date_t date;
    float mv;
    char mvEW;
    char posMode;
    char navStatus;
} nmea_sentence_rmc_t;

/**
 * @brief NMEA structure for VTG sentence
 */
typedef struct nmea_sentence_vtg
{
    float cogt;
    char cogtUnit;
    float cogm;
    char cogmUnit;
    float sogn;
    char sognUnit;
    float sogk;
    char sogkUnit;
    char posMode;
} nmea_sentence_vtg_t;

/**
 * @brief NMEA structure for ZDA sentence
 */
typedef struct nmea_sentence_zda
{
    nmea_time_t time;
    int day;
    int month;
    int year;
    int ltzh;
    int ltzn;
} nmea_sentence_zda_t;

/**
 * @brief Check if the sentence is valid
 *
 * @param sentence Sentence to check (with null terminator)
 * @return True if the sentence is valid, false otherwise
 */
bool nmea_check_sentence(const char *sentence);

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
bool nmea_scan(const char *sentence, const char *format, ...);

/**
 * @brief Get the sentence ID
 *
 * @param sentence Sentence to parse
 * @return Sentence ID
 */
nmea_sentence_id_t nmea_get_sentence_id(const char *sentence);

/**
 * @brief Get the talker ID
 *
 * @param sentence Sentence to parse
 * @return Talker ID
 */
nmea_talker_t nmea_get_talker_id(const char *sentence);

/**
 * @brief Parse the GBS sentence
 *
 * @param sentence Sentence to parse
 * @param frame GBS sentence structure
 * @return True if the sentence was parsed successfully, false otherwise
 */
bool nmea_parse_gbs(const char *sentence, nmea_sentence_gbs_t *frame);

/**
 * @brief Parse the GGA sentence
 *
 * @param sentence Sentence to parse
 * @param frame GGA sentence structure
 * @return True if the sentence was parsed successfully, false otherwise
 */
bool nmea_parse_gga(const char *sentence, nmea_sentence_gga_t *frame);

/**
 * @brief Parse the GLL sentence
 *
 * @param sentence Sentence to parse
 * @param frame GLL sentence structure
 * @return True if the sentence was parsed successfully, false otherwise
 */
bool nmea_parse_gll(const char *sentence, nmea_sentence_gll_t *frame);

/**
 * @brief Parse the GNS sentence
 *
 * @param sentence Sentence to parse
 * @param frame GNS sentence structure
 * @return True if the sentence was parsed successfully, false otherwise
 */
bool nmea_parse_gns(const char *sentence, nmea_sentence_gns_t *frame);

/**
 * @brief Parse the GSA sentence
 *
 * @param sentence Sentence to parse
 * @param frame GSA sentence structure
 * @return True if the sentence was parsed successfully, false otherwise
 */
bool nmea_parse_gsa(const char *sentence, nmea_sentence_gsa_t *frame);

/**
 * @brief Parse the GST sentence
 *
 * @param sentence Sentence to parse
 * @param frame GST sentence structure
 * @return True if the sentence was parsed successfully, false otherwise
 */
bool nmea_parse_gst(const char *sentence, nmea_sentence_gst_t *frame);

/**
 * @brief Parse the GSV sentence
 *
 * @param sentence Sentence to parse
 * @param frame GSV sentence structure
 * @return True if the sentence was parsed successfully, false otherwise
 */
bool nmea_parse_gsv(const char *sentence, nmea_sentence_gsv_t *frame);

/**
 * @brief Parse the RMC sentence
 *
 * @param sentence Sentence to parse
 * @param frame RMC sentence structure
 * @return True if the sentence was parsed successfully, false otherwise
 */
bool nmea_parse_rmc(const char *sentence, nmea_sentence_rmc_t *frame);

/**
 * @brief Parse the VTG sentence
 *
 * @param sentence Sentence to parse
 * @param frame VTG sentence structure
 * @return True if the sentence was parsed successfully, false otherwise
 */
bool nmea_parse_vtg(const char *sentence, nmea_sentence_vtg_t *frame);

/**
 * @brief Parse the ZDA sentence
 *
 * @param sentence Sentence to parse
 * @param frame ZDA sentence structure
 * @return True if the sentence was parsed successfully, false otherwise
 */
bool nmea_parse_zda(const char *sentence, nmea_sentence_zda_t *frame);

#endif