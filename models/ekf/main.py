'''
Resources:
    - Docs:
        - PX4 Docs: https://docs.px4.io/main/en/advanced_config/tuning_the_ecl_ekf.html
        - PX4 Models PDF: https://github.com/PX4/PX4-ECL/blob/master/EKF/documentation/Process%20and%20Observation%20Models.pdf
        - AHRS: https://ahrs.readthedocs.io/en/latest/filters/ekf.html
    - Fusion:
        - Matlab Video 1: https://www.youtube.com/watch?v=0rlvvYgmTvI&list=PLn8PRpmsu08ryYoBpEKzoMOveSTyS-h4a&index=3
        - Matlab Video 2: https://www.youtube.com/watch?v=hN8dL55rP5I&list=PLn8PRpmsu08ryYoBpEKzoMOveSTyS-h4a&index=4
        - Stack Exchange Idea: https://dsp.stackexchange.com/questions/60511/kalman-filter-how-to-combine-data-from-sensors-with-different-measurement-rate/60513#60513
        - Matlab functions: https://www.mathworks.com/help/fusion/referencelist.html?type=function&category=inertial-sensor-fusion&s_tid=CRUX_topnav
    - Code:
        - PX4 Derivation 1: https://github.com/PX4/PX4-Autopilot/blob/main/src/modules/ekf2/EKF/python/ekf_derivation
        - PX4 Derivation 2: https://github.com/PX4/PX4-ECL/tree/master/EKF/python/ekf_derivation
        - AP Derivation: https://github.com/ArduPilot/ardupilot/tree/master/libraries/AP_NavEKF3/derivation
'''

import numpy as np
from ekf import *
from scenarios import *
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
    'gps_vel_x': 0.5,
    'gps_vel_y': 0.5,
    'baro': 0.5,
    'mag_x': 0.3,
    'mag_y': 0.3,
    'mag_z': 0.3,
}

init_paramas = {
    'x0': np.array([[1, 0, 0, 0, 0, 0, 0, 0, 0, 1000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]]).T,
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

h_hndl = lambdify([q_w, q_x, q_y, q_z, vel_n, vel_e, vel_d, pos_n, pos_e, pos_d, gyro_x_bias, gyro_y_bias, gyro_z_bias,
                  accel_x_bias, accel_y_bias, accel_z_bias, mag_n, mag_e, mag_d, mag_x_bias, mag_y_bias, mag_z_bias], h)

H_hndl = lambdify([q_w, q_x, q_y, q_z, vel_n, vel_e, vel_d, pos_n, pos_e, pos_d, gyro_x_bias, gyro_y_bias, gyro_z_bias,
                  accel_x_bias, accel_y_bias, accel_z_bias, mag_n, mag_e, mag_d, mag_x_bias, mag_y_bias, mag_z_bias], H)

Q_hndl = lambdify([q_w, q_x, q_y, q_z, vel_n, vel_e, vel_d, pos_n, pos_e, pos_d, gyro_x_bias, gyro_y_bias, gyro_z_bias, accel_x_bias, accel_y_bias,
                  accel_z_bias, mag_n, mag_e, mag_d, mag_x_bias, mag_y_bias, mag_z_bias, sigma_a_x, sigma_a_y, sigma_a_z, sigma_w_x, sigma_w_y, sigma_w_z, dt], Q)

R_hndl = lambdify([sigma_gps_pos_x, sigma_gps_pos_y, sigma_gps_pos_z, sigma_gps_vel_x,
                  sigma_gps_vel_y, sigma_baro, sigma_mag_x, sigma_mag_y, sigma_mag_z], R)


def get_F(state, control):
    state = state[:, 0]

    return np.array(f_hndl(*state, *control, scenario_config['g'], scenario_config['dt'])), np.array(F_hndl(*state, *control, scenario_config['g'], scenario_config['dt']))


def get_H(state):
    state = state[:, 0]

    return np.array(h_hndl(*state)), np.array(H_hndl(*state))


def get_Q(state):
    state = state[:, 0]

    return np.array(Q_hndl(*state, variances['a_x'], variances['a_y'], variances['a_z'], variances['w_x'], variances['w_y'], variances['w_z'], scenario_config['dt']))


def get_R(state):
    state = state[:, 0]

    return np.array(R_hndl(variances['gps_pos_x'], variances['gps_pos_y'], variances['gps_pos_z'], variances['gps_vel_x'], variances['gps_vel_y'], variances['baro'], variances['mag_x'], variances['mag_y'], variances['mag_z']))


true_measurements, noisy_measurements, true_controls, noisy_controls = generate_scenario_free_fall_parachute(
    scenario_config)

X_est, P_est, X_pred, P_pred, K = EKF(
    noisy_measurements, noisy_controls, init_paramas, get_F, get_H, get_Q, get_R)

draw_plots(X_est, P_est, true_measurements, scenario_config['dt'])
