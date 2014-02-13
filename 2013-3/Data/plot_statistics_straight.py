# Copyright (c) Helio Perroni Filho <xperroni@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

#!/usr/bin/env python

from itertools import izip
from os.path import join as joinpath
from pprint import pprint
from re import match

import numpy
from numpy import array, max, mean, std

from mpl_toolkits.mplot3d import Axes3D
from matplotlib import pyplot

place = r'\(\s+(\d+),\s+(\d+),\s+(\d+)\) \-> \(\s+(\d+),\s+(\d+),\s+(\d+)\)'
values = r'\((\d+), (\d+), (\d+), (\d+)\) \((\d+), (\d+), (\d+), (\d+)\) (\d+)'

class plotter(object):
    def __init__(self, title, ymin, ymax):
        self.title = title
        self.ymin = ymin
        self.ymax = ymax
        self.data = {}

    def append(self, d, samples):
        if (len(samples) == 0):
            return

        values = self.data.setdefault(d, [])
        values.extend(samples)

    def plot(self):
        figure = pyplot.figure()
        figure.text(0.5, 0.95, self.title, horizontalalignment='center', fontsize=24)

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

        plot1.axis([min(x), max(x), 0, max(y)])
        plot2.axis([min(x), max(x), 0, max(n)])

        p1, = plot1.plot(x, y, 'b-')
        p2, = plot2.plot(x, n, 'g-')

        return figure

    def save(self, mode, folder):
        self.plot().savefig(joinpath(folder, '%s-%s.svg' % (mode, self.title)), bbox_inches='tight')

def read_header_straight(s, line):
    matched = match(place, line)
    s.index = int(matched.group(4)) - int(matched.group(1))
    return read_entry

def check_straight(sample):
    (size, drift) = sample
    return (size > 0) and (drift > 0)

def read_change_size(matched):
    area = lambda offset : float(matched.group(1 + offset)) * float(matched.group(2 + offset))
    a1 = area(0)
    a2 = area(4)
    return (a2 - a1) / min(a1, a2)

def read_change_drift(matched):
    distance = lambda offset : ((float(matched.group(3 + offset)) - 320.0) ** 2.0 + (float(matched.group(4 + offset)) - 240) ** 2.0) ** 0.5
    return distance(4) - distance(0)

def collect_entries(s):
    samples = [[], []]
    for sample in s.samples:
        if not s.sample_ok(sample):
            continue

        for (r, v) in zip(samples, sample):
            r.append(v)

    if len(samples[0]) > 1:
        return samples

    return []

def read_entry(s, line):
    if line == '\n':
        for (plotter, samples) in zip(s.plotters, collect_entries(s)):
            plotter.append(s.index, samples)

        s.samples = []
        return s.header

    matched = match(values, line)
    if matched == None:
        raise Exception(line)

    size = read_change_size(matched)
    drift = read_change_drift(matched)

    sample = (size, drift)
    s.samples.append(sample)

    return read_entry

class statistics(object):
    def __init__(self, header, checker, *plots):
        self.index = 0
        self.header = header
        self.sample_ok = (lambda s, sample: checker(sample)).__get__(self)
        self.samples = []
        self.plotters = [plotter(*d) for d in plots]

    def load(self, path):
        state = self.header
        for line in open(path):
            state = state(self, line)

    def plot(self):
        for plotter in self.plotters:
            plotter.plot()

        pyplot.show()

    def save(self, mode, folder):
        for plotter in self.plotters:
            plotter.save(mode, folder)

def statistics_straight():
    plots = [('Size', 0, 70), ('Drift', 0, 70)]
    s = statistics(read_header_straight, check_straight, *plots)
    s.load('forward-2014-01-22-01.txt')
    #s.plot()
    s.save('straight', '/home/helio/Roboken/Yamabiko/2013-3/Data')

def main():
    statistics_straight()

if __name__ == '__main__':
    main()
