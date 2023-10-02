import numpy as np
from sympy import *
from scipy.special import erfinv


def print_matrix(m: Matrix):
    for i in range(m.shape[0]):
        for j in range(m.shape[1]):
            print(m[i, j], end=', ')

        print("")


def convert_sympy_matrix_to_numpy(m: Matrix):
    return np.array(m.tolist()).astype(np.float64)


def add_noise(X, sigma, seed):
    S = np.tile(sigma, X.shape)

    np.random.seed(seed)

    randUniform = np.random.rand(*X.shape)
    randNormal = np.sqrt(2) * erfinv(2 * randUniform - 1)

    noise = randNormal * S

    Z = X + noise

    return Z
