#!/usr/bin/env python
# coding=utf-8

from itertools import izip
from pprint import pprint
from re import match

from numpy import array, max, mean, std
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import pyplot

place = r'\(\s+(\d+),\s+(\d+),\s+(\d+)\) \-> \(\s+(\d+),\s+(\d+),\s+(\d+)\)'
values = r'\((\d+), (\d+), (\d+), (\d+)\) \((\d+), (\d+), (\d+), (\d+)\) (\d+)'

def stem(values):
    return std(values) / (float(len(values)) ** 0.5)

class difference(object):
    def __init__(self, title, ymin, ymax):
        self.title = title
        self.ymin = ymin
        self.ymax = ymax
        self.data = [{}, {}]

    def append(self, d, delta):
        subplot = self.data[0 if delta < 0 else 1]
        values = subplot.setdefault(d, [])
        values.append(delta)

    def subplotname(self, b):
        return u'Δ %s 0' % ('<' if b == 1 else '>=')

    def plot(self, figure):
        figure.text(0.4, 0.95, self.title, horizontalalignment='center', fontsize=24)
        figure.text(0.02, 0.47, 'd')
        figure.text(0.4, 0.02, 's')
        figure.text(0.77, 0.47, 'n')

        b = 1
        for band in self.data:

            x = []
            y = []
            n = []

            for d in sorted(band.keys()):
                samples = band[d]
                m = mean(samples)

                x.append(d)
                y.append(m)
                n.append(float(len(samples)))

            plot1 = figure.add_subplot(1, 2, b)
            plot1.text(0.5, 1.05, self.subplotname(b), horizontalalignment='center', fontsize=12, transform=plot1.transAxes)
            plot2 = plot1.twinx()
            b += 1

            box = plot1.get_position()
            plot1.set_position([box.x0 * 0.9 - 0.05, box.y0 * 1.05 - 0.04, box.width * 0.79, box.height])
            plot2.set_position([box.x0 * 0.9 - 0.05, box.y0 * 1.05 - 0.04, box.width * 0.79, box.height])

            #plot1.axis([min(x), max(x), self.ymin, self.ymax])
            plot2.axis([min(x), max(x), 0, max(n)])

            p1, = plot1.plot(x, y, 'b-')
            p2, = plot2.plot(x, n, 'g-')

        figure.legend([p1, p2], ['Average Difference (d)', 'Sample Size (n)'], loc=(0.78, 0.83))

def read_header_forward(s, line):
    matched = match(place, line)
    s.index = int(matched.group(4)) - int(matched.group(1))
    return read_entry

def read_header_sideways(s, line):
    matched = match(place, line)
    s.index = int(matched.group(5)) - int(matched.group(2))
    return read_entry

def read_header_turning(s, line):
    matched = match(place, line)
    s.index = int(matched.group(6)) - int(matched.group(3))
    return read_entry

def resample(samples):
    v = samples[0]
    e = samples[1]

    m = mean(e)
    s = stem(e)
    o = m + s

    resampled = [value for (value, error) in izip(v, e) if error < o]
    return resampled if len(resampled) > 2 else v

def read_band_u(matched):
    return int(matched.group(3));

def read_band_v(matched):
    return int(matched.group(4));

def read_entry(s, line):
    if line == '\n':
        d = s.d
        samples = resample(s.samples)
        for sample in samples:
            for i in range(0, 4):
                d[i].append(s.index, sample[i])

        s.samples = ([], [])
        return s.header

    matched = match(values, line)
    if matched == None:
        raise Exception(line)

    v = tuple(int(matched.group(5 + i)) - int(matched.group(1 + i)) for i in range(0, 4))
    e = int(matched.group(9))

    samples = s.samples
    samples[0].append(v)
    samples[1].append(e)

    return read_entry

class statistics(object):
    def __init__(self, header, *plots):
        self.index = 0
        self.header = header
        self.samples = ([], [])
        self.d = [difference(*plot) for plot in plots]

    def load(self, path):
        state = self.header
        for line in open(path):
            state = state(self, line)

    def plot(self):
        i = 1
        for d in self.d:
            d.plot(pyplot.figure(i))
            i += 1

        pyplot.show()

def statistics_forward():
    s = statistics(read_header_forward, ('Width', -10, 10), ('Height', -10, 10), ('X', -50, 50), ('Y', -30, 30))
    s.load('/home/helio/Projects/Data/Landmarks/forward-2014-01-22-01.txt')
    s.plot()

def statistics_sideways():
    s = statistics(read_header_sideways, ('Width', -8, 8), ('Height', -11, 11), ('X', -140, 30), ('Y', -30, 30))
    s.load('/home/erios/Projects/Data/Landmarks/sideways-2014-01-16-01.txt')
    s.plot()

def statistics_turning():
    s = statistics(read_header_turning, ('Width', -20, 20), ('Height', -15, 15), ('X', -300, 15), ('Y', -50, 80))
    s.load('/home/erios/Projects/Data/Landmarks/turning-2014-01-16-01.txt')
    s.plot()

def main():
    statistics_forward()

if __name__ == '__main__':
    main()

