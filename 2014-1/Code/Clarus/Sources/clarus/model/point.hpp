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

#ifndef CLARUS_MODEL_POINT_HPP
#define CLARUS_MODEL_POINT_HPP

#include <clarus/core/list.hpp>
#include <clarus/core/math.hpp>

#include <opencv2/opencv.hpp>

#include <iostream>

namespace clarus {
    class Point;

    /*
    Computes the angle of the line segment passing through the given point and the origin.
    */
    double angle2d(const Point &point);

    /*
    Computes the angle of the line segment passing through two points.
    */
    double angle2d(const Point &p0, const Point &p1);

    /*
    Returns the rectangle bounds of a point list.
    */
    cv::Rect bounds2d(const List<Point> &points);

    /*
    Computes the point's distance from the origin.
    */
    double distance(const Point &p0, int upto = INT_MAX);

    /*
    Computes the distance between two points.
    */
    double distance(const Point &p0, const Point &p1, int upto = INT_MAX);

    /*
    Computes the point's squared distance from the origin.
    */
    double distance2(const Point &p0, int upto = INT_MAX);

    /*
    Computes the squared distance between two points.
    */
    double distance2(const Point &p0, const Point &p1, int upto = INT_MAX);

    bool inrange(const Point &pt, const Point &p0, const Point &pn);

    Point mean(const List<Point> &points);

    /*
    Creates a new point at the origin of the space of given dimension.
    */
    Point Point0(size_t dimension);

    /*
    Creates a new 2D point.
    */
    Point Point2D(double x0, double x1);

    /*
    Creates a new 2D point.
    */
    Point Point2D(const cv::Point2f &p);

    /*
    Creates a new 2D point.
    */
    List<Point> Point2D(const List<cv::Point2f> &p);

    /*
    Creates a new 3D point.
    */
    Point Point3D(double x0, double x1, double x2);

    /*
    Converts a 2D point to an OpenCV point.
    */
    cv::Point2f PointCV(const Point &p);
}

/*
A point in n-dimensional cartesian space.
*/
class clarus::Point {
    /* Point coordinates. */
    List<double> point;

public:
    /*
    Creates a new point of dimension 0.
    */
    Point();

    /*
    Custom copy-constructor.
    */
    Point(const Point &that);

    /*
    Creates a new point of given dimension.
    */
    Point(size_t dimensions, ...);

    /*
    Creates a new point fromthe given list.
    */
    Point(const List<double> &point);

    /*
    Creates a new point at the average position between two others.
    */
    Point(const Point &p0, const Point &p1);

    /*
    Virtual destructor. Enforces polymorphism. Do not remove.
    */
    virtual ~Point();

    const List<double> &operator * () const;

    double &operator [] (int index);

    const double &operator [] (int index) const;

    bool operator == (const Point &that) const;

    bool operator != (const Point &that) const;

    Point &operator = (Point that);

    Point &operator += (const Point &that);

    Point operator + (const Point &that) const;

    Point operator - (const Point &that) const;

    Point operator * (const Point &that) const;

    Point operator / (const Point &that) const;

    Point operator + (double v) const;

    Point operator - (double v) const;

    Point operator * (double v) const;

    Point operator / (double v) const;

    size_t dimension() const;
};

std::ostream &operator << (std::ostream &out, const clarus::Point &point);

std::istream &operator >> (std::istream &in, clarus::Point &point);

#endif
