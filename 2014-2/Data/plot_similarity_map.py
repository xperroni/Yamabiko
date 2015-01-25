#!/usr/bin/env python

from sys import float_info

from matplotlib import pyplot, cm

from numpy import array, arange
from numpy import max as amax


def meters(v, fps, n):
    s = arange(0.0, float(n))
    return array([v * t / fps for t in s])


def load_data(path):
    def normalize(values):
        row = array(values)
        return row * 100.0 / amax(row)

    rows = eval(' '.join(s.strip() for s in open(path)))
    data = array([normalize(row) for row in rows])
    #data = array(rows)
    return data.T


def slide_x(data):
    (m, n) = data.shape
    yn = float(m)

    x0 = 0.0
    xt = 1.0
    d = float_info.max
    x1n = n / 3.0
    x2n = n - x1n
    for x2 in arange(n - 1.0, x2n - 1.0, -1.0):
        for x1 in arange(0, x1n):
            t = yn / (x2 - x1)
            e = 0.0

            for j in arange(x1, x2):
                i = min(t * j, m - 1)
                e += data[i, j]
                if e >= d:
                    break

            if e < d:
                d = e;
                x0 = x1;
                xt = t;

    return (x0, xt, d)


def slide_y(data):
    (m, n) = data.shape
    xn = float(n)

    y0 = 0.0
    yt = 1.0
    d = float_info.max
    y1n = m / 3.0
    y2n = m - y1n
    for y2 in arange(m - 1.0, y2n - 1.0, -1.0):
        for y1 in arange(0.0, y1n):
            t = (y2 - y1) / xn
            e = 0.0

            for j in arange(0.0, xn):
                i = y1 + t * j
                e += data[i, j]
                if e >= d:
                    break

            if e < d:
                d = e;
                y0 = y1;
                yt = t;

    return (y0, yt, d)


def slide(data):
    costs = amax(data) - data
    (x0, xt, xd) = slide_x(costs)
    (y0, yt, yd) = slide_y(costs)

    if xd > yd:
        return (x0, 0, xt)
    else:
        return (0, y0, yt)


def plot_line(plotter, data, line, v = 0.3):
    marker = 'k--' # k-- / w--

    if len(line) == 4:
        (x1, y1, x2, y2) = [int(s) for s in line]
        if x1 < x2:
            plotter.plot([y1, y2], [x1, x2], marker)

        return

    (m, n) = data.shape
    print (m, n)

    (x0, y0, t) = slide(data)
    print (x0, y0, t)

    x = [x0]
    y = [y0]
    while x[-1] < n and y[-1] < m:
        xt = x[-1] + 1.0
        yt = y0 + xt * t
        x.append(xt)
        y.append(yt)

    plotter.plot(x, y, marker)


def plot_contours(plotter, data):
    c = plotter.contourf(data, cmap=cm.jet) # cmap=cm.gray / cmap=cm.jet
    pyplot.colorbar(c)


def plot(path, *line):
    (figure, axes) = pyplot.subplots()
    data = load_data(path)
    plot_contours(axes, data)
    plot_line(axes, data, line)

    axes.grid()
    #axes.set_xlabel('Replay image index #', labelpad=10)
    #axes.set_ylabel('Teach image index #', labelpad=10)
    #axes.axis([0, 59, 0, 59])

    figure.savefig('figure.svg', bbox_inches='tight')

    pyplot.show()


def main():
    from sys import argv

    plot(*argv[1:])


if __name__ == '__main__':
    main()
