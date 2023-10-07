import numpy as np
from utils import *


def generate_scenario_free_fall_parachute(config):
    N = config['N']
    dt = config['dt']
    g = config['g']
    start_height = config['start_height']
    seed = config['seed']
    parachute_factor = config['parachute_factor']
    mag_strength = config['mag_strength']

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
    gps_vel_x = np.tile(0, N)
    gps_vel_y = np.tile(0, N)
    baro_height = start_height + 0.5 * a_z * t ** 2
    mag_x = np.tile(1, N) * mag_strength
    mag_y = np.tile(0, N) * mag_strength
    mag_z = np.tile(0, N) * mag_strength

    noisy_a_x = add_noise(a_x, config['variances']['a_x'], seed)
    noisy_a_y = add_noise(a_y, config['variances']['a_y'], seed)
    noisy_a_z = add_noise(a_z, config['variances']['a_z'], seed)
    noisy_w_x = add_noise(w_x, config['variances']['w_x'], seed)
    noisy_w_y = add_noise(w_y, config['variances']['w_y'], seed)
    noisy_w_z = add_noise(w_z, config['variances']['w_z'], seed)
    noisy_gps_x = add_noise(gps_x, config['variances']['gps_pos_x'], seed)
    noisy_gps_y = add_noise(gps_y, config['variances']['gps_pos_y'], seed)
    noisy_gps_z = add_noise(gps_z, config['variances']['gps_pos_z'], seed)
    noisy_gps_vel_x = add_noise(
        gps_vel_x, config['variances']['gps_vel_x'], seed)
    noisy_gps_vel_y = add_noise(
        gps_vel_y, config['variances']['gps_vel_y'], seed)
    noisy_baro_height = add_noise(
        baro_height, config['variances']['baro'], seed)
    noisy_mag_x = add_noise(mag_x, config['variances']['mag_x'], seed)
    noisy_mag_y = add_noise(mag_y, config['variances']['mag_y'], seed)
    noisy_mag_z = add_noise(mag_z, config['variances']['mag_z'], seed)

    true_measurements = np.vstack(
        (gps_x, gps_y, gps_z, gps_vel_x, gps_vel_y, baro_height, mag_x, mag_y, mag_z))

    noisy_measurements = np.vstack(
        (noisy_gps_x, noisy_gps_y, noisy_gps_z, noisy_gps_vel_x, noisy_gps_vel_y, noisy_baro_height, noisy_mag_x, noisy_mag_y, noisy_mag_z))

    true_controls = np.vstack(
        (a_x, a_y, a_z, w_x, w_y, w_z))

    noisy_controls = np.vstack(
        (noisy_a_x, noisy_a_y, noisy_a_z, noisy_w_x, noisy_w_y, noisy_w_z))

    return true_measurements, noisy_measurements, true_controls, noisy_controls
