#!/usr/bin/env python

from re import match

from numpy import array, max, mean, std
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import pyplot

place = r'\(\s+(\d+),\s+(\d+),\s+(\d+)\) \-> \(\s+(\d+),\s+(\d+),\s+(\d+)\)'
values = r'\(\s+(\d+\.\d+),\s+(\d+\.\d+),\s+(\d+\.\d+),\s+(\d+\.\d+)\)'

def gausstimate(samples):
    m = mean(samples)
    s = std(samples) / (float(len(samples)) ** 0.5)

    #o = m + s
    #w = m - s

    #resample = [v for v in samples if w < v and v < o]
    #if len(resample) > 2:
    #    m = mean(resample)
    #    s = std(resample)

    return (m, s)

class difference(object):
    def __init__(self, title, ymax):
        self.title = title
        self.ymax = ymax
        self.data = {}

    def append(self, d, value):
        values = self.data.setdefault(d, {})
        values[value] = values.get(value, 0) + 1

    def plot(self):
        x = []
        y = []
        w = []
        o = []
        n = []
        data = self.data
        for d in sorted(data.keys()):
            samples = array([k for (k, v) in data[d].iteritems() for i in range(0, v)])
            (m, s) = gausstimate(samples)
            x.append(d)
            w.append(m - s)
            y.append(m)
            o.append(m + s)
            n.append(float(len(samples)))

        plot1 = pyplot.subplot(111)
        plot1.text(0.5, 1.05, self.title, horizontalalignment='center', fontsize=24, transform=plot1.transAxes)
        plot1.axis([min(x), max(x), 0, self.ymax])
        plot1.set_xlabel('s', labelpad=20, fontsize=18)
        plot1.set_ylabel('d', labelpad=20, fontsize=18, rotation='horizontal')
        plot2 = plot1.twinx()
        plot2.axis([min(x), max(x), 0, max(n)])
        plot2.set_ylabel('n', labelpad=20, rotation='horizontal')
        p1, = plot1.plot(x, y, 'b-')
        p2, = plot1.plot(x, w, 'r-')
        plot1.plot(x, o, 'r-')
        p3, = plot2.plot(x, n, 'g-')

        box = plot1.get_position()
        plot1.set_position([box.x0, box.y0, box.width * 0.79, box.height])
        plot2.set_position([box.x0, box.y0, box.width * 0.79, box.height])
        plot1.legend([p1, p2, p3], ['Absolute Difference (d)', 'Standard Error (d)', 'Sample Size (n)'], loc=2, bbox_to_anchor=(1.08, 1))

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

def read_entry(s, line):
    if line == '\n':
        return s.header

    matched = match(values, line)
    if matched == None:
        raise Exception(line)
        return read_entry

    for i in range(0, 4):
        d = s.d[i]
        d.append(s.index, float(matched.group(1 + i)))

    return read_entry

class statistics(object):
    def __init__(self, ymax, header):
        self.index = 0
        self.header = header
        self.d = [
            difference('Width', ymax),
            difference('Height', ymax),
            difference('X', ymax),
            difference('Y', ymax)
        ]

    def load(self, path):
        state = self.header
        for line in open(path):
            state = state(self, line)

    def plot(self):
        i = 1
        for d in self.d:
            pyplot.figure(i)
            d.plot()
            i += 1

        pyplot.show()

def statistics_forward():
    s = statistics(65, read_header_forward)
    s.load('/home/helio/Projects/Data/Landmarks/forward-2014-01-13-01.txt')
    s.plot()

def statistics_sideways():
    s = statistics(200, read_header_sideways)
    s.load('/home/helio/Projects/Data/Landmarks/sideways-2014-01-13-01.txt')
    s.plot()

def statistics_turning():
    s = statistics(300, read_header_turning)
    s.load('/home/helio/Projects/Data/Landmarks/turning-2014-01-13-01.txt')
    s.plot()

def main():
    statistics_sideways()

if __name__ == '__main__':
    main()
