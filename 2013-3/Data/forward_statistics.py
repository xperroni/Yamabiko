#!/usr/bin/env python

from re import match

from numpy import array, max, mean, std
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import pyplot

place = r'\(\s+(\d+),\s+\d+,\s+6\) \-> \(\s+(\d+),\s+(\d+),\s+6\)'
values = r'\(\s+(\d+\.\d+),\s+(\d+\.\d+),\s+(\d+\.\d+),\s+(\d+\.\d+)\)'
exp = place + ' : ' + values

data = {}

def add(data, d, values):
    if not d in data:
        data[d] = []
    
    data[d].append(values)

for line in open('/home/helio/Projects/Data/Landmarks/forward-2014-01-09-01.txt'):
    found = match(exp, line)
    u1 = int(found.group(1))
    u2 = int(found.group(2))
    v = int(found.group(3))
    w = float(found.group(4))
    h = float(found.group(5))
    x = float(found.group(6))
    y = float(found.group(7))
    
    add(data, u2 - u1, (w, h, x, y))

titles = ['Width', 'Height', 'X', 'Y']

for i in range(0, 4):
    w = []
    y = []
    o = []
    n = []

    for (d, v) in data.iteritems():
        x = [p[i] for p in v]

        m = mean(x)
        s = std(x) / (float(len(x)) ** 0.5)
        w.append(m - s)
        y.append(m)
        o.append(m + s)
        n.append(float(len(x)))

    n = array(n)

    pyplot.figure(i + 1)
    plot1 = pyplot.subplot(111)
    plot1.text(0.5, 1.05, titles[i], horizontalalignment='center', transform=plot1.transAxes)
    plot1.axis([0, 8, 0, max(o) * 1.05])
    plot1.set_xlabel('s', labelpad=20)
    plot1.set_ylabel('d', labelpad=20, rotation='horizontal')
    plot2 = plot1.twinx()
    plot2.axis([0, 8, 0, max(n)])
    plot2.set_ylabel('n', labelpad=20, rotation='horizontal')
    p1, = plot1.plot(y, 'b-')
    p2, = plot1.plot(w, 'r-')
    plot1.plot(o, 'r-')
    p3, = plot1.plot(n * (max(o) / max(n)), 'g-')
    
    box = plot1.get_position()
    plot1.set_position([box.x0, box.y0, box.width * 0.79, box.height])
    plot2.set_position([box.x0, box.y0, box.width * 0.79, box.height])
    plot1.legend([p1, p2, p3], ['Absolute Difference (d)', 'Standard Error (d)', 'Sample Size (n)'], loc=2, bbox_to_anchor=(1.08, 1))

pyplot.show()

