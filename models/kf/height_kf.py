from sympy import *
from utils import *

dt, h, v, acc, p, t, sigma_acc, sigma_p, sigma_t = symbols(
    'dt, h, v, acc, p, t, sigma_acc, sigma_p, sigma_t')


state_vector = Matrix([
    h,
    v
])

control_vector = Matrix([
    acc,
])

measurement_vector = Matrix([
    p,
    t
])

f = Matrix([
    h + v * dt + acc * dt ** 2 / 2,
    v + acc * dt,
])

h = Matrix([
    101325 * (1 - (0.0065 * h) / 288.15) ** 5.25588,
    288.15 - 0.0065 * h,
])

F = f.jacobian(state_vector)
G = f.jacobian(control_vector)
H = h.jacobian(state_vector)
Q = integrate(G * transpose(G), (dt, 0, dt))
R = Matrix([
    [sigma_p, 0],
    [0, sigma_t],
])