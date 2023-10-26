import numpy as np


class ExtendedKalmanFilter:
    def __init__(self, x0, P0, n_m, g, dt, ctrl_vars):
        self.x = x0
        self.P = P0

        self.g = g
        self.dt = dt
        self.ctrl_vars = ctrl_vars

        n_x = self.x.shape[0]

        self.i = -1
        self.X_est = np.zeros((n_x, n_m))
        self.X_pred = np.zeros((n_x, n_m + 1))
        self.P_est = np.zeros((n_x, n_x, n_m))
        self.P_pred = np.zeros((n_x, n_x, n_m + 1))

    def predict(self, z, f, F, Q):
        calc_f = f(*self.x[:, 0], *z, self.g, self.dt)
        calc_F = F(*self.x[:, 0], *z, self.g, self.dt)
        calc_Q = Q(*self.x[:, 0], *self.ctrl_vars, self.g, self.dt)

        self.x = calc_f
        self.P = calc_F @ self.P @ calc_F.T + calc_Q

        self.i += 1
        
        i = self.i

        self.X_pred[:, i:i + 1] = self.x
        self.P_pred[:, :, i:i + 1] = self.P[..., np.newaxis]

    def correct(self, z, h, H, R):
        z = np.array([z]).T

        calc_h = h(*self.x[:, 0])
        calc_H = H(*self.x[:, 0])

        PH_ = self.P @ calc_H.T
        K = PH_ @ np.linalg.inv(calc_H @ PH_ + R)
        self.x = self.x + K @ (z - calc_h)
        I_KH = np.eye(calc_H.shape[1]) - K @ calc_H
        self.P = I_KH @ self.P @ I_KH.T + K @ R @ K.T

        i = self.i

        self.X_est[:, i:i + 1] = self.x
        self.P_est[:, :, i:i + 1] = self.P[..., np.newaxis]
