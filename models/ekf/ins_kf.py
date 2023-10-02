import numpy as np
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


config = {
    'dt': 0.01,
    'N': 1000,
    'seed': 0,
    'x0': np.array([[0, 0, 0, 0, 0, 0]]).T,
    'P0': np.eye(6) * 1000,
    'sigma_a': 0.1,
    'sigma_gps': 2,
    'sigma_p': 0.5,
    'sigma_t': 0.5,
    'g': -9.81,
    'a': 20,
}


def generate_scenario(config):
    dt = config['dt']

    t = np.arange(0, dt * config['N'], dt)

    a_x = np.tile(0, t.size)
    a_y = np.tile(0, t.size)
    a_z = np.tile(config['a'], t.size)
    x = np.tile(0, t.size)
    y = np.tile(0, t.size)
    z = 0.5 * a_z * t ** 2
    pressure = 101325 * (1 - (0.0065 * z) / 288.15) ** 5.25588
    temperature = 288.15 - 0.0065 * z
    a_z = a_z - config['g']

    z = add_noise(z, config['sigma_gps'], config['seed'])
    pressure = add_noise(pressure, config['sigma_p'], config['seed'])
    temperature = add_noise(temperature, config['sigma_t'], config['seed'])
    a_z = add_noise(a_z, config['sigma_a'], config['seed'])

    a_x = np.insert(a_x, 0, np.array([0]))
    a_y = np.insert(a_y, 0, np.array([0]))
    a_z = np.insert(a_z, 0, np.array([0]))

    measurements = np.vstack((x, y, z, pressure, temperature))
    controls = np.vstack((a_x, a_y, a_z))

    Z = np.array([measurements, controls], dtype=object)

    return Z


def get_R(config):
    return convert_sympy_matrix_to_numpy(R.subs({
        sigma_gps: config['sigma_gps'],
        sigma_p: config['sigma_p'],
        sigma_t: config['sigma_t'],
    }))


def get_Q(config):
    return convert_sympy_matrix_to_numpy(Q.subs({
        dt: config['dt'],
        sigma_a: config['sigma_a'],
    }))


def get_F(x_state, u, config):
    return convert_sympy_matrix_to_numpy(f.subs({
        dt: config['dt'],
        x: x_state[0, 0],
        y: x_state[1, 0],
        z: x_state[2, 0],
        x_dot: x_state[3, 0],
        y_dot: x_state[4, 0],
        z_dot: x_state[5, 0],
        x_ddot: u[0],
        y_ddot: u[1],
        z_ddot: u[2],
    })), convert_sympy_matrix_to_numpy(F.subs({
        dt: config['dt'],
    }))


def get_H(x_state):
    return convert_sympy_matrix_to_numpy(h.subs({
        x: x_state[0, 0],
        y: x_state[1, 0],
        z: x_state[2, 0],
    })), convert_sympy_matrix_to_numpy(H.subs({
        z: x_state[2, 0],
    }))


X_est, P_est, X_pred, P_pred, K = EKF(
    generate_scenario(config), config, get_R, get_Q, get_F, get_H)

print(X_est[2])