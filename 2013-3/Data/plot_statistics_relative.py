#!/usr/bin/env python

from itertools import izip
from pprint import pprint
from re import match

import numpy
from numpy import array, max, mean, std

from mpl_toolkits.mplot3d import Axes3D
from matplotlib import pyplot

place = r'\(\s+(\d+),\s+(\d+),\s+(\d+)\) \-> \(\s+(\d+),\s+(\d+),\s+(\d+)\)'
values = r'\((\d+), (\d+), (\d+), (\d+)\) \((\d+), (\d+), (\d+), (\d+)\) (\d+)'

def cross(samples):
    n = len(samples)
    return [samples[i] - samples[j] for i in range(0, n) for j in range(i + 1, n)]

def relative(samples):
    (s1, s2) = samples
    return [((v2 / v1) if v1 != 0.0 else v2) for (v1, v2) in zip(cross(s1), cross(s2))]

class dimension(object):
    def __init__(self, band_name, bands, title, ymin, ymax):
        self.band_name = band_name
        self.bands = bands
        self.title = title
        self.ymin = ymin
        self.ymax = ymax
        self.data = {}

    def bandname(self, b):
        bands = self.bands
        if b == 0:
            return self.band_name + ' < %d' % bands[0]

        if b == len(bands):
            return self.band_name + ' >= %d' % bands[-1]

        return '%d <= %s < %d' % (bands[b - 1], self.band_name, bands[b])

    def append(self, d, samples):
        if (len(samples) == 0):
            return

        values = self.data.setdefault(d, [])
        values.extend(relative(samples))

    def plot(self, figure):
        figure.text(0.4, 0.95, self.title, horizontalalignment='center', fontsize=24)
        figure.text(0.02, 0.47, 'd')
        figure.text(0.4, 0.02, 's')
        figure.text(0.77, 0.47, 'n')

        data = self.data
        x = []
        y = []
        n = []

        for d in sorted(data.keys()):
            samples = data[d]
            m = mean(samples)

            x.append(d)
            y.append(m)
            n.append(float(len(samples)))

        plot1 = figure.add_subplot(1, 1, 1)
        plot2 = plot1.twinx()

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

def getband(bands, x):
    if x < bands[0]:
        return 0

    for i in range(1, len(bands)):
        if bands[i - 1] <= x and x < bands[i]:
            return i

    return len(bands)

def band_u_read(matched):
    return int(matched.group(3));

def band_u_check_forward(sample, bands):
    m = len(bands) / 2
    b = getband(bands, sample[0])
    w = sample[1]
    u = sample[3]
    return (w >= 0) and ((b == m) or (b < m) == (u < 0))

def band_u_check_sideways(sample, bands):
    w = sample[1]
    h = sample[2]
    u = sample[3]
    v = sample[4]
    return (abs(w) < 50) and (abs(h) < 50) and (u < 0) and (2 * abs(v) < abs(u))

def band_u_check_turning(sample, bands):
    w = sample[1]
    h = sample[2]
    u = sample[3]
    v = sample[4]
    return (abs(w) < 50) and (abs(h) < 50) and (u < 0) and (2 * abs(v) < abs(u))

def band_v_read(matched):
    return int(matched.group(4));

def read_change(matched):
    read = lambda offset: tuple(float(matched.group(index + offset)) for index in range(0, 4))
    return (read(1), read(5))

def read_rate(matched, index):
    a = int(matched.group(index + 5))
    b = int(matched.group(index + 1))
    return int(100.0 * float(a - b) / float(min(a, b)))

def read_diff(matched, index):
    a = int(matched.group(index + 5))
    b = int(matched.group(index + 1))
    return a - b

def collect_entries(s):
    samples = [[[], []], [[], []], [[], []], [[], []]]
    for sample in s.samples:
        if not s.sample_ok(sample):
            continue

        (c1, c2) = sample[-1]
        for i in range(0, 4):
            samples[i][0].append(c1[i])
            samples[i][1].append(c2[i])

    if len(samples[0][0]) > 1:
        return samples

    return []

def read_entry(s, line):
    if line == '\n':
        for (d, samples) in zip(s.dimensions, collect_entries(s)):
            d.append(s.index, samples)

        s.samples = []
        return s.header

    matched = match(values, line)
    if matched == None:
        raise Exception(line)

    b = s.read_band(matched)
    w = read_rate(matched, 0)
    h = read_rate(matched, 1)
    u = read_diff(matched, 2)
    v = read_diff(matched, 3)

    sample = (b, w, h, u, v, read_change(matched))
    s.samples.append(sample)

    return read_entry

class statistics(object):
    def __init__(self, header, band_name, bands, band_reader, sample_checker, *dimensions):
        self.index = 0
        self.header = header
        self.samples = []
        self.read_band = (lambda s, matched: band_reader(matched)).__get__(self)
        self.sample_ok = (lambda s, sample: sample_checker(sample, bands)).__get__(self)
        self.dimensions = [dimension(band_name, bands, *d) for d in dimensions]

    def load(self, path):
        state = self.header
        for line in open(path):
            state = state(self, line)

    def plot(self):
        i = 1
        for d in self.dimensions:
            d.plot(pyplot.figure(i))
            i += 1

        pyplot.show()

def statistics_forward():
    bands = [71, 142, 213, 284, 355, 426, 497, 568]
    dimensions = [('Width', 0, 70), ('Height', 0, 70), ('U', -120, 70), ('V', -120, 70)]
    s = statistics(read_header_forward, 'u', bands, band_u_read, band_u_check_forward, *dimensions)
    s.load('forward-2014-01-22-01.txt')
    s.plot()

def statistics_sideways():
    bands = [71, 142, 213, 284, 355, 426, 497, 568]
    plots = [('Width', -50, 50), ('Height', -50, 50), ('U', -250, 30), ('V', -250, 30)]
    s = statistics(read_header_sideways, 'u', bands, band_u_read, band_u_check_sideways, *plots)
    s.load('sideways-2014-01-22-01.txt')
    s.plot()

def statistics_turning():
    bands = [71, 142, 213, 284, 355, 426, 497, 568]
    plots = [('Width', -50, 50), ('Height', -50, 50), ('U', -600, 30), ('V', -600, 30)]
    s = statistics(read_header_turning, 'u', bands, band_u_read, band_u_check_turning, *plots)
    s.load('turning-2014-01-22-01.txt')
    s.plot()

def main():
    statistics_sideways()

if __name__ == '__main__':
    main()
