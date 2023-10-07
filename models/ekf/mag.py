'''
Resources:
    - https://github.com/PX4/PX4-ECL/blob/master/geo_lookup/fetch_noaa_table.py
    - https://www.ngdc.noaa.gov/geomag/CalcSurveyFin.shtml
'''

import math
import json
import urllib.request

BASE_URL = "https://www.ngdc.noaa.gov/geomag-web/calculators/calculateIgrfgrid"
KEY = "gFE5W"

SAMPLING_RES = 10
SAMPLING_MIN_LAT = -90
SAMPLING_MAX_LAT = 90
SAMPLING_MIN_LON = -180
SAMPLING_MAX_LON = 180

LAT_DIM = int((SAMPLING_MAX_LAT - SAMPLING_MIN_LAT) / SAMPLING_RES) + 1
LON_DIM = int((SAMPLING_MAX_LON - SAMPLING_MIN_LON) / SAMPLING_RES) + 1


def get_data(component, key):
    table = []
    index = 0

    for latitude in range(SAMPLING_MIN_LAT, SAMPLING_MAX_LAT + 1, SAMPLING_RES):
        params = urllib.parse.urlencode({'key': KEY, 'lat1': latitude, 'lat2': latitude, 'lon1': SAMPLING_MIN_LON, 'lon2': SAMPLING_MAX_LON,
                                        'latStepSize': 1, 'lonStepSize': SAMPLING_RES, 'magneticComponent': component, 'resultFormat': 'json'})

        f = urllib.request.urlopen(BASE_URL + "?%s" % params)
        data = json.loads(f.read())

        table.append([])

        for p in data['result']:
            table[index].append(p[key])

        index += 1

    return table


def get_declination():
    return get_data('d', 'declination')


def get_inclination():
    return get_data('i', 'inclination')


def get_strength():
    return get_data('f', 'totalintensity')


def get_value_from_table(lat, lon, table):
    min_lat = math.floor(lat / SAMPLING_RES) * SAMPLING_RES
    min_lon = math.floor(lon / SAMPLING_RES) * SAMPLING_RES

    min_lat_index = int((min_lat - SAMPLING_MIN_LAT) / SAMPLING_RES)
    min_lon_index = int((min_lon - SAMPLING_MIN_LON) / SAMPLING_RES)

    data_ne = table[min_lat_index + 1][min_lon_index + 1]
    data_se = table[min_lat_index][min_lon_index + 1]
    data_sw = table[min_lat_index][min_lon_index]
    data_nw = table[min_lat_index + 1][min_lon_index]

    data_min = (data_se - data_sw) / SAMPLING_RES * (lon - min_lon) + data_sw
    data_max = (data_ne - data_nw) / SAMPLING_RES * (lon - min_lon) + data_nw
    data = (data_max - data_min) / SAMPLING_RES * (lat - min_lat) + data_min

    return data
