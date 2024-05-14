#ifndef _GEO_H
#define _GEO_H

#include "lib/maths/vector.h"

/**
 * @brief Structure for holding geo coordinates
 */
typedef struct geo_position
{
    double lat; /** Latitude in degrees */
    double lon; /** Latitude in degrees */
} geo_position_t;

/**
 * @brief Structure for holding WGS 84 geo coordinates (includes altitude)
 */
typedef struct geo_position_wgs84
{
    double lat; /** Latitude in degrees */
    double lon; /** Latitude in degrees */
    float alt;  /** Altitude in meters */
} geo_position_wgs84_t;

/**
 * @brief Get XYZ position in NED frame from geo positions
 *
 * @param basePos Position of reference point
 * @param pos Position of current point
 * @return XYZ position
 */
vec3_prec_t geo_to_ned(geo_position_wgs84_t basePos, geo_position_wgs84_t pos);

/**
 * @brief Get geo position from NED frame
 *
 * @param basePos Position of reference point
 * @param pos NED frame position
 * @return Geo position
 */
geo_position_wgs84_t ned_to_geo(geo_position_wgs84_t basePos, vec3_prec_t pos);

/**
 * @brief Calculate distance between two coordinates
 *
 * @param p0 First point
 * @param p1 Second point
 * @return Distance in meters
 */
float geo_calculate_distance(geo_position_t p0, geo_position_t p1);

/**
 * @brief Calculate bearing (angle in degrees) between two coordinates
 *
 * @param p0 First point
 * @param p1 Second point
 * @return Angle in degrees
 */
float geo_calculate_bearing(geo_position_t p0, geo_position_t p1);

#endif