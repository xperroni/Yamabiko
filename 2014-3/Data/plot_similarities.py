#!/usr/bin/env python

from itertools import count, izip
from os.path import exists
from os.path import join as joinpath
from sys import maxint

from matplotlib import pyplot, cm

from numpy import array, arange, zeros
from numpy import max as amax


def linePn(x0, y0, t, m, n):
    xn = n - 1
    yn = m - 1
    yd = yn - y0;
    xd = x0 + yd / t;

    if xd <= xn:
        return (xd, yn)
    else:
        return (xn, y0 + (xn - x0) * t)


def load_similarities(path, width, M, N):
    cols = [eval(line) for line in open(path)]

    n = len(cols)
    w = len(cols[0])
    m = w + max(n - width, 0)

    data = zeros((m, n))
    for (j, col) in izip(count(), cols):
        i = max(j - width, 0)
        z = i + w
        data[i:z, j] = col

    i_n = min(M, data.shape[0])
    j_n = min(N, data.shape[1])
    return data[:i_n, :j_n]


def plot_lines(plotter, folder, m, n, M, N):
    path = joinpath(folder, 'lines.txt')
    lines = [eval(line) for line in open(path)]
    x = [lines[0][0]]
    y = [lines[0][1]]

    for (k, line) in izip(count(), lines):
        (x0, y0, t) = line
        (xi, yi) = linePn(x0 + k, y0 + k, t, m + k, n + k)
        if xi > N or yi > M:
            break

        x.append(xi)
        y.append(yi)

    plotter.plot(x, y, 'k-')


def plot_contours(plotter, data):
    c = plotter.matshow(data, cmap=cm.jet, origin='lower') # cmap=cm.gray / cmap=cm.jet


def plot_ground_truth(plotter, folder, ground, M, N):
    path = joinpath(folder, 'ground_truth.txt' if ground == '' else ground)
    if not exists(path):
        return

    x = []
    y = []
    for line in open(path):
        (xi, yi, si) = eval(line)
        if xi > N or yi > M:
            break

        x.append(xi)
        y.append(yi)

    plotter.plot(x, y, 'w-')


def plot(m, n, folder, ground='', M='maxint', N='maxint'):
    path_similarities = joinpath(folder, 'similarities.txt')
    m = int(m)
    n = int(n)
    M = eval(M)
    N = eval(N)

    (figure, axes) = pyplot.subplots()
    data = load_similarities(path_similarities, n, M, N)

    plot_lines(axes, folder, m, n, M, N)

    plot_contours(axes, data)
    plot_ground_truth(axes, folder, ground, M, N)

    (m, n) = data.shape
    axes.axis([-0.5, n - 0.5, -0.5,  m - 0.5])

    axes.set_xticks(arange(-0.5, n, 1.0))
    axes.set_yticks(arange(-0.5, m, 1.0))

    axes.set_xticklabels([(str(i) if i % 10 == 0 else '') for i in range(0, n)])
    axes.set_yticklabels([(str(i) if i > 0 and i % 10 == 0 else '') for i in range(0, m)])

    axes.xaxis.set_ticks_position('bottom')

    axes.grid()

    axes.set_xlabel('Replay image index #', labelpad=10)
    axes.set_ylabel('Teach image index #', labelpad=10)

    pyplot.show()


def main():
    from sys import argv
    plot(*argv[1:])


if __name__ == '__main__':
    main()
