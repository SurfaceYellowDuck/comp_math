from itertools import product
from math import sqrt, sin, cos
import matplotlib.pyplot as plt

import numpy as np

stiffness_matrix = {'a': np.zeros(0), 'b': np.zeros(0), 'c': np.zeros(0)}
task_params = {'h': 0, 'n': 0, 'x_nodes': np.zeros(0), 'y': np.zeros(0), 'lambda': 0.0}


def phi_j(params, j, x):
    # page 92

    if j == 0:
        if 0 <= x <= params['x_nodes'][1]:
            return (params['x_nodes'][1] - x) / params['h']
        else:
            return 0
    elif j == params['n']:
        if params['x_nodes'][params['n'] - 1] <= x <= params['x_nodes'][params['n']]:
            return (x - params['x_nodes'][params['n'] - 1]) / params['h']
        else:
            return 0
    else:
        if params['x_nodes'][j - 1] <= x <= params['x_nodes'][j]:
            return (x - params['x_nodes'][j - 1]) / params['h']
        elif params['x_nodes'][j] <= x <= params['x_nodes'][j + 1]:
            return (params['x_nodes'][j + 1] - x) / params['h']
        else:
            return 0


def calc_left_coefficients(params, j, k):
    xk: np.ndarray = params['x_nodes']
    lb = params['lambda']
    if j > k:
        j, k = k, j
    if j == k:
        return (xk[j + 1] + lb * xk[j] ** 2 * xk[j + 1] - lb * xk[j] * xk[j + 1] ** 2 + (lb * xk[j + 1] ** 3) / 3 -
                xk[j - 1] - lb * xk[j] ** 2 * xk[j - 1] + lb * xk[j] * xk[j - 1] ** 2 - (lb * xk[j - 1] ** 3) / 3) / (
                       params['h'] ** 2)
    elif j + 1 == k:
        return (-1 / 6.0) * (-6 + lb * (xk[j] - xk[j - 1]) ** 2) * (xk[j] - xk[j + 1]) / (params['h'] ** 2)
    else:
        return 0


def calc_right_coefficients(params, j):
    xk: np.ndarray = params['x_nodes']
    lb_sqrt = sqrt(params['lambda'])
    first_integral = 2 * (- lb_sqrt * (xk[j] - xk[j + 1]) * cos(lb_sqrt * xk[j]) + sin(lb_sqrt * xk[j]) - sin(
        lb_sqrt * xk[j + 1]))
    second_integral = 2 * (- lb_sqrt * (xk[j] - xk[j - 1]) * cos(lb_sqrt * xk[j]) + sin(lb_sqrt * xk[j]) - sin(
        lb_sqrt * xk[j - 1]))

    return (first_integral + second_integral) / params['h']


def thomas_algorithm(mt, d):
    n: int = d.size
    c1: np.ndarray = np.zeros(n, dtype='float64')
    d1: np.ndarray = np.zeros(n, dtype='float64')
    x: np.ndarray = np.zeros(n + 1, dtype='float64')

    helper = lambda i: mt['b'][i] - mt['a'][i] * c1[i - 1]
    c_new = lambda i: (mt['c'][i] / mt['b'][i]) if i == 0 else (mt['c'][i] / helper(i))
    d_new = lambda i: (d[i] / mt['b'][i]) if i == 0 else (d[i] - mt['a'][i] * d1[i - 1]) / helper(i)

    for i in range(n):
        c1[i] = c_new(i)
        d1[i] = d_new(i)

    x_new = lambda i: d1[i] if i == (n - 1) else d1[i] - c1[i] * x[i + 1]

    for i in range(n - 1, -1, -1):
        x[i] = x_new(i)

    return x


def solve_sys(params):
    stiffness_matrix['a'] = np.zeros(params['n'])
    stiffness_matrix['b'] = np.zeros(params['n'])
    stiffness_matrix['c'] = np.zeros(params['n'])

    d: np.ndarray = np.zeros(params['n'])

    for i in range(1, params['n'] + 1):
        j = i - 1

        if i >= 1:
            stiffness_matrix['a'][j] = calc_left_coefficients(params, i - 1, i)

        stiffness_matrix['b'][j] = calc_left_coefficients(params, j, j)

        if i < params['n']:
            stiffness_matrix['c'][j] = calc_left_coefficients(params, i, i + 1)

        d[j] = calc_right_coefficients(params, j)

    y = thomas_algorithm(stiffness_matrix, d)

    y[0] = 0
    y[params['n']] = 0
    params['y'] = y


def u_exact(params, x):
    left_index, right_index = 0, params['n']

    while right_index - left_index > 1:
        i = (right_index + left_index) // 2
        if x > params['x_nodes'][i]:
            left_index = i
        else:
            right_index = i

    return params['y'][left_index] * phi_j(params, left_index, x) + \
           params['y'][right_index] * phi_j(params, right_index, x)


def calc_error(params, r_b, n):
    test_n: int = (params['n'] + 1) * 10
    test_h: float = r_b / test_n
    err: float = 0.0
    real_val_elems = []
    approx_val_elems = []
    for i in range(test_n):
        x: float = i * test_h
        approx_val: float = u_exact(params, x)
        real_val: float = sin(sqrt(params['lambda']) * x)
        real_val_elems.append(real_val)
        approx_val_elems.append(approx_val)
        err = max(abs(approx_val - real_val), err)
    plt.plot(np.linspace(0.0, r_b, test_n), real_val_elems)
    plt.show()
    # fig.get_figure().savefig(f"images/testfig_{n}")

    return test_h, err


def FEM(r_b, lam, n):
    x: np.ndarray = np.linspace(0.0, r_b, n)
    task_params['h'] = x[1] - x[0]
    task_params['x_nodes'] = x
    task_params['n'] = n - 1
    task_params['y'] = np.zeros(1)
    task_params['lambda'] = lam
    solve_sys(task_params)
    return task_params


def experiment():
    test = list(product([1, 10, 100, 1000], [10, 100, 1000, 10000]))
    results = []

    for lam, n in test:
        right_b = 2 * np.pi / sqrt(lam)
        y = FEM(right_b, lam, n)
        h, err = calc_error(y, right_b, n)
        results.append({'h': y['h'], 'error': err, 'n': n, 'lambda': lam})
    # with open('experiment.txt', 'w') as file:
    for el in results:
        print(f"error: {round(el['error'], 6)} h**2: {round(el['h'], 6)}, n: {el['n']},  lambda: {el['lambda']}")
        # file.write(f"error: {round(el['error'], 6)} h**2: {round(el['h'], 6)}, n: {el['n']},  lambda: {el['lambda']} \n")
    # file.close()


experiment()

# task_params = {'h': 0, 'n': 0, 'x_nodes': np.zeros(0), 'y': np.zeros(0), 'lambda': 0.0}
# y = FEM(np.pi, np.pi ** 2, 10)
# plt.plot(np.linspace(0.0, np.pi, 10), task_params['y'])
# plt.show()
