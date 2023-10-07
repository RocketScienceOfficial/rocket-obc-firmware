import numpy as np


def EKF(Z_Meas, Z_Ctrl, init_params, F_handle, H_handle, Q_handle, R_handle):
    x = init_params['x0']
    P = init_params['P0']

    n_z = Z_Meas.shape[0]
    n_x = x.shape[0]
    n_m = Z_Meas.shape[1]

    X_est = np.zeros((n_x, n_m))
    X_pred = np.zeros((n_x, n_m + 1))
    P_est = np.zeros((n_x, n_x, n_m))
    P_pred = np.zeros((n_x, n_x, n_m + 1))
    K = np.zeros((n_x, n_z, n_m))

    for i in range(n_m):
        x_p, F = F_handle(x, Z_Ctrl[:, i])
        P_p = F @ P @ F.T + Q_handle(x_p)

        h_p, H = H_handle(x_p)
        PH_ = P_p @ H.T
        k = PH_ @ np.linalg.inv(H @ PH_ + R_handle(x_p))
        x = x_p + k @ (Z_Meas[:, i:i + 1] - h_p)
        I_kH = np.eye(n_x) - k @ H
        P = I_kH @ P_p @ I_kH.T + k @  R_handle(x) @ k.T

        X_est[:, i:i + 1] = x
        P_est[:, :, i:i + 1] = P[..., np.newaxis]
        K[:, :, i:i + 1] = k[..., np.newaxis]
        X_pred[:, i:i + 1] = x_p
        P_pred[:, :, i:i + 1] = P_p[..., np.newaxis]

    x_p, F = F_handle(x, Z_Ctrl[:, i])
    P_p = F @ P @ F.T + Q_handle(x_p)

    X_pred[:, i + 1:i + 2] = x_p
    P_pred[:, :, i + 1:i + 2] = P_p[..., np.newaxis]

    return X_est, P_est, X_pred, P_pred, K
