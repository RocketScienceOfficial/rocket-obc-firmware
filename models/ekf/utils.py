import numpy as np
from sympy import *
from scipy.special import erfinv


def print_matrix(m: Matrix):
    for i in range(m.shape[0]):
        for j in range(m.shape[1]):
            print(m[i, j], end=', ')

        print("")


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


def add_noise(X, sigma, seed):
    S = np.tile(sigma, X.shape)

    np.random.seed(seed)

    randUniform = np.random.rand(*X.shape)
    randNormal = np.sqrt(2) * erfinv(2 * randUniform - 1)

    noise = randNormal * S

    Z = X + noise

    return Z
