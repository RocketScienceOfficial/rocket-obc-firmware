from sympy import *


def quat_order(order, q, w, dt):
    OMEGA = Matrix([
        [0, -w[0], -w[1], -w[2]],
        [w[0], 0, w[2], -w[1]],
        [w[1], -w[2], 0, -w[0]],
        [w[2], w[1], -w[0], 0]
    ])

    q_temp = Matrix([0, 0, 0, 0])

    for k in range(0, order + 1):
        q_temp += (dt ** k / factorial(k)) * OMEGA ** k * q

    return q_temp


def rotate_trough_quaternion(q, x):
    return Matrix([
        x[0] * (1 - 2 * (q[2] ** 2 + q[3] ** 2)) + x[1] * 2 * (q[1] *
                                                               q[2] - q[0] * q[3]) + x[2] * 2 * (q[1] * q[3] + q[0] * q[2]),
        x[0] * 2 * (q[1] * q[2] + q[0] * q[3]) + x[1] * (1 - 2 *
                                                         (q[1] ** 2 + q[3] ** 2)) + x[2] * 2 * (q[2] * q[3] - q[0] * q[1]),
        x[0] * 2 * (q[1] * q[3] - q[0] * q[2]) + x[1] * 2 * (q[2] *
                                                             q[3] + q[0] * q[1]) + x[2] * (1 - 2 * (q[1] ** 2 + q[2] ** 2))
    ])


x, y, z, x_dot, y_dot, z_dot, q_w, q_x, q_y, q_z, x_ddot, y_ddot, z_ddot, omega_x, omega_y, omega_z, n_a_x, n_a_y, n_a_z, n_m_x, n_m_y, n_m_z, theta, p, t, dt = symbols(
    'x, y, z, x_dot, y_dot, z_dot, q_w, q_x, q_y, q_z, x_ddot, y_ddot, z_ddot, omega_x, omega_y, omega_z, n_a_x, n_a_y, n_a_z, n_m_x, n_m_y, n_m_z, theta, p, t, dt')

state_vector = Matrix([x, y, z, x_dot, y_dot, z_dot, q_w, q_x, q_y, q_z])
control_vector = Matrix([x_ddot, y_ddot, z_ddot, omega_x, omega_y, omega_z])
measurement_vector = Matrix([n_a_x, n_a_y, n_a_z, n_m_x, n_m_y, n_m_z, p, t])

q = quat_order(1, Matrix([q_w, q_x, q_y, q_z]),
               Matrix([omega_x, omega_y, omega_z]), dt)

f = Matrix([
    x + x_dot * dt + x_ddot * dt ** 2 / 2,
    y + y_dot * dt + y_ddot * dt ** 2 / 2,
    z + z_dot * dt + z_ddot * dt ** 2 / 2,
    x_dot + x_ddot * dt,
    y_dot + y_ddot * dt,
    z_dot + z_ddot * dt,
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
SIGMA = Matrix([])
Q = integrate(G * SIGMA * transpose(G), (dt, 0, dt))
