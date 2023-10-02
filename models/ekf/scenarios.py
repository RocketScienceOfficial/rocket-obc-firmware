import numpy as np
from utils import *


def generate_scenario_free_fall_parachute():
    N = 1000
    dt = 0.01
    g = -9.81
    start_height = 10000
    seed = 0
    parachute_factor = 0.5
    mag_strength = 0.5

    a_x = np.tile(0, N)
    a_y = np.tile(0, N)
    a_z = np.tile(g * parachute_factor, N)
    w_x = np.tile(0, N)
    w_y = np.tile(0, N)
    w_z = np.tile(0, N)
    gps_x = np.tile(0, N)
    gps_y = np.tile(0, N)
    gps_z = start_height - 0.5 * a_z * dt ** 2
    gps_vel_x = np.tile(0, N)
    gps_vel_y = np.tile(0, N)
    baro_height = start_height - 0.5 * a_z * dt ** 2
    mag_x = np.tile(1, N) * mag_strength
    mag_y = np.tile(0, N) * mag_strength
    mag_z = np.tile(0, N) * mag_strength

    noisy_a_x = add_noise(a_x, 0.001, seed)
    noisy_a_y = add_noise(a_y, 0.001, seed)
    noisy_a_z = add_noise(a_z, 0.001, seed)
    noisy_w_x = add_noise(w_x, 0.001, seed)
    noisy_w_y = add_noise(w_y, 0.001, seed)
    noisy_w_z = add_noise(w_z, 0.001, seed)
    noisy_gps_x = add_noise(gps_x, 0.1, seed)
    noisy_gps_y = add_noise(gps_y, 0.1, seed)
    noisy_gps_z = add_noise(gps_z, 0.1, seed)
    noisy_gps_vel_x = add_noise(gps_vel_x, 0.07, seed)
    noisy_gps_vel_y = add_noise(gps_vel_y, 0.07, seed)
    noisy_baro_height = add_noise(baro_height, 0.15, seed)
    noisy_mag_x = add_noise(mag_x, 0.01, seed)
    noisy_mag_y = add_noise(mag_y, 0.01, seed)
    noisy_mag_z = add_noise(mag_z, 0.01, seed)

    true_measurements = np.vstack(
        (gps_x, gps_y, gps_z, gps_vel_x, gps_vel_y, baro_height, mag_x, mag_y, mag_z))

    noisy_measurements = np.vstack(
        (noisy_gps_x, noisy_gps_y, noisy_gps_z, noisy_gps_vel_x, noisy_gps_vel_y, noisy_baro_height, noisy_mag_x, noisy_mag_y, noisy_mag_z))

    true_controls = np.vstack(
        (a_x, a_y, a_z, w_x, w_y, w_z))

    noisy_controls = np.vstack(
        (noisy_a_x, noisy_a_y, noisy_a_z, noisy_w_x, noisy_w_y, noisy_w_z))

    Z = np.array([
        true_measurements, noisy_measurements, true_controls, noisy_controls
    ], dtype=object)

    return Z


def generate_scenario_free_fall_rotating():
    pass


def generate_scenerio_vertical_ascent():
    pass


def generate_scenario_circular_movement():
    pass
