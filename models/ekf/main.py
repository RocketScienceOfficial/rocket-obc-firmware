'''
Resources:
    - Docs:
        - PX4 Docs: https://docs.px4.io/main/en/advanced_config/tuning_the_ecl_ekf.html
        - PX4 Models PDF: https://github.com/PX4/PX4-ECL/blob/master/EKF/documentation/Process%20and%20Observation%20Models.pdf
       - AHRS: https://ahrs.readthedocs.io/en/latest/filters/ekf.html
'''

import numpy as np
from ekf import *
from data_provider import *
from derivation import *
from analysis import *

variances = {
    'a_x': 0.1,
    'a_y': 0.1,
    'a_z': 0.1,
    'w_x': 0.1,
    'w_y': 0.1,
    'w_z': 0.1,
    'gps_pos_x': 1,
    'gps_pos_y': 1,
    'gps_pos_z': 1,
    'baro': 0.5,
    'mag_x': 0.3,
    'mag_y': 0.3,
    'mag_z': 0.3,
}

init_paramas = {
    'x0': np.array([[1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]]).T,
    'P0': np.eye(22) * 1000,
}

scenario_config = {
    'N': 1000,
    'dt': 0.01,
    'g': -9.81,
    'start_height': init_paramas['x0'][9, 0],
    'seed': 0,
    'parachute_factor': 0.5,
    'mag_strength': 0.5,
    'variances': variances
}

f_hndl = lambdify([q_w, q_x, q_y, q_z, vel_n, vel_e, vel_d, pos_n, pos_e, pos_d, gyro_x_bias, gyro_y_bias, gyro_z_bias, accel_x_bias, accel_y_bias,
                  accel_z_bias, mag_n, mag_e, mag_d, mag_x_bias, mag_y_bias, mag_z_bias, accel_x, accel_y, accel_z, gyro_x, gyro_y, gyro_z, g, dt], f)

F_hndl = lambdify([q_w, q_x, q_y, q_z, vel_n, vel_e, vel_d, pos_n, pos_e, pos_d, gyro_x_bias, gyro_y_bias, gyro_z_bias, accel_x_bias, accel_y_bias,
                  accel_z_bias, mag_n, mag_e, mag_d, mag_x_bias, mag_y_bias, mag_z_bias, accel_x, accel_y, accel_z, gyro_x, gyro_y, gyro_z, g, dt], F)

h_gps_hndl = lambdify([q_w, q_x, q_y, q_z, vel_n, vel_e, vel_d, pos_n, pos_e, pos_d, gyro_x_bias, gyro_y_bias, gyro_z_bias,
                      accel_x_bias, accel_y_bias, accel_z_bias, mag_n, mag_e, mag_d, mag_x_bias, mag_y_bias, mag_z_bias], h_gps)
h_baro_hndl = lambdify([q_w, q_x, q_y, q_z, vel_n, vel_e, vel_d, pos_n, pos_e, pos_d, gyro_x_bias, gyro_y_bias, gyro_z_bias,
                       accel_x_bias, accel_y_bias, accel_z_bias, mag_n, mag_e, mag_d, mag_x_bias, mag_y_bias, mag_z_bias], h_baro)
h_mag_hndl = lambdify([q_w, q_x, q_y, q_z, vel_n, vel_e, vel_d, pos_n, pos_e, pos_d, gyro_x_bias, gyro_y_bias, gyro_z_bias,
                      accel_x_bias, accel_y_bias, accel_z_bias, mag_n, mag_e, mag_d, mag_x_bias, mag_y_bias, mag_z_bias], h_mag)
H_gps_hndl = lambdify([q_w, q_x, q_y, q_z, vel_n, vel_e, vel_d, pos_n, pos_e, pos_d, gyro_x_bias, gyro_y_bias, gyro_z_bias,
                      accel_x_bias, accel_y_bias, accel_z_bias, mag_n, mag_e, mag_d, mag_x_bias, mag_y_bias, mag_z_bias], H_gps)
H_baro_hndl = lambdify([q_w, q_x, q_y, q_z, vel_n, vel_e, vel_d, pos_n, pos_e, pos_d, gyro_x_bias, gyro_y_bias, gyro_z_bias,
                       accel_x_bias, accel_y_bias, accel_z_bias, mag_n, mag_e, mag_d, mag_x_bias, mag_y_bias, mag_z_bias], H_baro)
H_mag_hndl = lambdify([q_w, q_x, q_y, q_z, vel_n, vel_e, vel_d, pos_n, pos_e, pos_d, gyro_x_bias, gyro_y_bias, gyro_z_bias,
                      accel_x_bias, accel_y_bias, accel_z_bias, mag_n, mag_e, mag_d, mag_x_bias, mag_y_bias, mag_z_bias], H_mag)

Q_hndl = lambdify([q_w, q_x, q_y, q_z, vel_n, vel_e, vel_d, pos_n, pos_e, pos_d, gyro_x_bias, gyro_y_bias, gyro_z_bias, accel_x_bias, accel_y_bias,
                  accel_z_bias, mag_n, mag_e, mag_d, mag_x_bias, mag_y_bias, mag_z_bias, sigma_a_x, sigma_a_y, sigma_a_z, sigma_w_x, sigma_w_y, sigma_w_z, dt], Q)

R_gps_hndl = lambdify([sigma_gps_pos_x, sigma_gps_pos_y, sigma_gps_pos_z,
                      sigma_baro, sigma_mag_x, sigma_mag_y, sigma_mag_z], R_gps)
R_baro_hndl = lambdify([sigma_gps_pos_x, sigma_gps_pos_y, sigma_gps_pos_z,
                       sigma_baro, sigma_mag_x, sigma_mag_y, sigma_mag_z], R_baro)
R_mag_hndl = lambdify([sigma_gps_pos_x, sigma_gps_pos_y, sigma_gps_pos_z,
                      sigma_baro, sigma_mag_x, sigma_mag_y, sigma_mag_z], R_mag)


def get_F(state, control):
    new_state = state[:, 0]

    return np.array(f_hndl(*new_state, *control, scenario_config['g'], scenario_config['dt'])), np.array(F_hndl(*new_state, *control, scenario_config['g'], scenario_config['dt']))


def get_H(state, meas, flags):
    new_state = state[:, 0]

    arr = []

    if flags[0] == 1:
        arr.append((np.array(h_gps_hndl(*new_state)),
                   np.array(H_gps_hndl(*new_state)), meas[0:3]))

    if flags[3] == 1:
        arr.append((np.array(h_baro_hndl(*new_state)),
                   np.array(H_baro_hndl(*new_state)), meas[3]))

    if flags[4] == 1:
        arr.append((np.array(h_mag_hndl(*new_state)),
                   np.array(H_mag_hndl(*new_state)), meas[4:7]))

    return arr


def get_Q(state):
    new_state = state[:, 0]

    return np.array(Q_hndl(*new_state, variances['a_x'], variances['a_y'], variances['a_z'], variances['w_x'], variances['w_y'], variances['w_z'], scenario_config['dt']))


def get_R(flags):
    arr = []

    if flags[0] == 1:
        arr.append(np.array(R_gps_hndl(variances['gps_pos_x'], variances['gps_pos_y'], variances['gps_pos_z'],
                   variances['baro'], variances['mag_x'], variances['mag_y'], variances['mag_z'])))

    if flags[3] == 1:
        arr.append(np.array(R_baro_hndl(variances['gps_pos_x'], variances['gps_pos_y'], variances['gps_pos_z'],
                   variances['baro'], variances['mag_x'], variances['mag_y'], variances['mag_z'])))

    if flags[4] == 1:
        arr.append(np.array(R_mag_hndl(variances['gps_pos_x'], variances['gps_pos_y'], variances['gps_pos_z'],
                   variances['baro'], variances['mag_x'], variances['mag_y'], variances['mag_z'])))

    return arr


measurements, flags, controls = get_SAC_data()

X_est, P_est, X_pred, P_pred = EKF(
    measurements, flags, controls, init_paramas, get_F, get_H, get_Q, get_R)

draw_plots(X_est, scenario_config['dt'])
