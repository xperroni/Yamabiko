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

#include <algorithm>

point::cluster::cluster(int _range, int i0, int j0):
    range(_range)
{
    append(point(i0, j0));
    i_min = i0 - 2;
    i_max = i0 + 2;
    j_min = j0 - 2;
    j_max = j0 + 2;
}

bool point::cluster::add(const point &p) {
    append(p);
    i_min = std::min(i_min, (int) p[0]);
    i_max = std::max(i_max, (int) p[0]);
    j_min = std::min(j_min, (int) p[1]);
    j_max = std::max(j_max, (int) p[1]);
    return true;
}

bool point::cluster::operator < (const cluster &other) const {
    return size() < other.size();
}

bool point::cluster::add(int i, int j) {
    point p(i, j);
    if (
        i_min <= i && i <= i_max &&
        j_min <= j && j <= j_max
    ) {
        return add(p);
    }

    for (PointIterator k(*this); k.more(); k.next()) {
        if (p.distance(*k) <= range) {
            return add(p);
        }
    }

    return false;
}

bool point::cluster::inside(int i, int j) const {
    return (
        i_min <= i && i <= i_max &&
        j_min <= j && j <= j_max
    );
}

bool point::cluster::inside(const cluster &other) const {
    return (
        inside(other.i_min, other.j_min) &&
        inside(other.i_min, other.j_max) &&
        inside(other.i_max, other.j_min) &&
        inside(other.i_max, other.j_max)
    );
}

bool point::cluster::merge(const cluster &other) {
    if (!this->inside(other) && !other.inside(*this)) {
        return false;
    }

    for (PointIteratorConst k(*this); k.more(); k.next()) {
        add(*k);
    }

    return true;
}

void point::cluster::draw(cv::Mat &image) const {
    static cv::Scalar RED(0, 0, 255);
    static cv::Scalar YELLOW(0, 255, 255);

    cv::Point a(j_min, i_min);
    cv::Point b(j_max, i_max);
    cv::rectangle(image, a, b, RED);
/*
    for (const_iterator i = begin(), n = end(); i != n; ++i) {
        const point &p = *i;
        circle(image, cv::Point(p[1], p[0]), 1, YELLOW);
    }
*/
}

cv::Mat point::cluster::extract(const cv::Mat &src) const {
    cv::Rect roi(
        std::max(j_min, 0),
        std::max(i_min, 0),
        std::min(j_max - j_min, src.cols),
        std::min(i_max - i_min, src.rows)
    );

    cv::Mat region(src, roi);
    return region;
}

void point::cluster::mask_points(cv::Mat &data, int label) const {
    for (PointIteratorConst k(*this); k.more();) {
        const point &p = k.next();
        data.at<int>(p[0], p[1]) = label;
    }
}

void point::cluster::mask_region(cv::Mat &data) const {
    static cv::Scalar MAYBE = cv::Scalar::all(0);

    cv::Point a(
        std::max(0, j_min - 1),
        std::max(0, i_min - 1)
    );

    cv::Point b(
        std::min(data.cols, j_max + 1),
        std::min(data.rows, i_max + 1)
    );

    cv::rectangle(data, a, b, MAYBE, CV_FILLED);
}

point::cloud::cloud(int _range):
    range(_range)
{
    // Nothing to do.
}

void point::cloud::add(int i, int j) {
    for (ClusterIterator k(*this); k.more(); k.next()) {
        if (k->add(i, j)) {
            merge(k);
            return;
        }
    }

    append(cluster(range, i, j));
}

void point::cloud::merge(ClusterIterator &added) {
    point::cluster &c = added.current();
    for (ClusterIterator k(*this); k.more(); k.next()) {
        if (k == added) {
            continue;
        }

        if (k->merge(c)) {
            added.erase();
            return;
        }
    }
}

void point::cloud::draw(cv::Mat &image, size_t size) const {
    for (ClusterIteratorConst k(*this); k.more();) {
        const cluster &c = k.next();
        if (c.size() >= size) {
            c.draw(image);
        }
    }
}

cv::Mat point::cloud::mask(const cv::Size &size, size_t threshold) const {
    cv::Mat data(size, CV_32S, cv::Scalar::all(255));
    for (ClusterIteratorConst k(*this); k.more();) {
        const cluster &c = k.next();
        if (c.size() < threshold) {
           continue;
        }

        c.mask_region(data);
    }

    int label = 1;
    for (ClusterIteratorConst k(*this); k.more(); ++label) {
        const cluster &c = k.next();
        if (c.size() < threshold) {
           continue;
        }

        c.mask_points(data, label);
    }

    return data;
}

point::gaussian::gaussian(double _sigma, const cv::Size &_size):
    sigma(_sigma),
    size(_size)
{
    // Nothing to do.
}

int point::gaussian::sample(int c, int n) const {
    static cv::RNG rng;

    int i = rng.gaussian(sigma) + c;
    if (i < 0) {
        return 0;
    }
    else if (i >= n) {
        return n - 1;
    }

    return i;
}

point point::gaussian::operator () () const {
    int in = size.height;
    int ic = in / 2;

    int jn = size.width;
    int jc = jn / 2;

    return point(
        sample(ic, in),
        sample(jc, jn)
    );
}

std::set<point> point::gaussian::operator () (size_t n) const {
    std::set<point> points;
    while (points.size() < n) {
        points.insert((*this)());
    }

    return points;
}

point::sampler::sampler() {
    // Nothing to do.
}

point::sampler::sampler(size_t n, distribution p):
    points(p(n))
{
    // Nothing to do.
}

point::range::range():
    p0(0, 0),
    pc(0, 0),
    pn(0, 0)
{
    // Nothing to do.
}

point::range::range(size_t i0, size_t j0, size_t in, size_t jn):
    p0(i0, j0),
    pc((i0 + in) / 2, (j0 + jn) / 2),
    pn(in, jn)
{
    // Nothing to do.
}

point::range::operator cv::Rect () const {
    int x = p0[1];
    int y = p0[0];
    int width = pn[1] - x;
    int height = pn[0] - y;
    return cv::Rect(x, y, width, height);
}

bool point::range::contains(const point &p) const {
    return (
        p0[0] <= p[0] && p[0] < pn[0] &&
        p0[1] <= p[1] && p[1] < pn[1]
    );
}

bool point::range::intersects(const range &that) const {
    return !(
        pn[0] <= that.p0[0] || that.pn[0] <= p0[0] ||
        pn[1] <= that.p0[1] || that.pn[1] <= p0[1]
    );
}

point::point(size_t i, size_t j):
    Index<2>(i, j)
{
    // Nothing to do.
}

point::point(const cv::Rect &rect):
    Index<2>(rect.y + rect.height / 2, rect.x + rect.width / 2)
{
    // Nothing to do.
}

point::operator cv::Point () const {
    const point &p = *this;
    return cv::Point(p[1], p[0]);
}
