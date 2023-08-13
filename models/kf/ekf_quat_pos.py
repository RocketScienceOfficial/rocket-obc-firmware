from sympy import *

dt = symbols('dt')


def rotate_trough_quaternion(q, x):
    return Matrix([
        x[0] * (1 - 2 * (q[2] ** 2 + q[3] ** 2)) + x[1] * 2 * (q[1] *
                                                               q[2] - q[0] * q[3]) + x[2] * 2 * (q[1] * q[3] + q[0] * q[2]),
        x[0] * 2 * (q[1] * q[2] + q[0] * q[3]) + x[1] * (1 - 2 *
                                                         (q[1] ** 2 + q[3] ** 2)) + x[2] * 2 * (q[2] * q[3] - q[0] * q[1]),
        x[0] * 2 * (q[1] * q[3] - q[0] * q[2]) + x[1] * 2 * (q[2] *
                                                             q[3] + q[0] * q[1]) + x[2] * (1 - 2 * (q[1] ** 2 + q[2] ** 2))
    ])


def quat_order(order, q, w, dt):
    OMEGA = Matrix([
        [0, -w[0], -w[1], -w[2]],
        [w[0], 0, w[2], -w[1]],
        [w[1], -w[2], 0, -w[0]],
        [w[2], w[1], -w[0], 0]
    ])

    q_temp = Matrix([0, 0, 0, 0])

    for k in range(0, order + 1):
        q_temp += (1 / factorial(k) * (dt / 2 * OMEGA) ** k) * q

    return q_temp


def construct_state():
    x, y, z, x_dot, y_dot, z_dot, q_w, q_x, q_y, q_z, x_ddot, y_ddot, z_ddot, omega_x, omega_y, omega_z, n_a_x, n_a_y, n_a_z, n_m_x, n_m_y, n_m_z, theta, p, t = symbols(
        'x, y, z, x_dot, y_dot, z_dot, q_w, q_x, q_y, q_z, x_ddot, y_ddot, z_ddot, omega_x, omega_y, omega_z, n_a_x, n_a_y, n_a_z, n_m_x, n_m_y, n_m_z, theta, p, t')

    state_vector = Matrix([
        x,
        y,
        z,
        x_dot,
        y_dot,
        z_dot,
        q_w,
        q_x,
        q_y,
        q_z
    ])

    control_vector = Matrix([
        x_ddot,
        y_ddot,
        z_ddot,
        omega_x,
        omega_y,
        omega_z
    ])

    measurement_vector = Matrix([
        n_a_x,
        n_a_y,
        n_a_z,
        n_m_x,
        n_m_y,
        n_m_z,
        x,
        y,
        z,
        p,
        t
    ])

    accel = rotate_trough_quaternion(
        Matrix([q_w, q_x, q_y, q_z]), Matrix([x_ddot, y_ddot, z_ddot]))

    q = quat_order(1, Matrix([q_w, q_x, q_y, q_z]),
                   Matrix([omega_x, omega_y, omega_z]), dt)

    f = Matrix([
        x + x_dot * dt + accel[0] * dt ** 2 / 2,
        y + y_dot * dt + accel[1] * dt ** 2 / 2,
        z + z_dot * dt + accel[2] * dt ** 2 / 2,
        x_dot + accel[0] * dt,
        y_dot + accel[1] * dt,
        z_dot + accel[2] * dt,
        q[0],
        q[1],
        q[2],
        q[3],
    ])

    g = transpose(Matrix([0, 0, -1]))
    r = transpose(Matrix([cos(theta), 0, sin(theta)]))

    a_exp = rotate_trough_quaternion(Matrix([q_w, q_x, q_y, q_z]), g)
    m_exp = rotate_trough_quaternion(Matrix([q_w, q_x, q_y, q_z]), r)

    h = Matrix([
        a_exp[0],
        a_exp[1],
        a_exp[2],
        m_exp[0],
        m_exp[1],
        m_exp[2],
        x,
        y,
        z,
        101325 * (1 - (0.0065 * y) / (t + 273.15)) ** 5.25588,
        (0.0065 * y) / (1 - (p / 101325) ** 0.190284) - 273.15,
    ])

    F = f.jacobian(state_vector)
    G = f.jacobian(control_vector)
    H = h.jacobian(state_vector)

    return (F, G, H, state_vector, control_vector, measurement_vector)


def construct_noises(G):
    sigma_x_ddot, sigma_y_ddot, sigma_z_ddot, sigma_omega_x, sigma_omega_y, sigma_omega_z = symbols(
        'sigma_x_ddot, sigma_y_ddot, sigma_z_ddot, sigma_omega_x, sigma_omega_y, sigma_omega_z')

    sigma_n_a_x, sigma_n_a_y, sigma_n_a_z, sigma_n_m_x, sigma_n_m_y, sigma_n_m_z, sigma_x, sigma_y, sigma_z, sigma_p, sigma_t = symbols(
        'sigma_n_a_x, sigma_n_a_y, sigma_n_a_z, sigma_n_m_x, sigma_n_m_y, sigma_n_m_z, sigma_x, sigma_y, sigma_z, sigma_p, sigma_t')

    SIGMA = Matrix([
        [sigma_x_ddot, 0, 0, 0, 0, 0],
        [0, sigma_y_ddot, 0, 0, 0, 0],
        [0, 0, sigma_z_ddot, 0, 0, 0],
        [0, 0, 0, sigma_omega_x, 0, 0],
        [0, 0, 0, 0, sigma_omega_y, 0],
        [0, 0, 0, 0, 0, sigma_omega_z],
    ])
    R = Matrix([
        [sigma_n_a_x, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, sigma_n_a_y, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, sigma_n_a_z, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, sigma_n_m_x, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, sigma_n_m_y, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, sigma_n_m_z, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, sigma_x, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, sigma_y, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, sigma_z, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, sigma_p, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, sigma_t],
    ])
    Q = integrate(G * SIGMA * transpose(G), (dt, 0, dt))

    for i in range(0, 10):
        print(i, "   ", Q[0, i])

    return (Q, R)


(F, G, H, state_vector, control_vector, measurement_vector) = construct_state()
(Q, R) = construct_noises(G)
