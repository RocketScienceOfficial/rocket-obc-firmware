import numpy as np


def EKF(Z_Meas, Z_Meas_Flags, Z_Ctrl, init_params, F_handle, H_handle, Q_handle, R_handle):
    x = init_params['x0']
    P = init_params['P0']

    n_x = x.shape[0]
    n_m = Z_Meas.shape[1]

    X_est = np.zeros((n_x, n_m))
    X_pred = np.zeros((n_x, n_m + 1))
    P_est = np.zeros((n_x, n_x, n_m))
    P_pred = np.zeros((n_x, n_x, n_m + 1))

    for i in range(n_m):
        x_p, F = F_handle(x, Z_Ctrl[:, i])
        P_p = F @ P @ F.T + Q_handle(x_p)

        flags = Z_Meas_Flags[:, i:i + 1]
        meas = Z_Meas[:, i:i + 1]

        h_data = H_handle(x_p, meas, flags)
        r_data = R_handle(flags)

        for i in range(len(h_data)):
            h_p, H, m = h_data[i]
            R = r_data[i]

            PH_ = P_p @ H.T
            k = PH_ @ np.linalg.inv(H @ PH_ + R)
            x = x_p + k @ (m - h_p)
            I_kH = np.eye(n_x) - k @ H
            P = I_kH @ P_p @ I_kH.T + k @ R @ k.T
        
        X_est[:, i:i + 1] = x
        P_est[:, :, i:i + 1] = P[..., np.newaxis]
        X_pred[:, i:i + 1] = x_p
        P_pred[:, :, i:i + 1] = P_p[..., np.newaxis]
        
    x_p, F = F_handle(x, Z_Ctrl[:, i])
    P_p = F @ P @ F.T + Q_handle(x_p)

    X_pred[:, i + 1:i + 2] = x_p
    P_pred[:, :, i + 1:i + 2] = P_p[..., np.newaxis]

    return X_est, P_est, X_pred, P_pred
