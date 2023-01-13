import matplotlib.pyplot as plt


def load_data_log2():
    with open('test_data/log2.csv', 'r') as f:
        lines = f.read().splitlines()[1:]
        d = []

        for x in lines:
            d.append(x.split(','))

        return d


def avg_filter(filtered_y):
    for i in range(2, len(filtered_y)):
        filtered_y[i] = (filtered_y[i] + filtered_y[i-1] + filtered_y[i-2]) / 3

    return filtered_y


def avg_filter_weight(filtered_y):
    for i in range(2, len(filtered_y)):
        filtered_y[i] = (filtered_y[i] * 0.5 + filtered_y[i-1]
                         * 0.3 + filtered_y[i-2] * 0.2)

    return filtered_y


def exp_filter(filtered_y, alpha, beta):
    b = filtered_y[1] - filtered_y[0]

    for i in range(2, len(filtered_y)):
        filtered_y[i] = alpha * filtered_y[i] + \
            (1 - alpha) * (filtered_y[i-1] + b)
        b = beta * (filtered_y[i] - filtered_y[i-1]) + (1-beta) * b

    return filtered_y


data = load_data_log2()
accel_x = [float(x[4]) for x in data]
accel_y = [float(x[5]) for x in data]
accel_z = [float(x[6]) for x in data]

current_data = accel_z

plt.plot(current_data)
#plt.plot(avg_filter(current_data))
#plt.plot(avg_filter_weight(current_data))
#plt.plot(exp_filter(current_data, 0.2, 0.5))
plt.show()
