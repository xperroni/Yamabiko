#!/usr/bin/env python

from itertools import izip
from os.path import join as joinpath
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
    def __init__(self, band_name, bands, title, ymin, ymax, nmax):
        self.band_name = band_name
        self.bands = bands
        self.title = title
        self.ymin = ymin
        self.ymax = ymax
        self.nmax = nmax
        self.data = {}

    def bandname(self, b):
        bands = self.bands
        if b == 0:
            return self.band_name + ' < %d' % bands[0]

        if b == len(bands):
            return self.band_name + ' >= %d' % bands[-1]

        return '%d <= %s < %d' % (bands[b - 1], self.band_name, bands[b])

    def append(self, d, sample):
        (x, v) = sample
        band = self.data.setdefault(getband(self.bands, x), {})
        values = band.setdefault(d, [])
        values.append(v)

    def plot(self):
        figure = pyplot.figure()
        figure.text(0.4, 0.95, self.title, horizontalalignment='center', fontsize=24)
        figure.text(0.02, 0.47, 'd')
        figure.text(0.4, 0.02, 's')
        figure.text(0.77, 0.47, 'n')

        data = self.data
        for b in sorted(data.keys()):
            band = data[b]

            x = []
            y = []
            n = []

            for d in sorted(band.keys()):
                samples = band[d]
                m = mean(samples)
                s = stem(samples)

                x.append(d)
                y.append(m)
                n.append(float(len(samples)))

            plot1 = figure.add_subplot(3, 3, 1 + b)
            plot2 = plot1.twinx()

            plot1.text(0.5, 1.05, self.bandname(b), horizontalalignment='center', fontsize=12, transform=plot1.transAxes)

            box = plot1.get_position()
            plot1.set_position([box.x0 * 0.9 - 0.05, box.y0 * 1.05 - 0.04, box.width * 0.79, box.height])
            plot2.set_position([box.x0 * 0.9 - 0.05, box.y0 * 1.05 - 0.04, box.width * 0.79, box.height])

            plot1.axis([min(x), max(x), self.ymin, self.ymax])
            plot2.axis([min(x), max(x), 0, max(n)])

            p1, = plot1.plot(x, y, 'b-')
            p2, = plot2.plot(x, n, 'g-')

        figure.legend([p1, p2], ['Average Difference (d)', 'Sample Size (n)'], loc=(0.78, 0.83))

    def save(self, mode, folder):
        data = self.data
        figure = pyplot.figure()
        figure.text(0.5, 1.1, self.title, horizontalalignment='center', fontsize=12)

        for b in sorted(data.keys()):
            band = data[b]

            x = []
            y = []
            n = []

            for d in sorted(band.keys()):
                samples = band[d]
                m = mean(samples)
                s = stem(samples)

                x.append(d)
                y.append(m)
                n.append(float(len(samples)))

            plot1 = figure.add_subplot(3, 3, b + 1)
            plot2 = plot1.twinx()

            plot1.text(0.5, 1.05, self.bandname(b), horizontalalignment='center', fontsize=12, transform=plot1.transAxes)
            plot1.tick_params('x', labelsize='12')
            plot1.tick_params('y', labelsize='12', pad=20)
            plot2.tick_params('y', labelsize='12', pad=20)

            box = plot1.get_position()
            box_params = [box.x0 * 0.95, box.y0 * 1.2, box.width, box.height]
            plot1.set_position(box_params)
            plot2.set_position(box_params)

            plot1.axis([min(x), max(x), self.ymin, self.ymax])
            plot2.axis([min(x), max(x), 0, self.nmax])
            plot1.set_xticks(range(min(x), max(x) + 1, 2))
            
            if b % 3 != 0:
                plot1.set_yticks([])
            
            if b % 3 != 2:
                plot2.set_yticks([])

            p1, = plot1.plot(x, y, '-', color='black')
            p2, = plot2.plot(x, n, '-', color='gray')
            
        figure.savefig(joinpath(folder, '%s-%s.svg' % (mode, self.title)), bbox_inches='tight')

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
    return samples[0]

#    v = samples[0]
#    e = samples[1]
#
#    m = mean(e)
#    s = stem(e)
#    o = m + s
#
#    resampled = [value for (value, error) in izip(v, e) if error < o]
#    return resampled if len(resampled) > 2 else v

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

def read_normalized(matched, index):
    a = int(matched.group(index + 5))
    b = int(matched.group(index + 1))
    return int(100.0 * float(a - b) / float(min(a, b)))

def read_absolute(matched, index):
    a = int(matched.group(index + 5))
    b = int(matched.group(index + 1))
    return a - b

def read_entry(s, line):
    if line == '\n':
        d = s.d
        samples = resample(s.samples)
        for sample in samples:
            if not s.sample_ok(sample):
                continue

            x = sample[0]
            for i in range(0, 4):
                d[i].append(s.index, (x, sample[i + 1]))

        s.samples = ([], [])
        return s.header

    matched = match(values, line)
    if matched == None:
        raise Exception(line)

    b = s.read_band(matched)
    e = int(matched.group(9))
    w = read_normalized(matched, 0)
    h = read_normalized(matched, 1)
    u = read_absolute(matched, 2)
    v = read_absolute(matched, 3)

    r = (b, w, h, u, v)

    samples = s.samples
    samples[0].append(r)
    samples[1].append(e)

    return read_entry

class statistics(object):
    def __init__(self, header, band_name, bands, band_reader, sample_checker, *plots):
        self.index = 0
        self.header = header
        self.samples = ([], [])
        self.read_band = (lambda s, matched: band_reader(matched)).__get__(self)
        self.sample_ok = (lambda s, sample: sample_checker(sample, bands)).__get__(self)
        self.d = [difference(band_name, bands, *plot) for plot in plots]

    def load(self, path):
        state = self.header
        for line in open(path):
            state = state(self, line)

    def plot(self):
        for d in self.d:
            d.plot()

        pyplot.show()

    def save(self, mode, folder):
        for d in self.d:
            d.save(mode, folder)

def statistics_forward_u():
    bands = [71, 142, 213, 284, 355, 426, 497, 568]
    plots = [('Width', 0, 70, 250), ('Height', 0, 70, 250), ('U', -120, 70, 250), ('V', -120, 70, 250)]
    s = statistics(read_header_forward, 'u', bands, band_u_read, band_u_check_forward, *plots)
    s.load('/home/erios/Roboken/Yamabiko/2013-3/Data/forward-2014-01-22-01.txt')
    #s.plot()
    s.save('straight', '/home/erios/Roboken/Yamabiko/2013-3')

def statistics_forward_v():
    bands = [260, 270, 280, 290, 300, 315, 335, 350]
    plots = [('Width', -1, 10), ('Height', -8, 14), ('U', -70, 70), ('V', -10, 30)]
    s = statistics(read_header_forward, bands, 'v', band_v_read, *plots)
    s.load('/home/erios/Projects/Data/Landmarks/forward-2014-01-16-01.txt')
    s.plot()

def statistics_sideways_u():
    bands = [71, 142, 213, 284, 355, 426, 497, 568]
    plots = [('Width', -50, 50, 200), ('Height', -50, 50, 200), ('U', -250, 30, 200), ('V', -250, 30, 200)]
    s = statistics(read_header_sideways, 'u', bands, band_u_read, band_u_check_sideways, *plots)
    s.load('/home/erios/Roboken/Yamabiko/2013-3/Data/sideways-2014-01-22-01.txt')
    #s.plot()
    s.save('sideways', '/home/erios/Roboken/Yamabiko/2013-3')

def statistics_sideways_v():
    bands = [260, 270, 280, 290, 300, 315, 335, 350]
    s = statistics(read_header_sideways, bands, 'v', band_v_read, ('Width', -3, 8), ('Height', -2, 16), ('U', -180, 0), ('V', -16, 35))
    s.load('/home/erios/Projects/Data/Landmarks/sideways-2014-01-16-01.txt')
    s.plot()

def statistics_turning_u():
    bands = [71, 142, 213, 284, 355, 426, 497, 568]
    plots = [('Width', -50, 50, 1300), ('Height', -50, 50, 1300), ('U', -600, 30, 1300), ('V', -600, 30, 1300)]
    s = statistics(read_header_turning, 'u', bands, band_u_read, band_u_check_turning, *plots)
    s.load('/home/erios/Roboken/Yamabiko/2013-3/Data/turning-2014-01-22-01.txt')
    #s.plot()
    s.save('turning', '/home/erios/Roboken/Yamabiko/2013-3')

def statistics_turning_v():
    bands = [260, 270, 280, 290, 300, 315, 335, 350]
    s = statistics(read_header_turning, bands, 'v', band_v_read, ('Width', -20, 12), ('Height', -15, 20), ('U', -450, -50), ('V', -70, 120))
    s.load('/home/erios/Projects/Data/Landmarks/turning-2014-01-16-01.txt')
    s.plot()

def main():
    statistics_sideways_u()

if __name__ == '__main__':
    main()
