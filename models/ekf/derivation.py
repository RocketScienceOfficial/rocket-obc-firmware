from sympy import *
from utils import *


# ==================== Init ====================


q_w, q_x, q_y, q_z, vel_n, vel_e, vel_d, pos_n, pos_e, pos_d, gyro_x_bias, gyro_y_bias, gyro_z_bias, accel_x_bias, accel_y_bias, accel_z_bias, mag_n, mag_e, mag_d, mag_x_bias, mag_y_bias, mag_z_bias, gyro_x, gyro_y, gyro_z, accel_x, accel_y, accel_z, gps_x, gps_y, gps_z, gps_vel_x, gps_vel_y, baro_height, mag_x, mag_y, mag_z, g, dt = symbols(
    'q_w, q_x, q_y, q_z, vel_n, vel_e, vel_d, pos_n, pos_e, pos_d, gyro_x_bias, gyro_y_bias, gyro_z_bias, accel_x_bias, accel_y_bias, accel_z_bias, mag_n, mag_e, mag_d, mag_x_bias, mag_y_bias, mag_z_bias, gyro_x, gyro_y, gyro_z, accel_x, accel_y, accel_z,gps_x, gps_y, gps_z, gps_vel_x, gps_vel_y, baro_height, mag_x, mag_y, mag_z, g, dt ')

sigma_a_x, sigma_a_y, sigma_a_z, sigma_w_x, sigma_w_y, sigma_w_z, sigma_gps_pos_x, sigma_gps_pos_y, sigma_gps_pos_z, sigma_gps_vel_x, sigma_gps_vel_y, sigma_baro, sigma_mag_x, sigma_mag_y, sigma_mag_z = symbols(
    'sigma_a_x, sigma_a_y, sigma_a_z, sigma_w_x, sigma_w_y, sigma_w_z, sigma_gps_pos_x, sigma_gps_pos_y, sigma_gps_pos_z, sigma_gps_vel_x, sigma_gps_vel_y, sigma_baro, sigma_mag_x, sigma_mag_y, sigma_mag_z')


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
W = Matrix([
    [sigma_a_x, 0, 0, 0, 0, 0],
    [0, sigma_a_y, 0, 0, 0, 0],
    [0, 0, sigma_a_z, 0, 0, 0],
    [0, 0, 0, sigma_w_x, 0, 0],
    [0, 0, 0, 0, sigma_w_y, 0],
    [0, 0, 0, 0, 0, sigma_w_z],
])
Q = G * W * transpose(G)
R = Matrix([
    [sigma_gps_pos_x, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, sigma_gps_pos_y, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, sigma_gps_pos_z, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, sigma_gps_vel_x, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, sigma_gps_vel_y, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, sigma_baro, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, sigma_mag_x, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, sigma_mag_y, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, sigma_mag_z],
])
