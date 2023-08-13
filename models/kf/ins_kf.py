from sympy import *
from utils import *

dt, x, y, z, x_dot, y_dot, z_dot, x_ddot, y_ddot, z_ddot, p, t, sigma_a, sigma_gps, sigma_p, sigma_t = symbols(
    'dt, x, y, z, x_dot, y_dot, z_dot, x_ddot, y_ddot, z_ddot, p, t, sigma_a, sigma_gps, sigma_p, sigma_t')


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
    101325 * (1 - (0.0065 * z) / 288.15) ** 5.25588,
    288.15 - 0.0065 * z,
])

F = f.jacobian(state_vector)
G = f.jacobian(control_vector)
H = h.jacobian(state_vector)
Q = integrate(G * transpose(G), (dt, 0, dt)) * sigma_a
R = Matrix([
    [sigma_gps, 0, 0, 0, 0],
    [0, sigma_gps, 0, 0, 0],
    [0, 0, sigma_gps, 0, 0],
    [0, 0, 0, sigma_p, 0],
    [0, 0, 0, 0, sigma_t],
])