#!/usr/bin/env python

from numpy import array, max, mean, std
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import pyplot

data = array([9434, 5315, 5394, 7166, 6538, 8029, 6087, 8100, 13044, 10347, 9743])

m = mean(data)
s = std(data) / (float(len(data)) ** 0.5)

pyplot.plot(data, 'b*')
pyplot.axhline(m, color='g')
pyplot.axhline(m + s, 0, len(data), color='r')
pyplot.axhline(m - s, 0, len(data), color='r')
pyplot.show()
