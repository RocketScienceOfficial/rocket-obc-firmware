from sympy import *
from utils import *


# ==================== Utils ====================


def body_to_nav_frame(q):
    return Matrix([
        [q[0] ** 2 + q[1] ** 2 - q[2] ** 2 - q[3] ** 2, 2 *
            (q[1] * q[2] - q[0] * q[3]), 2 * (q[1] * q[3] + q[0] * q[2])],
        [2 * (q[1] * q[2] + q[0] * q[3]), q[0] ** 2 - q[1] ** 2 +
         q[2] ** 2 - q[3] ** 2, 2 * (q[2] * q[3] - q[0] * q[1])],
        [2 * (q[1] * q[3] - q[0] * q[2]), 2 * (q[0] * q[1] + q[2] * q[3]),
         q[0] ** 2 - q[1] ** 2 - q[2] ** 2 + q[3] ** 2],
    ])


def nav_to_body_frame(q):
    return body_to_nav_frame(q).transpose()


# ==================== Init ====================


q_w, q_x, q_y, q_z, vel_n, vel_e, vel_d, pos_n, pos_e, pos_d, gyro_x_bias, gyro_y_bias, gyro_z_bias, accel_x_bias, accel_y_bias, accel_z_bias, mag_n, mag_e, mag_d, mag_x_bias, mag_y_bias, mag_z_bias, gyro_x, gyro_y, gyro_z, accel_x, accel_y, accel_z, gps_x, gps_y, gps_z, gps_vel_x, gps_vel_y, baro_height, mag_x, mag_y, mag_z, g, dt = symbols(
    'q_w, q_x, q_y, q_z, vel_n, vel_e, vel_d, pos_n, pos_e, pos_d, gyro_x_bias, gyro_y_bias, gyro_z_bias, accel_x_bias, accel_y_bias, accel_z_bias, mag_n, mag_e, mag_d, mag_x_bias, mag_y_bias, mag_z_bias, gyro_x, gyro_y, gyro_z, accel_x, accel_y, accel_z,gps_x, gps_y, gps_z, gps_vel_x, gps_vel_y, baro_height, mag_x, mag_y, mag_z, g, dt ')


# ==================== State ====================


state_vector = Matrix([
    q_w,
    q_x,
    q_y,
    q_z,
    vel_n,
    vel_e,
    vel_d,
    pos_n,
    pos_e,
    pos_d,
    gyro_x_bias,
    gyro_y_bias,
    gyro_z_bias,
    accel_x_bias,
    accel_y_bias,
    accel_z_bias,
    mag_n,
    mag_e,
    mag_d,
    mag_x_bias,
    mag_y_bias,
    mag_z_bias,
])

control_vector = Matrix([
    accel_x,
    accel_y,
    accel_z,
    gyro_x,
    gyro_y,
    gyro_z,
])


gyro_truth = Matrix([
    gyro_x - gyro_x_bias,
    gyro_y - gyro_y_bias,
    gyro_z - gyro_z_bias,
])

acc_truth = Matrix([
    accel_x - accel_x_bias,
    accel_y - accel_y_bias,
    accel_z - accel_z_bias,
])

rot_acc = body_to_nav_frame(Matrix([q_w, q_x, q_y, q_z])) * acc_truth
corrected_acc = rot_acc - Matrix([0, 0, g])

d_quat = Matrix([
    1,
    gyro_truth[0] * dt / 2,
    gyro_truth[1] * dt / 2,
    gyro_truth[2] * dt / 2,
])

f = Matrix([
    q_w * d_quat[0] - q_x * d_quat[1] - q_y * d_quat[2] - q_z * d_quat[3],
    q_x * d_quat[0] + q_w * d_quat[1] - q_z * d_quat[2] + q_y * d_quat[3],
    q_y * d_quat[0] + q_z * d_quat[1] + q_w * d_quat[2] - q_x * d_quat[3],
    q_z * d_quat[0] - q_y * d_quat[1] + q_x * d_quat[2] + q_w * d_quat[3],
    vel_n + corrected_acc[0] * dt,
    vel_e + corrected_acc[1] * dt,
    vel_d + corrected_acc[2] * dt,
    pos_n + vel_n * dt + corrected_acc[0] * dt ** 2 / 2,
    pos_e + vel_e * dt + corrected_acc[1] * dt ** 2 / 2,
    pos_d + vel_d * dt + corrected_acc[2] * dt ** 2 / 2,
    gyro_x_bias,
    gyro_y_bias,
    gyro_z_bias,
    accel_x_bias,
    accel_y_bias,
    accel_z_bias,
    mag_n,
    mag_e,
    mag_d,
    mag_x_bias,
    mag_y_bias,
    mag_z_bias,
])


# ==================== Observation ====================


measurement_vector = Matrix([
    gps_x,
    gps_y,
    gps_z,
    gps_vel_x,
    gps_vel_y,
    baro_height,
    mag_x,
    mag_y,
    mag_z,
])

mag = Matrix([mag_n, mag_e, mag_d])
rot_mag = nav_to_body_frame(Matrix([q_w, q_x, q_y, q_z])) * mag

mag_meas = Matrix([
    rot_mag[0] + mag_x_bias,
    rot_mag[1] + mag_y_bias,
    rot_mag[2] + mag_z_bias,
])

h = Matrix([
    pos_n,
    pos_e,
    pos_d,
    vel_n,
    vel_e,
    pos_d,
    mag_meas[0],
    mag_meas[1],
    mag_meas[2],
])


# ==================== Equations ====================


F = f.jacobian(state_vector)
G = f.jacobian(control_vector)
H = h.jacobian(state_vector)


'''
TODO:
    - Accel & Gyro in state
    - GPS to local
    - Coordinates
    - Seperate measurements vectors, jacobians, ...
    - Different sampling rates
    - Same sensors fusion
 

SOURCES:
    - Docs:
        + https://docs.px4.io/main/en/advanced_config/tuning_the_ecl_ekf.html
        + https://ahrs.readthedocs.io/en/latest/filters/ekf.html
        + https://github.com/PX4/PX4-ECL/blob/master/EKF/documentation/Process%20and%20Observation%20Models.pdf
    - Geometry:
        + https://en.wikipedia.org/wiki/Axes_conventions
        + https://en.wikipedia.org/wiki/Local_tangent_plane_coordinates
        + https://en.wikipedia.org/wiki/Geographic_coordinate_conversion
        + https://www.mathworks.com/help/map/choose-a-3-d-coordinate-system.html
        + https://stackoverflow.com/questions/4679876/how-do-i-translate-of-lon-lat-coordinate-by-some-n-e-meters-distance-on-earth-su
        + http://www.movable-type.co.uk/scripts/latlong-vincenty.html#direct
        + https://www.mathworks.com/help/map/ref/geodetic2ned.html
        + https://www.mathworks.com/help/nav/ref/lla2ned.html
        + https://en.wikipedia.org/wiki/Equirectangular_projection        
    - Fusion:
        + https://www.youtube.com/watch?v=0rlvvYgmTvI&list=PLn8PRpmsu08ryYoBpEKzoMOveSTyS-h4a&index=3
        + https://www.youtube.com/watch?v=hN8dL55rP5I&list=PLn8PRpmsu08ryYoBpEKzoMOveSTyS-h4a&index=4
        + https://dsp.stackexchange.com/questions/60511/kalman-filter-how-to-combine-data-from-sensors-with-different-measurement-rate/60513#60513
    - Code:
        + https://github.com/PX4/PX4-Autopilot/blob/main/src/modules/ekf2/EKF/python/ekf_derivation/derivation.py
        + https://github.com/PX4/PX4-Autopilot/blob/main/src/modules/ekf2/EKF/gps_control.cpp
'''
