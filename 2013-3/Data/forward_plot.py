#!/usr/bin/env python

from re import match

from mpl_toolkits.mplot3d import Axes3D
from matplotlib import pyplot

place = r'\(\s+(\d+),\s+\d+,\s+6\) \-> \(\s+(\d+),\s+(\d+),\s+6\)'
values = r'\(\s+(\d+\.\d+),\s+(\d+\.\d+),\s+(\d+\.\d+),\s+(\d+\.\d+)\)'
exp = place + ' : ' + values

xs = []
ys = []
zs = []

for line in open('/home/helio/Projects/Data/Landmarks/forward-2013-12-03-01.txt'):
    found = match(exp, line)
    u1 = int(found.group(1))
    u2 = int(found.group(2))
    v = int(found.group(3))
    w = float(found.group(4))
    h = float(found.group(5))
    x = float(found.group(6))
    y = float(found.group(7))
    
    xs.append(v)
    ys.append(u2 - u1)
    zs.append(w)

display = pyplot.figure()
plot = display.add_subplot(111, projection='3d')
plot.scatter(xs, ys, zs)
plot.set_xlabel('v')
plot.set_ylabel('u2 - u1')
plot.set_zlabel('w')

pyplot.show()

