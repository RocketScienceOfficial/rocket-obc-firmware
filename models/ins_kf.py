from sympy import *

dt = symbols('dt')

x, y, z, x_dot, y_dot, z_dot, x_ddot, y_ddot, z_ddot, p, t = symbols(
    'x, y, z, x_dot, y_dot, z_dot, x_ddot, y_ddot, z_ddot, p, t')


def print_matrix(m):
    for i in range(m.shape[0]):
        for j in range(m.shape[1]):
            print(m[i, j], end=', ')
        print("")


state_vector = Matrix([
    x,
    y,
    z,
    x_dot,
    y_dot,
    z_dot,
])

control_vector = Matrix([
    x_ddot,
    y_ddot,
    z_ddot,
])

measurement_vector = Matrix([
    x,
    y,
    z,
    p,
    t
])

f = Matrix([
    x + x_dot * dt + x_ddot * dt ** 2 / 2,
    y + y_dot * dt + y_ddot * dt ** 2 / 2,
    z + z_dot * dt + z_ddot * dt ** 2 / 2,
    x_dot + x_ddot * dt,
    y_dot + y_ddot * dt,
    z_dot + z_ddot * dt,
])

h = Matrix([
    x,
    y,
    z,
    101325 * (1 - (0.0065 * z) / (288.15)) ** 5.25588,
    288.15 - 0.0065 * z,
])

F = f.jacobian(state_vector)
G = f.jacobian(control_vector)
H = h.jacobian(state_vector)

sigma_a = symbols('sigma_a')

Q = integrate(G * transpose(G), (dt, 0, dt)) * sigma_a

sigma_x, sigma_y, sigma_z, sigma_p, sigma_t = symbols(
    'sigma_x, sigma_y, sigma_z, sigma_p, sigma_t')

R = Matrix([
    [sigma_x, 0, 0, 0, 0],
    [0, sigma_y, 0, 0, 0],
    [0, 0, sigma_z, 0, 0],
    [0, 0, 0, sigma_p, 0],
    [0, 0, 0, 0, sigma_t],
])

var_x, var_y, var_z, var_x_dot, var_y_dot, var_z_dot, cov_x_x_dot, cov_y_y_dot, cov_z_z_dot = symbols(
    'var_x, var_y, var_z, var_x_dot, var_y_dot, var_z_dot, cov_x_x_dot, cov_y_y_dot, cov_z_z_dot')

P = Matrix([
    [var_x, 0, 0, cov_x_x_dot, 0, 0],
    [0, var_y, 0, 0, cov_y_y_dot, 0],
    [0, 0, var_z, 0, 0, cov_z_z_dot],
    [cov_x_x_dot, 0, 0, var_x_dot, 0, 0],
    [0, cov_y_y_dot, 0, 0, var_y_dot, 0],
    [0, 0, cov_z_z_dot, 0, 0, var_z_dot],
])

cov_predict = simplify(F * P * transpose(F) + Q)
#K = simplify(P * transpose(H) * (H * P * transpose(H) + R).inv())

#print_matrix(K)

print_matrix(Q)