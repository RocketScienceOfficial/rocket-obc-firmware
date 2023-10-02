'''
Resources:
    - https://www.mathworks.com/help/map/ref/geodetic2ned.html
    - https://www.mathworks.com/help/nav/ref/lla2ned.html
    - https://www.mathworks.com/help/map/ref/ned2geodetic.html
    - https://www.mathworks.com/help/nav/ref/ned2lla.html
    - https://en.wikipedia.org/wiki/Geographic_coordinate_conversion
    - https://en.wikipedia.org/wiki/Local_tangent_plane_coordinates
    
    
    TODO: Check algorithm for 'ecef_to_geo' function (https://www.mathworks.com/help/aeroblks/ecefpositiontolla.html)
'''

from math import pi, sin, cos, sqrt, atan, atan2, asin, radians, degrees


a = 6378137.0
b = 6356752.3142
e = 8.1819190842622e-2
e2 = 6.69437999014e-3
f = 1 / 298.257223563


def __convert_coords_to_rad(coords_list):
    new_coords = []

    for x in coords_list:
        new_coords.append(radians(x))

    return new_coords


def __convert_coords_to_deg(coords_list):
    new_coords = []

    for x in coords_list:
        new_coords.append(degrees(x))

    return new_coords


def __wrap_to_pi(x):
    return (x + 2 * pi) % pi


def __wrap_to_2pi(x):
    return __wrap_to_pi(x) + pi


def geo_to_ecef(lat, lon, alt):
    n = a / sqrt(1 - e2 * sin(lat) ** 2)

    x = (n + alt) * cos(lat) * cos(lon)
    y = (n + alt) * cos(lat) * sin(lon)
    z = ((1 - e2) * n + alt) * sin(lat)

    return [x, y, z]


def ecef_to_geo(x, y, z):
    # lon = atan2(y, x)

    # p = sqrt(x ** 2 + y ** 2)
    # lat = atan2(z, p * (1 - e2))
    # alt = 0
    # err = 1

    # while err > 1e-10:
    #     n = a / sqrt(1 - e2 * sin(lat) ** 2)

    #     new_lat = atan2(z + e2 * n * sin(lat), p)
    #     alt = p / cos(lat) - n

    #     err = new_lat - lat
    #     lat = new_lat

    # return [lat, lon, alt]

    lon = atan2(y, x)

    s = sqrt(x ** 2 + y ** 2)

    beta = atan(z / ((1 - f) * s))
    miu = atan((z + e2 * (1 - f) / (1 - e2) * a * sin(beta)
               ** 3) / (s - e2 * a * cos(beta) ** 3))
    err = 1e10

    while err > 1e-10:
        beta = atan((1 - f) * sin(miu) / cos(miu))
        last_miu = miu
        miu = atan((z + e2 * (1 - f) / (1 - e2) * a * sin(beta)
                   ** 3) / (s - e2 * a * cos(beta) ** 3))
        err = last_miu - miu

    n = a / sqrt(1 - e2 * sin(miu) ** 2)
    h = s * cos(miu) + (z + e2 * n * sin(miu)) * sin(miu) - n

    return [miu, lon, h]


def ecef_to_ned(x, y, z, lat0, lon0, alt0):
    [x0, y0, z0] = geo_to_ecef(lat0, lon0, alt0)

    dx = x - x0
    dy = y - y0
    dz = z - z0

    x_ned = dx * (-sin(lat0) * cos(lon0)) + dy * \
        (-sin(lat0) * sin(lon0)) + dz * (cos(lat0))
    y_ned = dx * (-sin(lon0)) + dy * (cos(lon0)) + dz * (0)
    z_ned = dx * (-cos(lat0) * cos(lon0)) + dy * \
        (-cos(lat0) * sin(lon0)) + dz * (-sin(lat0))

    return [x_ned, y_ned, z_ned]


def ned_to_ecef(x, y, z, lat0, lon0, alt0):
    [x0, y0, z0] = geo_to_ecef(lat0, lon0, alt0)

    x_ecef = x * (-sin(lat0) * cos(lon0)) + y * \
        (-sin(lon0)) + z * (-cos(lat0) * cos(lon0))
    y_ecef = x * (-sin(lat0) * sin(lon0)) + y * \
        (cos(lon0)) + z * (-cos(lat0) * sin(lon0))
    z_ecef = x * (cos(lat0)) + y * (0) + z * (-sin(lat0))

    x_ecef += x0
    y_ecef += y0
    z_ecef += z0

    return [x_ecef, y_ecef, z_ecef]


def geo_to_ned(lat0, lon0, alt0, lat1, lon1, alt1):
    [lat0, lon0, lat1, lon1] = __convert_coords_to_rad(
        [lat0, lon0, lat1, lon1])

    [x1, y1, z1] = geo_to_ecef(lat1, lon1, alt1)
    [x, y, z] = ecef_to_ned(x1, y1, z1, lat0, lon0, alt0)

    return [x, y, z]


def ned_to_geo(lat0, lon0, alt0, x, y, z):
    [lat0, lon0] = __convert_coords_to_rad([lat0, lon0])

    [x_ecef, y_ecef, z_ecef] = ned_to_ecef(x, y, z, lat0, lon0, alt0)
    [lat, lon, alt] = ecef_to_geo(x_ecef, y_ecef, z_ecef)
    [lat, lon] = __convert_coords_to_deg([lat, lon])

    return [lat, lon, alt]


#print(ecef_to_geo(4201000, 172460, 4780100))
print(ned_to_geo(44.532, -72.782, 1699, 1334.3, -2543.6, 359.65))


def geo_distance(lat0, lon0, lat1, lon1):
    [lat0, lon0, lat1, lon1] = __convert_coords_to_rad(
        [lat0, lon0, lat1, lon1])

    d_lat = lat1 - lat0
    d_lon = lon1 - lon0

    a = sin(d_lat / 2) * sin(d_lat / 2) + sin(d_lon / 2) * \
        sin(d_lon / 2) * cos(lat0) * cos(lat1)
    c = atan2(sqrt(a), sqrt(1 - a))
    d = 2 * a * c

    return d


def geo_bearing(lat0, lon0, lat1, lon1):
    [lat0, lon0, lat1, lon1] = __convert_coords_to_rad(
        [lat0, lon0, lat1, lon1])

    cos_lat1 = cos(lat1)
    d_lon = lon1 - lon0

    y = sin(d_lon) * cos_lat1
    x = cos(lat0) * sin(lat1) - \
        sin(lat0) * cos_lat1 * cos(d_lon)
    theta = atan2(y, x)
    bearing = degrees(__wrap_to_pi(theta))

    return bearing


def geo_point_from_bearing_and_distance(lat, lon, bearing, distance):
    [lat, lon] = __convert_coords_to_rad([lat, lon])

    bearing = radians(bearing)
    bearing = __wrap_to_2pi(bearing)
    radius_ratio = distance / a

    lat_target = asin(sin(lat) * cos(radius_ratio) + cos(lat)
                      * sin(radius_ratio) * cos(bearing))
    lon_target = lon + atan2(sin(bearing) * sin(radius_ratio)
                             * cos(lat), cos(radius_ratio) - sin(lat) * sin(lat_target))

    return [lat_target, lon_target]
