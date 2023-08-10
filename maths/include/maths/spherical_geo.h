#pragma once

#include <obc/api.h>
#include "vector.h"

/**
 * @brief Calculate the distance between two points on the Earth's surface. https://en.wikipedia.org/wiki/Haversine_formula
 *
 * @param lat1 Latitude of point 1
 * @param lon1 Longitude of point 1
 * @param lat2 Latitude of point 2
 * @param lon2 Longitude of point 2
 * @return Distance in meters
 */
FLOAT calculateDistanceBetweenGeoPoints(FLOAT lat1, FLOAT lon1, FLOAT lat2, FLOAT lon2);

/**
 * @brief Calculate the great circle distance between two points on the Earth's surface using haversine formula (Vincenty formula). https://en.wikipedia.org/wiki/Great-circle_distance
 *
 * @param lat1 Latitude of point 1
 * @param lon1 Longitude of point 1
 * @param lat2 Latitude of point 2
 * @param lon2 Longitude of point 2
 * @return Distance in meters
 */
FLOAT calculateHaversineFormula(FLOAT lat1, FLOAT lon1, FLOAT lat2, FLOAT lon2);

/**
 * @brief Calculate the bearing between two points on the Earth's surface
 *
 * @param lat1 Latitude of point 1
 * @param lon1 Longitude of point 1
 * @param lat2 Latitude of point 2
 * @param lon2 Longitude of point 2
 * @return Bearing in degrees
 */
FLOAT calculateBearingAngleBetweenGeoPoints(FLOAT lat1, FLOAT lon1, FLOAT lat2, FLOAT lon2);

/**
 * @brief Calculate x and y sides of a triangle from two points on the Earth's surface
 *
 * @param lat1 Latitude of point 1
 * @param lon1 Longitude of point 1
 * @param lat2 Latitude of point 2
 * @param lon2 Longitude of point 2
 * @return Vector with x and y sides
 */
vec2 calculateTriangleSidesFromGeoPoints(FLOAT lat1, FLOAT lon1, FLOAT lat2, FLOAT lon2);