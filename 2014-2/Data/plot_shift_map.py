#!/usr/bin/env python

from sys import float_info

from matplotlib import pyplot, cm

from numpy import array, arange
from numpy import min as amin
from numpy import max as amax
from numpy import argmax, sum


def attraction(y, g):
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

    drag = lambda i, w: w #/ max((y - i) ** 2.0, 1)

    y2 = yl if drag(yl, gl) > drag(yr, gr) else yr

    return y2


def shifts(contours):
    estimates = [argmax(contours[0])]
    #estimates = [argmax(sum(contours, axis=0))]
    for attractors in contours[1:]:
        attractors = attractors - amin(attractors)
        attractors /= amax(attractors)
        estimates.append(attraction(estimates[-1], attractors))

    return array(estimates) - len(contours[0]) // 2


def plot_contours(plotter, data, x0):
    d = data.T

    (m, n) = d.shape
    x = arange(x0, x0 + n)
    y = arange(-m // 2, m // 2)


    c = plotter.contourf(x, y, d, cmap=cm.gray) # cmap=cm.gray / cmap=cm.jet
    pyplot.colorbar(c)


def load_data(path):
    return array(eval(' '.join(s.strip() for s in open(path).readlines())))


def plot(path, x0):
    x0 = int(x0)
    (figure, axes) = pyplot.subplots()
    data = load_data(path)
    plot_contours(axes, data, x0)

    d = shifts(data)
    axes.plot(range(x0, x0 + len(d)), d, 'k--')

    axes.grid()
    axes.set_xlabel('Replay image index #', labelpad=10)
    axes.set_ylabel('Shift', labelpad=20)

    figure.savefig('figure.svg', bbox_inches='tight')

    pyplot.show()


def main():
    from sys import argv

    plot(*argv[1:])


if __name__ == '__main__':
    main()
