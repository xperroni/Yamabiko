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

#include <clarus/model/point.hpp>
using clarus::Point;

#include <clarus/core/list.hpp>
using clarus::List;
using clarus::ListIterator;
using clarus::ListIteratorConst;

#include <boost/bind.hpp>
#include <boost/function.hpp>

#include <algorithm>
#include <cstdarg>
#include <cmath>
#include <vector>

typedef boost::function<double(double, double)> PointOp;

Point::Point() {
    // Nothing to do.
}

Point::Point(const Point &that):
    point(*that.point)
{
    // Nothing to do.
}

Point::Point(size_t dimensions, ...) {
    va_list args;
    va_start(args, dimensions);

    for (size_t i = 0; i < dimensions; i++) {
        double value = va_arg(args, double);
        point.append(value);
    }

    va_end(args);
}

Point::Point(const List<double> &values) {
    for (ListIteratorConst<double> i(values); i.more();) {
        double x = i.next();
        point.append(x);
    }
}

Point::Point(const Point &p0, const Point &p1) {
    ListIteratorConst<double> i(*p0);
    ListIteratorConst<double> j(*p1);
    while (i.more() && j.more()) {
        double u = i.next();
        double v = j.next();
        point.append((u + v) / 2.0);
    }

    while (i.more()) {
        double u = i.next();
        point.append(u / 2.0);
    }

    while (j.more()) {
        double v = j.next();
        point.append(v / 2.0);
    }
}

Point::~Point() {
    // Nothing to do.
}

const List<double> &Point::operator * () const {
    return point;
}

double &Point::operator [] (int index) {
    return point[index];
}

const double &Point::operator [] (int index) const {
    return point[index];
}

bool Point::operator == (const Point &that) const {
    ListIteratorConst<double> i(this->point);
    ListIteratorConst<double> j(that.point);
    while (i.more() && j.more()) {
        if(i.next() != j.next()) {
            return false;
        }
    }

    return true;
}

bool Point::operator != (const Point &that) const {
    return !(*this == that);
}

Point &Point::operator = (Point that) {
    // Implementation of the copy-swap idiom
    // See: http://www.cplusplus.com/articles/y8hv0pDG/
    std::swap(*(this->point), *(that.point));
    return *this;
}

Point &Point::operator += (const Point &that) {
    ListIterator<double> i(this->point);
    ListIteratorConst<double> j(that.point);
    for (; i.more() && j.more(); i.next(), j.next()) {
        *i += *j;
    }

    return *this;
}

Point Point::operator + (const Point &that) const {
    return Point(vectorize(clarus::add, this->point, that.point));
}

Point Point::operator - (const Point &that) const {
    return Point(vectorize(clarus::sub, this->point, that.point));
}

Point Point::operator * (const Point &that) const {
    return Point(vectorize(clarus::mul, this->point, that.point));
}

Point Point::operator / (const Point &that) const {
    return Point(vectorize(clarus::div, this->point, that.point));
}

Point Point::operator + (double v) const {
    return Point(vectorize(boost::bind(clarus::add, _1, v), this->point));
}

Point Point::operator - (double v) const {
    return Point(vectorize(boost::bind(clarus::sub, _1, v), this->point));
}

Point Point::operator * (double v) const {
    return Point(vectorize(boost::bind(clarus::mul, _1, v), this->point));
}

Point Point::operator / (double v) const {
    return Point(vectorize(boost::bind(clarus::div, _1, v), this->point));
}

size_t Point::dimension() const {
    return point.size();
}

double clarus::angle2d(const Point &point) {
    static const Point center(Point2D(0, 0));

    return angle2d(center, point);
}

double clarus::angle2d(const Point &p0, const Point &p1) {
    if (p0[0] > p1[0]) {
        return angle2d(p1, p0);
    }

    double dx = p1[0] - p0[0];
    double dy = p1[1] - p0[1];
    return atan2(dy, dx);
}

cv::Rect clarus::bounds2d(const List<Point> &points) {
    double x_min = DBL_MAX;
    double y_min = DBL_MAX;
    double x_max = 0;
    double y_max = 0;
    for (ListIteratorConst<Point> i(points); i.more();) {
        const Point &p = i.next();
        double x = p[0];
        double y = p[1];
        x_min = std::min(x, x_min);
        y_min = std::min(y, y_min);
        x_max = std::max(x, x_max);
        y_max = std::max(y, y_max);
    }

    return cv::Rect(x_min, y_min, x_max - x_min, y_max - y_min);
}

double clarus::distance(const Point &p0, int upto) {
    return pow(distance2(p0, upto), 0.5);
}

double clarus::distance(const Point &p0, const Point &p1, int upto) {
    return pow(distance2(p0, p1, upto), 0.5);
}

double clarus::distance2(const Point &p0, int upto) {
    double d = 0.0;
    int k = 0;
    for (ListIteratorConst<double> i(*p0); i.more() && k < upto; k++) {
        d += pow(i.next(), 2.0);
    }

    return d;
}

double clarus::distance2(const Point &p0, const Point &p1, int upto) {
    if (p0.dimension() < p1.dimension()) {
        return distance2(p1, p0, upto);
    }

    int k = 0;
    double d = 0.0;
    ListIteratorConst<double> i(*p0);
    ListIteratorConst<double> j(*p1);
    while (i.more() && j.more() && k++ < upto) {
        double u = i.next();
        double v = j.next();
        d += pow(u - v, 2.0);
    }

    while (i.more() && k++ < upto) {
        double u = i.next();
        d += pow(u, 2.0);
    }

    return d;
}

bool clarus::inrange(const Point &pt, const Point &p0, const Point &pn) {
    for (int i = 0, m = pt.dimension(); i < m; i++) {
        double t = pt[i];
        double a = p0[i];
        double n = pn[i];
        if (!(a <= t && t < n)) {
            return false;
        }
    }

    return true;
}

Point clarus::mean(const List<Point> &points) {
    double n = points.size();
    if (n == 0.0) {
        return Point();
    }

    Point sumd = Point0(points[0].dimension());
    for (ListIteratorConst<Point> i(points); i.more();) {
        const Point &point = i.next();
        sumd += point;
    }

    return sumd / n;
}

Point clarus::Point0(size_t dimension) {
    return Point(List<double>(dimension, 0.0));
}

Point clarus::Point2D(double x0, double x1) {
    return Point(2, x0, x1);
}

Point clarus::Point2D(const cv::Point2f &p) {
    return Point2D(p.x, p.y);
}

List<Point> clarus::Point2D(const List<cv::Point2f> &p) {
    List<Point> points;
    for(ListIteratorConst<cv::Point2f> i(p); i.more();) {
        const cv::Point2f &q = i.next();
        points.append(Point2D(q));
    }

    return points;
}

Point clarus::Point3D(double x0, double x1, double x2) {
    return Point(3, x0, x1, x2);
}

cv::Point2f clarus::PointCV(const Point &p) {
    return cv::Point2f(p[0], p[1]);
}

std::ostream &operator << (std::ostream &out, const Point &point) {
    int n = point.dimension();
    if (n == 0) {
        out << "()";
        return out;
    }

    out << "(";
    for (int i = 0;;) {
        out << point[i];
        if (++i >= n) {
            break;
        }

        out << ", ";
    }

    out << ")";

    return out;
}

std::istream &operator >> (std::istream &in, clarus::Point &point) {
    char c = '\0';
    in >> c;

    List<double> values;
    while (c != ')') {
        double v = 0.0;
        in >> v >> c;
        values.append(v);
    }

    point = Point(values);

    return in;
}
