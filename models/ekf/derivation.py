from sympy import *
from code_gen import *


def __quat_to_rot(q):
    q0 = q[0]
    q1 = q[1]
    q2 = q[2]
    q3 = q[3]

    return Matrix([[1 - 2*(q2**2 + q3**2), 2*(q1*q2 - q0*q3), 2*(q1*q3 + q0*q2)],
                   [2*(q1*q2 + q0*q3), 1 - 2*(q1**2 + q3**2), 2*(q2*q3 - q0*q1)],
                   [2*(q1*q3-q0*q2), 2*(q2*q3 + q0*q1), 1 - 2*(q1**2 + q2**2)]])


def __quat_mult(p, q):
    r = Matrix([p[0] * q[0] - p[1] * q[1] - p[2] * q[2] - p[3] * q[3],
                p[0] * q[1] + p[1] * q[0] + p[2] * q[3] - p[3] * q[2],
                p[0] * q[2] - p[1] * q[3] + p[2] * q[0] + p[3] * q[1],
                p[0] * q[3] + p[1] * q[2] - p[2] * q[1] + p[3] * q[0]])

    return r


def __create_cov_matrix(i, j):
    if j >= i:
        return Symbol("P[" + str(i) + "][" + str(j) + "]", real=True)
    else:
        return 0


def __create_symmetric_cov_matrix(n):
    P = Matrix(n, n, __create_cov_matrix)

    for index in range(n):
        for j in range(n):
            if index > j:
                P[index, j] = P[j, index]

    return P


def __print_matrix(m: Matrix):
    for i in range(m.shape[0]):
        for j in range(m.shape[1]):
            print(m[i, j], end=', ')

        print("")


def __generate_observation_equations(P, state, h, R, n):
    H = zeros(n, state.shape[0])
    K = zeros(state.shape[0], n)

    for i in range(n):
        H_temp = Matrix([h[i]]).jacobian(state)
        K_temp = P * H_temp.T * (H_temp * P * H_temp.T + Matrix([R])).inv()

        H[i, :] = H_temp
        K[:, i] = K_temp

    return H, K


def generate_cov_propagation(P, F, G, G_var):
    length = P.shape[0]
    P_new = F * P * F.T + G * G_var * G.T

    for index in range(length):
        for j in range(length):
            if index > j:
                P_new[index, j] = 0

    return P_new


def generate_gps_observation(P, state, pos):
    obs_var = symbols('R_GPS')
    h = pos

    return __generate_observation_equations(P, state, h, obs_var, 3)


def generate_baro_observation(P, state, pos_d):
    obs_var = symbols('R_BARO')
    h = Matrix([pos_d])

    return __generate_observation_equations(P, state, h, obs_var, 1)


def generate_mag_observation(P, state, rot_to_body, mag, mag_bias):
    obs_var = symbols('R_MAG')
    h = rot_to_body * mag + mag_bias

    return __generate_observation_equations(P, state, h, obs_var, 3)


def generate_code():
    print("Starting...")

    dt, g = symbols('dt, g')

    print("Setting State Vector...")

    q_w, q_x, q_y, q_z = symbols('q_w, q_x, q_y, q_z')
    q = Matrix([q_w, q_x, q_y, q_z])
    rot_to_earth = __quat_to_rot(q)
    rot_to_body = rot_to_earth.T

    vel_n, vel_e, vel_d = symbols('vel_n, vel_e, vel_d')
    vel = Matrix([vel_n, vel_e, vel_d])

    pos_n, pos_e, pos_d = symbols('pos_n, pos_e, pos_d')
    pos = Matrix([pos_n, pos_e, pos_d])

    gyro_x_bias, gyro_y_bias, gyro_z_bias = symbols('gyro_x_bias, gyro_y_bias, gyro_z_bias')
    gyro_bias = Matrix([gyro_x_bias, gyro_y_bias, gyro_z_bias])

    accel_x_bias, accel_y_bias, accel_z_bias = symbols('accel_x_bias, accel_y_bias, accel_z_bias')
    accel_bias = Matrix([accel_x_bias, accel_y_bias, accel_z_bias])

    mag_n, mag_e, mag_d = symbols('mag_n, mag_e, mag_d')
    mag = Matrix([mag_n, mag_e, mag_d])

    mag_x_bias, mag_y_bias, mag_z_bias = symbols('mag_x_bias, mag_y_bias, mag_z_bias')
    mag_bias = Matrix([mag_x_bias, mag_y_bias, mag_z_bias])

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

    print("Setting Control Vector...")

    gyro_x, gyro_y, gyro_z = symbols('gyro_x, gyro_y, gyro_z')
    gyro = Matrix([gyro_x, gyro_y, gyro_z])

    accel_x, accel_y, accel_z = symbols('accel_x, accel_y, accel_z')
    accel = Matrix([accel_x, accel_y, accel_z])

    sigma_accel_x, sigma_accel_y, sigma_accel_z = symbols('sigma_accel_x, sigma_accel_y, sigma_accel_z')
    sigma_gyro_x, sigma_gyro_y, sigma_gyro_z = symbols('sigma_gyro_x, sigma_gyro_y, sigma_gyro_z')

    control_vector = Matrix([
        accel_x,
        accel_y,
        accel_z,
        gyro_x,
        gyro_y,
        gyro_z,
    ])

    print("Computing Transition Function...")

    gyro_truth = gyro - gyro_bias
    acc_truth = accel - accel_bias
    corrected_acc = rot_to_earth * acc_truth + Matrix([0, 0, g])
    q_new = __quat_mult(q, Matrix([1, gyro_truth[0] * dt / 2, gyro_truth[1] * dt / 2, gyro_truth[2] * dt / 2]))
    vel_new = vel + corrected_acc * dt
    pos_new = pos + vel * dt + corrected_acc * dt ** 2 / 2
    gyro_bias_new = gyro_bias
    accel_bias_new = accel_bias
    mag_new = mag
    mag_bias_new = mag_bias

    f = Matrix([
        q_new,
        vel_new,
        pos_new,
        gyro_bias_new,
        accel_bias_new,
        mag_new,
        mag_bias_new,
    ])

    print("Computing Jacobians...")

    F = f.jacobian(state_vector)
    G = f.jacobian(control_vector)
    G_var = Matrix.diag(sigma_accel_x, sigma_accel_y, sigma_accel_z, sigma_gyro_x, sigma_gyro_y, sigma_gyro_z)
    P = __create_symmetric_cov_matrix(state_vector.shape[0])

    print("Generating Covariance Propagation...")

    write_cov_matrix('covariance_prediction', generate_cov_propagation(P, F, G, G_var))

    print("Generating Observation Equations...")

    write_obs_eqs('gps_fusion', generate_gps_observation(P, state_vector, pos))
    write_obs_eqs('baro_fusion', generate_baro_observation(P, state_vector, pos_d))
    write_obs_eqs('mag_fusion', generate_mag_observation(P, state_vector, rot_to_body, mag, mag_bias))


if __name__ == '__main__':
    generate_code()
