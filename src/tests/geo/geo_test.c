#include "tests/test_framework.h"
#include <lib/geo/geo.h>
#include <lib/geo/geo_utils.h>
#include <lib/geo/geo_mag.h>
#include <lib/maths/math_utils.h>

int main()
{
    TEST_START();

    TEST_START_SECTION("Barometric formula");
    {
        float val = height_from_baro_formula(98000);

        TEST_ASSERT(value_approx_eql(val, 280.53f, 0.1f));
    }
    TEST_END_SECTION();

    TEST_START_SECTION("Magnetic strength");
    {
        float val = geo_mag_get_strength((geo_position_t){52.2330335, 20.8963895});

        TEST_ASSERT(value_approx_eql(val, 0.50f, 0.02f));
    }
    TEST_END_SECTION();

    TEST_START_SECTION("GEO to NED");
    {
        vec3_prec_t val = geo_to_ned((geo_position_wgs84_t){44.532, -72.782, 1699.0f}, (geo_position_wgs84_t){44.544, -72.814, 1340.0f});

        TEST_ASSERT(value_approx_eql(val.x, 1334.25f, 0.1f));
        TEST_ASSERT(value_approx_eql(val.y, -2543.56f, 0.1f));
        TEST_ASSERT(value_approx_eql(val.z, 359.64f, 0.1f));
    }
    TEST_END_SECTION();

    TEST_START_SECTION("NED to GEO");
    {
        geo_position_wgs84_t val = ned_to_geo((geo_position_wgs84_t){44.532, -72.782, 1699.0f}, (vec3_prec_t){1334.304, -2544.368, 359.961});

        TEST_ASSERT(value_approx_eql(val.lat, 44.544f, 0.1f));
        TEST_ASSERT(value_approx_eql(val.lon, -72.814f, 0.1f));
        TEST_ASSERT(value_approx_eql(val.alt, 1339.685f, 0.1f));
    }
    TEST_END_SECTION();

    TEST_START_SECTION("GEO Distance");
    {
        float val = geo_calculate_distance((geo_position_t){44.532, -72.782}, (geo_position_t){44.544, -72.814});

        TEST_ASSERT(value_approx_eql(val, 2865.85f, 0.02f));
    }
    TEST_END_SECTION();

    TEST_START_SECTION("GEO Bearing");
    {
        float val = geo_calculate_bearing((geo_position_t){44.532, -72.782}, (geo_position_t){44.544, -72.814});

        TEST_ASSERT(value_approx_eql(val, 297.76f, 0.02f));
    }
    TEST_END_SECTION();

    TEST_END();
}