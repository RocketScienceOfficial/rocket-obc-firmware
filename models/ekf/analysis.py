import matplotlib.pyplot as plt
import numpy as np


def __draw_3_axis_data(t, title, est_data, true_data):
    fig = plt.figure(figsize=(20, 10))

    plt.subplot(3, 1, 1)

    if true_data is not None:
        plt.plot(t, true_data[0], '-gd', markerfacecolor='lime',
                 linewidth=3, markeredgewidth=2, markersize=8, label='True values')

    plt.plot(t, est_data[0],  '-ro', markerfacecolor='m', linewidth=3,
             markeredgewidth=2, markersize=8, label='Estimates')

    plt.xlabel('Time (s)', fontsize=14, color='darkred')
    plt.ylabel('X (m)', fontsize=14, color='darkred')
    plt.legend(fontsize='x-large')

    plt.subplot(3, 1, 2)

    if true_data is not None:
        plt.plot(t, true_data[1], '-gd', markerfacecolor='lime',
                 linewidth=3, markeredgewidth=2, markersize=8, label='True values')

    plt.plot(t, est_data[1],  '-ro', markerfacecolor='m', linewidth=3,
             markeredgewidth=2, markersize=8, label='Estimates')

    plt.xlabel('Time (s)', fontsize=14, color='darkred')
    plt.ylabel('Y (m)', fontsize=14, color='darkred')
    plt.legend(fontsize='x-large')

    plt.subplot(3, 1, 3)

    if true_data is not None:
        plt.plot(t, true_data[2], '-gd', markerfacecolor='lime',
                 linewidth=3, markeredgewidth=2, markersize=8, label='True values')

    plt.plot(t, est_data[2],  '-ro', markerfacecolor='m', linewidth=3,
             markeredgewidth=2, markersize=8, label='Estimates')

    plt.xlabel('Time (s)', fontsize=14, color='darkred')
    plt.ylabel('Z (m)', fontsize=14, color='darkred')
    plt.legend(fontsize='x-large')

    plt.grid(which='both', color='0.95', linestyle='-')

    fig.suptitle(title)


def __draw_position(t, est_pos):
    __draw_3_axis_data(t, 'Position', est_pos, None)

    ax = plt.figure().add_subplot(projection='3d')
    ax.set_xlim([-5, 5])
    ax.set_ylim([-5, 5])
    ax.set_zlim([700, 1000])
    ax.plot(est_pos[0, :], est_pos[1, :], est_pos[2, :])


def __draw_velocity(t, est_vel):
    __draw_3_axis_data(t, 'Velocity', est_vel, None)

    plt.figure(figsize=(20, 10))

    est_v = np.sqrt(est_vel[0] ** 2 + est_vel[1] ** 2 + est_vel[2] ** 2)

    plt.plot(t, est_v,  '-ro', markerfacecolor='m', linewidth=3,
             markeredgewidth=2, markersize=8, label='Estimates')

    plt.xlabel('Time (s)', fontsize=14, color='darkred')
    plt.ylabel('V (m/s)', fontsize=14, color='darkred')
    plt.legend(fontsize='x-large')


def draw_plots(X, dt):
    t = np.arange(0, X.shape[1] * dt, dt)

    __draw_position(t, X[7:10, :])
    __draw_velocity(t, X[4:7, :])

    plt.show()
