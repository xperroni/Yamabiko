#!/usr/bin/env python

from re import match

from numpy import array, mean, std
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import pyplot, cm

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

def to2d(data, rows, cols):
    a = array(data)
    a.shape = (rows, cols)
    return a

class difference(object):
    def __init__(self, title):
        self.title = title
        self.data = {}
        self.rows = 0
        self.cols = 0

    def append(self, d, value):
        values = self.data.setdefault(d, [])
        values.append(value)
        self.rows = max(self.rows, d[0])
        self.cols = max(self.cols, d[1])

    def plot(self, figure):
        rows = self.rows
        cols = self.cols

        x = []
        y = []
        z = []
        w = []
        o = []
        n = []
        data = self.data
        for d in sorted(data.keys()):
            samples = array(data[d])
            m = mean(samples)
            s = std(samples) / (float(len(samples)) ** 0.5)
            x.append(d[0])
            y.append(d[1])
            z.append(m)
            w.append(m - s)
            o.append(m + s)
            n.append(float(len(samples)))

        plot1 = figure.add_subplot(111, projection='3d')
        #plot1.text(0.5, 1.05, self.title, horizontalalignment='center', fontsize=24, transform=plot1.transAxes)
        #plot1.axis([min(x), max(x), 0, max(o) * 1.05])
        #plot1.set_xlabel('s', labelpad=20, fontsize=18)
        #plot1.set_ylabel('d', labelpad=20, fontsize=18, rotation='horizontal')
        #plot2 = plot1.twinx()
        #plot2.axis([min(x), max(x), 0, max(n)])
        #plot2.set_ylabel('n', labelpad=20, rotation='horizontal')
        plot1.plot_surface(to2d(x, rows, cols), to2d(y, rows, cols), to2d(z, rows, cols), rstride=1, cstride=1, cmap=cm.coolwarm,
        linewidth=0, antialiased=False)
        #plot1.plot_wireframe(x, y, w, c='r')
        #plot1.plot_wireframe(x, y, o, c='r')
        #p3, = plot2.plot(x, n, 'g-')

        #box = plot1.get_position()
        #plot1.set_position([box.x0, box.y0, box.width * 0.79, box.height])
        #plot2.set_position([box.x0, box.y0, box.width * 0.79, box.height])
        #plot1.legend([p1, p2, p3], ['Absolute Difference (d)', 'Standard Error (d)', 'Sample Size (n)'], loc=2, bbox_to_anchor=(1.08, 1))

def read_header_forward(s, line):
    matched = match(place, line)
    s.index = (int(matched.group(2)) + 1, int(matched.group(4)) - int(matched.group(1)))
    return read_entry

def read_header_sideways(s, line):
    matched = match(place, line)
    s.index = (int(matched.group(5)) - int(matched.group(2)), int(matched.group(1)) + 1)
    return read_entry

def read_header_turning(s, line):
    matched = match(place, line)
    s.index = (int(matched.group(1)) + 1, int(matched.group(6)) - int(matched.group(3)))
    return read_entry

def read_entry(s, line):
    if line == '\n':
        return s.header

    matched = match(values, line)
    if matched == None:
        raise Exception(line)

    for i in range(0, 4):
        d = s.d[i]
        d.append(s.index, float(matched.group(1 + i)))

    return read_entry

class statistics(object):
    def __init__(self, header):
        self.index = 0
        self.header = header
        self.d = [
            difference('Width'),
            difference('Height'),
            difference('X'),
            difference('Y')
        ]

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
    s = statistics(read_header_forward)
    s.load('/home/helio/Projects/Data/Landmarks/forward-2014-01-13-01.txt')
    s.plot()

def statistics_sideways():
    s = statistics(read_header_sideways)
    s.load('/home/helio/Projects/Data/Landmarks/sideways-2014-01-13-01.txt')
    s.plot()

def statistics_turning():
    s = statistics(read_header_turning)
    s.load('/home/helio/Projects/Data/Landmarks/turning-2014-01-13-01.txt')
    s.plot()

def main():
    statistics_sideways()

if __name__ == '__main__':
    main()
