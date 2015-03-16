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

#include <clarus/model/line2d.hpp>
using clarus::Line2D;

#include <clarus/model/point.hpp>
using clarus::Point;

#include <stdexcept>
using std::runtime_error;

Line2D::Line2D():
    a(NAN),
    b(NAN),
    t(NAN)
{
    // Nothing to do.
}

Line2D::Line2D(const Point &p0, const Point &p1, double dx) {
    fit(p0, p1, dx);
}

Line2D::~Line2D() {
    // Nothing to do.
}

double Line2D::operator () (double x) const {
    return a * x + b;
}

void Line2D::fit(const Point &p0, const Point &p1, double dx) {
    if (p0.dimension() < 2 || p1.dimension() < 2) {
        throw runtime_error("Fitted points must have dimension >= 2");
    }

    double x0 = p0[0];
    double y0 = p0[1];

    double x1 = p1[0];
    double y1 = p1[1];

    if (x0 > x1) {
        fit(p1, p0);
        return;
    }

    if (fabs(x1 - x0) < dx) {
        a = NAN;
        b = x0;
        t = M_PI_2;
    }
    else {
        a = (y1 - y0) / (x1 - x0);
        b = y0 - x0 * a;
        t = atan(a);
        if (t < 0) {
            t = M_PI + t;
        }
    }
}

double Line2D::angle() const {
    return t;
}

double Line2D::intercept() const {
    return b;
}

double Line2D::slope() const {
    return a;
}

double clarus::distance(const Line2D &line, const Point &point) {
    if (point.dimension() < 2) {
        throw runtime_error("Point must have dimension >= 2");
    }

    double a = line.slope();
    double b = line.intercept();

    double x = point[0];
    double y = point[1];

    return (isnan(a) ? fabs(x - b) : fabs(y - a * x - b) / sqrt(pow(a, 2.0) + 1));
}

double clarus::distance(const Line2D &l0, const Line2D &l1) {
    return distance(
        Point2D(l0.angle(), l0.intercept()),
        Point2D(l1.angle(), l1.intercept())
    );
}
