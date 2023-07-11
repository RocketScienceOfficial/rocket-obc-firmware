import csv
import numpy as np
from ahrs.filters import Madgwick

gyro_data = []
acc_data = []

with open("D:/Data/Dev/Repos/easy-telemetry/Telemetry/Flight/FlightLog_2023-11-7--10-06-35.csv", 'r', encoding="utf-8") as f:
    reader = csv.reader(f)

    for row in reader:
        acc_data.append([float(row[0]), float(row[1]), float(row[2])])
        gyro_data.append([float(row[3]), float(row[4]), float(row[5])])

gyro_data = np.array(gyro_data)
acc_data = np.array(acc_data)

num_samples = len(acc_data)
madgwick = Madgwick(Dt=0.01, gain_imu=3.0)
Q = np.tile([1., 0., 0., 0.], (num_samples, 1))

with open('test.csv', 'w', newline='') as csvfile:
    csv_writer = csv.writer(csvfile, delimiter=',',
                            quotechar='"', quoting=csv.QUOTE_MINIMAL)

    for t in range(1, num_samples):
        Q[t] = madgwick.updateIMU(Q[t-1], gyr=gyro_data[t], acc=acc_data[t])

        roll = np.rad2deg(np.arctan2(
            2*(Q[t, 0]*Q[t, 1] + Q[t, 2]*Q[t, 3]), 1 - 2*(Q[t, 1]**2 + Q[t, 2]**2)))
        pitch = np.rad2deg(np.arcsin(2*(Q[t, 0]*Q[t, 2] - Q[t, 3]*Q[t, 1])))
        yaw = np.rad2deg(np.arctan2(
            2*(Q[t, 0]*Q[t, 3] + Q[t, 1]*Q[t, 2]), 1 - 2*(Q[t, 2]**2 + Q[t, 3]**2)))

        csv_writer.writerow([0, 0, 0, pitch, roll, yaw, 0, 0, 0, 0])
