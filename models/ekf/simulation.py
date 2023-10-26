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

config = {
    'dt': 0.01,
    'g': -9.81
}

control_variances = {
    'a_x': 0.1,
    'a_y': 0.1,
    'a_z': 0.1,
    'w_x': 0.1,
    'w_y': 0.1,
    'w_z': 0.1,
}

meas_variances = {
    'gps': 1,
    'baro': 0.5,
    'mag': 0.3,
}

init_params = {
    'x0': np.array([[1, 0, 0, 0, 0, 0, 0, 0, 0, 1000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]]).T,
    'P0': np.eye(22) * 1000,
}

handles = run_derivation(False)

measurements, flags, controls = generate_scenario_free_fall_parachute(1000, config['dt'], config['g'], 1000, control_variances | meas_variances)

n_m = measurements.shape[1]
filt = ExtendedKalmanFilter(init_params['x0'], init_params['P0'], n_m, config['g'], config['dt'], list(control_variances.values()))

# TODO: Test old version, which worked with position with accel; Position with accel oscillates now (Maybe double velocity?)!

for i in range(n_m):
    filt.predict(controls[:, i], handles['f'], handles['F'], handles['Q'])

    current_flags = flags[:, i]
    current_meas = measurements[:, i]

    # filt.correct(current_meas, handles['h'], handles['H'], handles['R'](meas_variances['gps'], meas_variances['baro'], meas_variances['mag']))

    if current_flags[0] == True:
        filt.correct(current_meas[0:3], handles['h_gps'], handles['H_gps'], handles['R_gps'](meas_variances['gps']))

    if current_flags[3] == True:
        filt.correct(current_meas[3], handles['h_baro'], handles['H_baro'], handles['R_baro'](meas_variances['baro']))

    if current_flags[4] == True:
        filt.correct(current_meas[4:7], handles['h_mag'], handles['H_mag'], handles['R_mag'](meas_variances['mag']))

filt.predict(controls[:, i], handles['f'], handles['F'], handles['Q'])

draw_plots(filt.X_est, config['dt'])
