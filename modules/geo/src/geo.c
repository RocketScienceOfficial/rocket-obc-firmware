#include "modules/geo/geo.h"
#include "modules/geo/physical_constants.h"
#include "modules/maths/math_constants.h"
#include <math.h>

static void geo_pos_to_rad(geo_position_wgs84_t *pos);
static void geo_pos_to_deg(geo_position_wgs84_t *pos);
static vec3_prec_t _geo_to_ecef(geo_position_wgs84_t pos);
static geo_position_wgs84_t _ecef_to_geo(vec3_prec_t pos);
static vec3_prec_t _ecef_to_ned(vec3_prec_t ecef, geo_position_wgs84_t basePos);
static vec3_prec_t _ned_to_ecef(vec3_prec_t ned, geo_position_wgs84_t basePos);

vec3_prec_t geo_to_ned(geo_position_wgs84_t basePos, geo_position_wgs84_t pos)
{
    geo_pos_to_rad(&basePos);
    geo_pos_to_rad(&pos);

    vec3_prec_t ecef = _geo_to_ecef(pos);

    return _ecef_to_ned(ecef, basePos);
}

geo_position_wgs84_t ned_to_geo(geo_position_wgs84_t basePos, vec3_prec_t pos)
{
    geo_pos_to_rad(&basePos);

    vec3_prec_t ecef = _ned_to_ecef(pos, basePos);
    geo_position_wgs84_t geo = _ecef_to_geo(ecef);

    geo_pos_to_deg(&geo);

    return geo;
}

float geo_calculate_distance(geo_position_t p0, geo_position_t p1)
{
    p0.lat = DEG_2_RAD(p0.lat);
    p1.lat = DEG_2_RAD(p1.lat);

    double d_lat = p1.lat - p0.lat;
    double d_lon = DEG_2_RAD(p1.lon - p0.lon);

    double sin_d_lat_2 = sin(d_lat / 2.0);
    double sin_d_lon_2 = sin(d_lon / 2.0);

    double a = sin_d_lat_2 * sin_d_lat_2 + sin_d_lon_2 * sin_d_lon_2 * cos(p0.lat) * cos(p1.lat);
    double c = 2 * atan2(sqrt(a), sqrt(1.0 - a));

    return (float)c * EARTH_RADIUS;
}

float geo_calculate_bearing(geo_position_t p0, geo_position_t p1)
{
    p0.lat = DEG_2_RAD(p0.lat);
    p1.lat = DEG_2_RAD(p1.lat);

    double cos_lat1 = cos(p1.lat);
    double d_lon = DEG_2_RAD(p1.lon - p0.lon);

    double y = sin(d_lon) * cos_lat1;
    double x = cos(p0.lat) * sin(p1.lat) - sin(p0.lat) * cos_lat1 * cos(d_lon);
    double theta = atan2(y, x);

    float bearing = fmodf((float)RAD_2_DEG(theta) + 360.0f, 360.0f);

    return bearing;
}

static void geo_pos_to_rad(geo_position_wgs84_t *pos)
{
    pos->lat = DEG_2_RAD(pos->lat);
    pos->lon = DEG_2_RAD(pos->lon);
}

static void geo_pos_to_deg(geo_position_wgs84_t *pos)
{
    pos->lat = RAD_2_DEG(pos->lat);
    pos->lon = RAD_2_DEG(pos->lon);
}

static vec3_prec_t _geo_to_ecef(geo_position_wgs84_t pos)
{
    double sin_lat = sin(pos.lat);
    double cos_lat = cos(pos.lat);

    double n = EARTH_SEMI_MAJOR_AXIS / sqrt(1.0 - EARTH_ECCENTRICITY_SQUARED * sin_lat * sin_lat);

    return (vec3_prec_t){
        .x = (n + (double)pos.alt) * cos_lat * cos(pos.lon),
        .y = (n + (double)pos.alt) * cos_lat * sin(pos.lon),
        .z = ((1.0 - EARTH_ECCENTRICITY_SQUARED) * n + (double)pos.alt) * sin_lat,
    };
}

static geo_position_wgs84_t _ecef_to_geo(vec3_prec_t pos)
{
    double s = sqrt(pos.x * pos.x + pos.y * pos.y);

    double beta = atan(pos.z / ((1.0 - EARTH_FLATTENING) * s));
    double sin_beta = sin(beta);
    double cos_beta = cos(beta);
    double miu = atan(
        (pos.z + EARTH_ECCENTRICITY_SQUARED * (1.0 - EARTH_FLATTENING) / (1.0 - EARTH_ECCENTRICITY_SQUARED) * EARTH_SEMI_MAJOR_AXIS * sin_beta * sin_beta * sin_beta) / (s - EARTH_ECCENTRICITY_SQUARED * EARTH_SEMI_MAJOR_AXIS * cos_beta * cos_beta * cos_beta));
    double err = 1e10;
    int iter = 0;

    const int MAX_ITER = 3;

    while (err > 1e-10 && iter < MAX_ITER)
    {
        double last_miu = miu;

        beta = atan((1.0 - EARTH_FLATTENING) * tan(miu));
        sin_beta = sin(beta);
        cos_beta = cos(beta);
        miu = atan(
            (pos.z + EARTH_ECCENTRICITY_SQUARED * (1.0 - EARTH_FLATTENING) / (1.0 - EARTH_ECCENTRICITY_SQUARED) * EARTH_SEMI_MAJOR_AXIS * sin_beta * sin_beta * sin_beta) / (s - EARTH_ECCENTRICITY_SQUARED * EARTH_SEMI_MAJOR_AXIS * cos_beta * cos_beta * cos_beta));

        err = last_miu - miu;
        iter++;
    }

    double sin_miu = sin(miu);
    double n = EARTH_SEMI_MAJOR_AXIS / sqrt(1.0 - EARTH_ECCENTRICITY_SQUARED * sin_miu * sin_miu);
    double h = s * cos(miu) + (pos.z + EARTH_ECCENTRICITY_SQUARED * n * sin(miu)) * sin(miu) - n;

    return (geo_position_wgs84_t){
        .lat = miu,
        .lon = atan2(pos.y, pos.x),
        .alt = h,
    };
}

static vec3_prec_t _ecef_to_ned(vec3_prec_t ecef, geo_position_wgs84_t basePos)
{
    vec3_prec_t ecef0 = _geo_to_ecef(basePos);
    vec3_prec_t diff = {
        .x = ecef.x - ecef0.x,
        .y = ecef.y - ecef0.y,
        .z = ecef.z - ecef0.z,
    };

    double sin_lat = sin(basePos.lat);
    double cos_lat = cos(basePos.lat);
    double sin_lon = sin(basePos.lon);
    double cos_lon = cos(basePos.lon);

    return (vec3_prec_t){
        .x = diff.x * (-sin_lat * cos_lon) + diff.y * (-sin_lat * sin_lon) + diff.z * (+cos_lat),
        .y = diff.x * (-sin_lon) + diff.y * (+cos_lon),
        .z = diff.x * (-cos_lat * cos_lon) + diff.y * (-cos_lat * sin_lon) + diff.z * (-sin_lat),
    };
}

static vec3_prec_t _ned_to_ecef(vec3_prec_t ned, geo_position_wgs84_t basePos)
{
    vec3_prec_t ecef0 = _geo_to_ecef(basePos);

    double sin_lat = sin(basePos.lat);
    double cos_lat = cos(basePos.lat);
    double sin_lon = sin(basePos.lon);
    double cos_lon = cos(basePos.lon);

    return (vec3_prec_t){
        .x = ecef0.x + ned.x * (-sin_lat * cos_lon) + ned.y * (-sin_lon) + ned.z * (-cos_lat * cos_lon),
        .y = ecef0.y + ned.x * (-sin_lat * sin_lon) + ned.y * (+cos_lon) + ned.z * (-cos_lat * sin_lon),
        .z = ecef0.z + ned.x * (cos_lat) + ned.z * (-sin_lat),
    };
}