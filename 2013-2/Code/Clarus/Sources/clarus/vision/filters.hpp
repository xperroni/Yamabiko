/*
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
along with Clarus.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CLARUS_VISION_EDGES_HPP
#define CLARUS_VISION_EDGES_HPP

#include <clarus/core/index.hpp>

#include <opencv2/opencv.hpp>

#include <set>

namespace sobel {
    cv::Mat filter(const cv::Mat &l);
}

namespace gaussian {
    class sampler;
}

class gaussian::sampler {
    typedef Index<2> point;

    std::set<point> points;

public:
    sampler();

    sampler(size_t n, double sigma, const cv::Size &size);

    template<class P> cv::Mat minchinton2bgr(const cv::Mat &sampling) const;

    template<class P> cv::Mat sample(const cv::Mat &sampling) const;

    template<class P> cv::Mat sample2d(const cv::Mat &sampling) const;
};

template<class P> cv::Mat gaussian::sampler::sample(const cv::Mat &sampling) const {
    cv::Mat samples(points.size(), 1, sampling.type(), cv::Scalar::all(0));

    int l = 0;
    for (std::set<point>::iterator k = points.begin(), n = points.end(); k != n; ++k) {
        const point &p = *k;
        samples.at<P>(l++) = sampling.at<P>(p[0], p[1]);
    }

    return samples;
}

template<class P> cv::Mat gaussian::sampler::sample2d(const cv::Mat &sampling) const {
    cv::Mat samples(sampling.size(), sampling.type(), cv::Scalar::all(0));

    for (std::set<point>::iterator k = points.begin(), n = points.end(); k != n; ++k) {
        const point &p = *k;
        samples.at<P>(p[0], p[1]) = sampling.at<P>(p[0], p[1]);
    }

    return samples;
}

template<class P> cv::Mat gaussian::sampler::minchinton2bgr(const cv::Mat &sampling) const {
    cv::Mat samples(sampling.size(), CV_8UC3, cv::Scalar::all(0));

    std::set<point>::iterator first = points.begin();
    std::set<point>::iterator last = --(points.end());
    for (std::set<point>::iterator k = first, n = last; k != n; ++k) {
        std::set<point>::iterator next = k;
        next++;

        const point &p1 = *k;
        const point &p2 = *next;
        if (sampling.at<P>(p1[0], p1[1]) > sampling.at<P>(p2[0], p2[1])) {
            samples.at<cv::Vec3b>(p1[0], p1[1]) = cv::Vec3b(0, 255, 0);
        }
        else {
            samples.at<cv::Vec3b>(p1[0], p1[1]) = cv::Vec3b(0, 0, 255);
        }
    }

    const point &p1 = *last;
    const point &p2 = *first;
    if (sampling.at<P>(p1[0], p1[1]) > sampling.at<P>(p2[0], p2[1])) {
        samples.at<cv::Vec3b>(p1[0], p1[1]) = cv::Vec3b(0, 255, 0);
    }
    else {
        samples.at<cv::Vec3b>(p1[0], p1[1]) = cv::Vec3b(0, 0, 255);
    }

    return samples;
}

#endif
