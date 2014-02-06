/*
Copyright (c) Helio Perroni Filho <xperroni@gmail.com>

This file is part of Dejavu.

Dejavu is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Dejavu is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Dejavu. If not, see <http://www.gnu.org/licenses/>.
*/

#include "movement.hpp"

dejavu::Movement::Movement():
    p(0, 0, 0)
{
    // Nothing to do.
}

dejavu::Movement::Movement(const Position &_p):
    p(_p)
{
    // Nothing to do.
}

dejavu::Movement::Movement(const Position &p1, const Position &p2):
    p(p1)
{
    guesses.append(p2 - p1);
    errors.append(0);
}

dejavu::Movement::Movement(const Movement &m1, const Movement &m2):
    p(0, 0, 0)
{
    double e1 = cv::sum(*(m1.errors))[0];
    double e2 = cv::sum(*(m2.errors))[0];
    *this = (e1 < e2 ? m1 : -m2);
}

inline void normalize(cv::Mat &w) {
    double minValue, maxValue;
    cv::minMaxLoc(w, &minValue, &maxValue);

    w = 1 + maxValue - w;
    cv::normalize(w, w);
}

dejavu::Movement::Movement(const Movement &f, const Movement &s, const Movement &u):
    p(f.p)
{
    size_t n = f.size() + s.size() + u.size();
    cv::Mat w(1, n, CV_64F, cv::Scalar(0));
    cv::Mat x(1, n, CV_64F, cv::Scalar(0));
    cv::Mat y(1, n, CV_64F, cv::Scalar(0));
    cv::Mat t(1, n, CV_64F, cv::Scalar(0));

    int j = 0;
    double e = 0.0;
    const Movement *all[] = {&f, &s, &u, NULL};
    for (const Movement **m = all; *m != NULL; m++) {
        const Movement &movement = **m;
        for (int i = 0, n = movement.size(); i < n; i++, j++) {
            e += w.at<double>(j) = movement.error(i);

            const Directions &d = movement.directions(i);
            x.at<double>(j) = d[0];
            y.at<double>(j) = d[1];
            t.at<double>(j) = d[2];
        }
    }

    normalize(w);
    double z = cv::sum(w)[0];
    Directions d(
        cv::sum(x.mul(w))[0] / z,
        cv::sum(y.mul(w))[0] / z,
        cv::sum(t.mul(w))[0] / z
    );

    guesses.append(d);
    errors.append(e / n);
}

dejavu::Movement dejavu::Movement::operator - () const {
    Movement r(p);

    for (int i = 0, n = size(); i < n; i++) {
        r.append(-directions(i), error(i));
    }

    return r;
}

void dejavu::Movement::append(const Directions &d, double e) {
    guesses.append(d);
    errors.append(e);
}

const dejavu::Directions &dejavu::Movement::directions(size_t index) const {
    return guesses.at(index);
}

const dejavu::Position &dejavu::Movement::origin() const {
    return p;
}

dejavu::Position dejavu::Movement::destination() const {
    return p + guesses.last();
}

double dejavu::Movement::error(size_t index) const {
    return errors.at(index);
}

size_t dejavu::Movement::size() const {
    return guesses.size();
}

std::ostream &operator << (std::ostream& out, const dejavu::Position& p) {
    return (out << "(" << p[0] << ", " << p[1] << ", " << p[2] << ")");
}
