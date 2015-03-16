/*
Copyright (c) Helio Perroni Filho <xperroni@gmail.com>

This file is part of Clarus.

Clarus is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Clarus is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Clarus. If not, see <http://www.gnu.org/licenses/>.
*/

#include <clarus/model/cluster.hpp>
using clarus::Cluster;
using clarus::Point;

#include <boost/bind.hpp>

#include <cmath>

#include <stdexcept>
using std::runtime_error;

Cluster::Cluster():
    List<Point>()
{
    // nothing to do.
}

Cluster::~Cluster() {
    // Nothing to do.
}

Cluster Cluster::clone() const {
    Cluster cloned;
    for (ListIteratorConst<Point> i(*this); i.more();) {
        Point point = i.next();
        cloned.append(point);
    }

    return cloned;
}

Point Cluster::center() const {
    if (size() < 1) {
        throw runtime_error("Empty cluster, no center defined");
    }

    size_t n = at(0).dimension();
    Point point(Point0(n));

    for (ListIteratorConst<Point> i(*this); i.more();) {
        point += i.next();
    }

    return point / size();
}

Point Cluster::closest() const {
    if (size() < 1) {
        throw runtime_error("Empty cluster, no closest point defined");
    }

    size_t n = at(0).dimension();
    Point p(Point0(n));

    double d = std::numeric_limits<double>::max();
    for (ListIteratorConst<Point> i(*this); i.more();) {
        const Point &q = i.next();
        double e = distance(q);
        if (e < d) {
            p = q;
            d = e;
        }
    }

    return p;
}

Point Cluster::farthest() const {
    if (size() < 1) {
        throw runtime_error("Empty cluster, no farthest point defined");
    }

    size_t n = at(0).dimension();
    Point p(Point0(n));

    double d = 0;
    for (ListIteratorConst<Point> i(*this); i.more();) {
        const Point &q = i.next();
        double e = distance(q);
        if (e > d) {
            p = q;
            d = e;
        }
    }

    return p;
}

double Cluster::spread() const {
    if (size() < 1) {
        throw runtime_error("Empty cluster, no spread defined");
    }

    Point c(center());
    double d = 0;
    for (ListIteratorConst<Point> i(*this); i.more();) {
        double e = distance(c, i.next());
        if (e > d) {
            d = e;
        }
    }

    return d;
}



double clarus::angle2d(const Cluster &cluster) {
    Point p = cluster.closest();
    return atan2(p[1], p[0]);
}

double clarus::distance(const Cluster &cluster) {
    return distance(cluster.closest());
}

double clarus::distance(const Cluster &c1, const Cluster &c2) {
    double d = std::numeric_limits<double>::max();
    for (ListIteratorConst<Point> i(c1); i.more();) {
        const Point &p1 = i.next();
        for (ListIteratorConst<Point> j(c2); j.more();) {
            const Point &p2 = j.next();
            double e = distance(p1, p2);
            if (e < d) {
                d = e;
            }
        }
    }

    return d;
}

static bool axis_cmp(int axis, const clarus::Point &p0, const clarus::Point &p1) {
    return p0[axis] < p1[axis];
}

Point clarus::median(const Cluster &cluster, int axis) {
    List<Point> points = sorted(cluster, boost::bind(axis_cmp, axis, _1, _2));
    int i = points.size() / 2;
    return points[i];
}
