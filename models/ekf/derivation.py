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


def __generate_observation_equations(P, state, H, R):
    n = H.shape[0]
    K = zeros(state.shape[0], n)

    for i in range(n):
        H_temp = H[i, :]
        K_temp = P * H_temp.T * (H_temp * P * H_temp.T + Matrix([R[i, i]])).inv()

        K[:, i] = K_temp

    return H, K


def generate_cov_prediction(P, F, Q):
    length = P.shape[0]
    P_new = F * P * F.T + Q

    for index in range(length):
        for j in range(length):
            if index > j:
                P_new[index, j] = 0

    return P_new


def generate_gps_observation(state, pos):
    obs_var = symbols('R_GPS')

    h = pos
    H = h.jacobian(state)
    R = Matrix.eye(3) * obs_var

    return h, H, R, obs_var


def generate_baro_observation(state, pos_d):
    obs_var = symbols('R_BARO')

    h = Matrix([pos_d])
    H = h.jacobian(state)
    R = Matrix.eye(1) * obs_var

    return h, H, R, obs_var


def generate_mag_observation(state, rot_to_body, mag, mag_bias):
    obs_var = symbols('R_MAG')

    h = rot_to_body * mag + mag_bias
    H = h.jacobian(state)
    R = Matrix.eye(3) * obs_var

    return h, H, R, obs_var


def run_derivation(generate_eqs):
    print("Starting derivation...")

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

    sigma_vector = Matrix([
        sigma_accel_x,
        sigma_accel_y,
        sigma_accel_z,
        sigma_gyro_x,
        sigma_gyro_y,
        sigma_gyro_z,
    ])

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
    pos_new = pos + vel * dt  # + corrected_acc * dt ** 2 / 2
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
    Q = G * Matrix.diag(*sigma_vector) * G.T

    h_gps, H_gps, R_gps, R_gps_var = generate_gps_observation(state_vector, pos)
    h_baro, H_baro, R_baro, R_baro_var = generate_baro_observation(state_vector, pos_d)
    h_mag, H_mag, R_mag, R_mag_var = generate_mag_observation(state_vector, rot_to_body, mag, mag_bias)

    if generate_eqs:
        print("Generating Covariance Prediction...")

        P = __create_symmetric_cov_matrix(state_vector.shape[0])

        write_cov_matrix('covariance_prediction', generate_cov_prediction(P, F, Q))

        print("Generating Observation Equations...")

        write_obs_eqs('gps_fusion', __generate_observation_equations(P, state_vector, H_gps, R_gps))
        write_obs_eqs('baro_fusion', __generate_observation_equations(P, state_vector, H_baro, R_baro))
        write_obs_eqs('mag_fusion', __generate_observation_equations(P, state_vector, H_mag, R_mag))

        print("Done!")

        return None
    else:
        print("Lambdifing functions...")

        functions_handles = {}

        functions_handles['f'] = lambdify([*state_vector, *control_vector, g, dt], f)
        functions_handles['F'] = lambdify([*state_vector, *control_vector, g, dt], F)
        functions_handles['Q'] = lambdify([*state_vector, *sigma_vector, g, dt], Q)
        functions_handles['h_gps'] = lambdify([*state_vector], h_gps)
        functions_handles['H_gps'] = lambdify([*state_vector], H_gps)
        functions_handles['R_gps'] = lambdify([R_gps_var], R_gps)
        functions_handles['h_baro'] = lambdify([*state_vector], h_baro)
        functions_handles['H_baro'] = lambdify([*state_vector], H_baro)
        functions_handles['R_baro'] = lambdify([R_baro_var], R_baro)
        functions_handles['h_mag'] = lambdify([*state_vector], h_mag)
        functions_handles['H_mag'] = lambdify([*state_vector], H_mag)
        functions_handles['R_mag'] = lambdify([R_mag_var], R_mag)

        functions_handles['h'] = lambdify([*state_vector], Matrix([h_gps[0, :], h_gps[1, :], h_gps[2, :], h_baro[0, :], h_mag[0, :], h_mag[1, :], h_mag[2, :]]))
        functions_handles['H'] = lambdify([*state_vector], Matrix([H_gps[0, :], H_gps[1, :], H_gps[2, :], H_baro[0, :], H_mag[0, :], H_mag[1, :], H_mag[2, :]]))
        functions_handles['R'] = lambdify([R_gps_var, R_baro_var, R_mag_var], Matrix.diag(R_gps, R_baro, R_mag))

        print("Done!")

        return functions_handles


if __name__ == '__main__':
    run_derivation(True)
