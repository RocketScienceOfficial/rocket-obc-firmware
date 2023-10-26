import numpy as np
import csv
import os
from geo import *


def __add_noise(X, sigma):
    np.random.seed(1)
    noise = np.random.normal(0, sigma ** 0.5, X.shape[0])

    return X + noise


def __parse_csv(filename, delim=';'):
    path = os.path.dirname(os.path.abspath(__file__))
    file_path = os.path.join(path, "data", filename)
    is_header = True
    data = []
    df_len = -1

    with open(file_path) as file:
        reader = csv.reader(file, delimiter=delim)

        for row in reader:
            if not is_header:
                row.pop()

                if len(row) == df_len:
                    for i in range(len(row)):
                        try:
                            row[i] = float(row[i])
                        except:
                            row[i] = 0

                    data.append(row)
            else:
                is_header = False
                df_len = len(row)

    return data


def get_SAC_data():
    data = __parse_csv('DATA30.TXT')
    data_len = len(data)

    a_x = np.array(data)[:, 16]
    a_y = np.array(data)[:, 17]
    a_z = np.array(data)[:, 18]
    w_x = np.array(data)[:, 19]
    w_y = np.array(data)[:, 20]
    w_z = np.array(data)[:, 21]
    gps_x = np.array(data)[:, 12]
    gps_y = np.array(data)[:, 13]
    gps_z = np.array(data)[:, 14]
    baro_height = np.array(data)[:, 27]
    mag_x = np.array(data)[:, 22] * 100000
    mag_y = np.array(data)[:, 23] * 100000
    mag_z = np.array(data)[:, 24] * 100000

    gps_flag = np.tile(1, data_len)

    for i in range(len(gps_x)):
        if gps_x[i] == 0:
            gps_flag[i] = 0
        else:
            [gps_x[i], gps_y[i], gps_z[i]] = geo_to_ned(
                32.951805, -106.915802, 137.073196, gps_x[i], gps_y[i], gps_z[i])

        gps_flag[i] = False

    true_flag = np.tile(True, data_len)

    measurements = np.vstack(
        (gps_x, gps_y, gps_z, baro_height, mag_x, mag_y, mag_z))

    measurements_flags = np.vstack(
        (gps_flag, gps_flag, gps_flag, true_flag, true_flag, true_flag, true_flag))

    controls = np.vstack((a_x, a_y, a_z, w_x, w_y, w_z))

    return measurements, measurements_flags, controls


def generate_scenario_free_fall_parachute(n, dt, g, start_height, variances):
    N = n
    parachute_factor = 0.5
    mag_strength = 0.5

    t = np.arange(0, dt * N, dt)

    a_x = np.tile(0, N)
    a_y = np.tile(0, N)
    a_z = np.tile(g * parachute_factor, N)
    w_x = np.tile(0, N)
    w_y = np.tile(0, N)
    w_z = np.tile(0, N)
    gps_x = np.tile(0, N)
    gps_y = np.tile(0, N)
    gps_z = start_height + 0.5 * a_z * t ** 2
    baro_height = start_height + 0.5 * a_z * t ** 2
    mag_x = np.tile(1, N) * mag_strength
    mag_y = np.tile(0, N) * mag_strength
    mag_z = np.tile(0, N) * mag_strength

    a_z *= -1

    noisy_a_x = __add_noise(a_x, variances['a_x'])
    noisy_a_y = __add_noise(a_y, variances['a_y'])
    noisy_a_z = __add_noise(a_z, variances['a_z'])
    noisy_w_x = __add_noise(w_x, variances['w_x'])
    noisy_w_y = __add_noise(w_y, variances['w_y'])
    noisy_w_z = __add_noise(w_z, variances['w_z'])
    noisy_gps_x = __add_noise(gps_x, variances['gps'])
    noisy_gps_y = __add_noise(gps_y, variances['gps'])
    noisy_gps_z = __add_noise(gps_z, variances['gps'])
    noisy_baro_height = __add_noise(baro_height, variances['baro'])
    noisy_mag_x = __add_noise(mag_x, variances['mag'])
    noisy_mag_y = __add_noise(mag_y, variances['mag'])
    noisy_mag_z = __add_noise(mag_z, variances['mag'])

    true_measurements = np.vstack(
        (gps_x, gps_y, gps_z, baro_height, mag_x, mag_y, mag_z))

    noisy_measurements = np.vstack(
        (noisy_gps_x, noisy_gps_y, noisy_gps_z, noisy_baro_height, noisy_mag_x, noisy_mag_y, noisy_mag_z))

    true_controls = np.vstack(
        (a_x, a_y, a_z, w_x, w_y, w_z))

    noisy_controls = np.vstack(
        (noisy_a_x, noisy_a_y, noisy_a_z, noisy_w_x, noisy_w_y, noisy_w_z))

    false_flag = np.tile(False, N)
    true_flag = np.tile(True, N)

    measurements_flags = np.vstack(
        (true_flag, true_flag, true_flag, true_flag, true_flag, true_flag, true_flag))

    return noisy_measurements, measurements_flags, noisy_controls
