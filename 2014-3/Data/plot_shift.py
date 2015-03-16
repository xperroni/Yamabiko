#!/usr/bin/env python

from itertools import count, izip
from os.path import exists
from os.path import join as joinpath
from sys import maxint

from matplotlib import pyplot, cm

from numpy import array, argmax
from numpy import min as amin
from numpy import max as amax
from numpy import sum as asum


def attraction(y, g):
    sum_iw = 0.0
    sum_w = 0.0
    for i in range(0, len(g)):
        if i == y:
            continue

        w = g[i] / abs(i - y if i != y else 1.0)
        sum_iw += i * w
        sum_w += w

    return int(round(sum_iw / sum_w))


def attraction_1(y, g):
    yl = y
    gl = g[y]
    for i in range(y, -1, -1):
        if g[i] < gl:
            break

        if g[i] > gl:
            yl = i
            gl = g[i]

    yr = y
    gr = g[y]
    for i in range(y + 1, len(g)):
        if g[i] < gr:
            break

        if g[i] > gr:
            yr = i
            gr = g[i]

    y2 = yl if gl > gr else yr

    return y2


def plot_contours(plotter, data, x0):
    x0 = abs(x0)
    d = data.T
    (m, n) = d.shape
    c = plotter.matshow(d, cmap=cm.jet, origin='lower', extent=(x0, x0 + n, -m // 2, m // 2))
    pyplot.colorbar(c)


def load_data(folder, x0, xn):
    def load_rows(folder):
        for (i, line) in izip(count(), open(joinpath(folder, 'shifts.txt'))):
            if i >= xn:
                break

            row = array(eval(line))
            row -= amin(row)
            row /= amax(row)
            yield row

    def merge(rows):
        if x0 < 0:
            for row in rows:
                yield row
        else:
            for i in range(x0, len(rows)):
                a = i - x0
                b = i + 1
                row = asum(array(rows[a:b]), 0)
                yield row

    rows = [row for row in load_rows(folder)]

    return array([row for row in merge(rows)])


def plot_ground(plotter, folder, ground, x0, xn, xs, ys):
    if x0 < 0:
        x0 = abs(x0)
        xn += x0

    if ys == 0.0:
        return

    path = joinpath(folder, 'ground_truth.txt' if ground == ' ' else ground)
    if not exists(path):
        return

    def load_ground(path):
        (x, y) = ([], [])
        for line in open(path):
            (x_replay, x_teach, shift) = eval(line)
            x_replay /= xs
            if x_replay < x0:
                continue
            if x_replay >= xn:
                break

            x.append(x_replay)
            y.append(shift / ys)

        return (array(x), array(y))


    (x, y) = load_ground(path)
    plotter.plot(x, y, 'k--')


def plot_shifts(plotter, data, x0, ys):
    if ys == 0.0:
        return

    estimates = [argmax(data[0])]
    for attractors in data[1:]:
        estimates.append(attraction(estimates[-1], attractors))

    y = array(estimates) - len(data[0]) // 2

    xn = len(y)
    if x0 < 0:
        x0 = abs(x0)
        x = range(x0, x0 + xn)
        plotter.plot(x, y, 'k-')
    else:
        x = range(x0, xn)
        plotter.plot(x, y[x0:], 'k-')


def plot(x0, ys, folder, ground=' ', xs='1.0', xn='maxint'):
    x0 = int(x0)
    xn = eval(xn)
    xs = eval(xs)
    ys = float(ys)

    (figure, axes) = pyplot.subplots()
    data = load_data(folder, x0, xn)
    (n, m) = data.shape

    plot_contours(axes, data, x0)

    plot_shifts(axes, data, x0, ys)

    plot_ground(axes, folder, ground, x0, n, xs, ys)

    axes.axis([abs(x0), n, -m // 2, m // 2])
    axes.xaxis.set_ticks_position('bottom')
    axes.set_aspect('auto', 'box')

    axes.grid()
    #axes.set_xlabel('Replay frame #', labelpad=10)
    #axes.set_ylabel('Shift (pixels%s)' % ((' / %.1f' % ys) if ys > 1.0 else ''), labelpad=20)

    pyplot.show()


def main():
    from sys import argv
    plot(*argv[1:])


if __name__ == '__main__':
    main()
