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

#include "difference.hpp"

#include <stdexcept>

inline double distance(const List<cv::Mat> &a, const List<cv::Mat> &b) {
    double total = 0;
    for (ListIteratorConst<cv::Mat> i(a), j(b); i.more();) {
        const cv::Mat &u = i.next();
        const cv::Mat &v = j.next();

        cv::Mat d;
        cv::absdiff(u, v, d);
        total += cv::sum(d)[0];
    }

    return total;
}

inline double distance(const dejavu::Scene &s1, const dejavu::Scene &s2, int i, int j) {
    const cv::Mat &l1 = s1.landmark(i);
    const cv::Mat &l2 = s2.landmark(j);

    size_t w1 = l1.cols;
    size_t h1 = l1.rows;

    size_t w2 = l2.cols;
    size_t h2 = l2.rows;

    cv::Size size((w1 + w2) / 2, (h1 + h2) / 2);

    cv::Mat g1, g2;
    cv::resize(l1, g1, size);
    cv::resize(l2, g2, size);

    return distance(colors::BGRL(g1), colors::BGRL(g2));
}

inline double distance(dejavu::Mode mode, const dejavu::Scene &s1, const dejavu::Scene &s2, int i, int j) {
    const cv::Rect &r1 = s1.region(i);
    const cv::Rect &r2 = s2.region(j);

    if (!mode(r1, r2)) {
        return std::numeric_limits<double>::max();
    }

    return distance(s1, s2, i, j);
}

inline cv::Mat distances(dejavu::Mode mode, const dejavu::Scene &s1, const dejavu::Scene &s2) {
    int rows = s1.size();
    int cols = s2.size();
    cv::Mat d(rows, cols, CV_64F, cv::Scalar(0));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            d.at<double>(i, j) = distance(mode, s1, s2, i, j);
        }
    }

    return d;
}

inline void assign(
    List<Assignment> &a,
    List<double> &e,
    const dejavu::Scene &s1,
    const dejavu::Scene &s2,
    dejavu::Mode mode,
    bool trim
) {
    bool s1_first = (s2.size() <= s1.size());
    cv::Mat d = (s1_first ? distances(mode, s1, s2) : distances(mode, s2, s1));
    a = munkres(d, s1_first);

    for (ListIterator<Assignment> i(a); i.more(); i.next()) {
        int u = i->first;
        int v = i->second;
        double error = (s1_first ? d.at<double>(u, v) : d.at<double>(v, u));
        e.append(error);
    }

    if (!trim) {
        return;
    }

    size_t n = e.size();
    cv::Mat mean, stdev;
    cv::meanStdDev(*e, mean, stdev);
    double o = mean.at<double>(0) + stdev.at<double>(0) / pow(n, 0.5);

    List<Assignment> a2;
    List<double> e2;
    for (size_t i = 0; i < n; i++) {
        double error = e[i];
        if (error < o) {
            a2.append(a[i]);
            e2.append(error);
        }
    }

    a = a2;
    e = e2;
}

dejavu::Difference::Difference(
    const Position &_p,
    const Scene &s1,
    const Scene &s2,
    Mode mode,
    bool trim
):
    p(_p)
{
    List<Assignment> assignments;
    List<double> errors;
    assign(assignments, errors, s1, s2, mode, trim);
    for (int i = 0, n = assignments.size(); i < n; i++) {
        double e = errors[i];
        if (e == std::numeric_limits<double>::max()) {
            continue;
        }

        Assignment &a = assignments[i];
        const cv::Rect &roi1 = s1.region(a.first);
        const cv::Rect &roi2 = s2.region(a.second);
        changes.append(Change(e, roi1, roi2));
        matches.append(a);
    }
}

const dejavu::Change &dejavu::Difference::change(size_t index) const {
    return changes.at(index);
}

double dejavu::Difference::error(size_t index) const {
    return changes.at(index).error();
}

const Assignment &dejavu::Difference::match(size_t index) const {
    return matches.at(index);
}

const dejavu::Position &dejavu::Difference::position() const {
    return p;
}

size_t dejavu::Difference::size() const {
    return changes.size();
}
