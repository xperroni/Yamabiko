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

#ifndef CLARUS_VISION_SAMPLER_HPP
#define CLARUS_VISION_SAMPLER_HPP

#include <clarus/core/index.hpp>

#include <boost/function.hpp>
#include <opencv2/opencv.hpp>

#include <set>

namespace clarus {
    typedef Index<2> Point;

    typedef boost::function<Point(void)> Distribution;

    class sampler;
}

class clarus::sampler {
    std::set<Point> points;

public:
    class gaussian {
        double sigma;

        cv::Size size;

        int sample(int c, int n) const;

    public:
        gaussian(double sigma, const cv::Size &size);

        Point operator () () const;
    };

    sampler();

    sampler(size_t n, Distribution distribution);

    template<class P> cv::Mat sample(const cv::Mat &sampling) const;

    template<class P> cv::Mat sample2d(const cv::Mat &sampling) const;

    template<class P> cv::Mat minchinton2bgr(const cv::Mat &sampling) const;
};

template<class P> cv::Mat clarus::sampler::sample(const cv::Mat &sampling) const {
    cv::Mat samples(points.size(), 1, sampling.type(), cv::Scalar::all(0));

    int l = 0;
    for (std::set<Point>::iterator k = points.begin(), n = points.end(); k != n; ++k) {
        const Point &p = *k;
        samples.at<P>(l++) = sampling.at<P>(p[0], p[1]);
    }

    return samples;
}

template<class P> cv::Mat clarus::sampler::sample2d(const cv::Mat &sampling) const {
    cv::Mat samples(sampling.size(), sampling.type(), cv::Scalar::all(0));

    for (std::set<Point>::iterator k = points.begin(), n = points.end(); k != n; ++k) {
        const Point &p = *k;
        samples.at<P>(p[0], p[1]) = sampling.at<P>(p[0], p[1]);
    }

    return samples;
}

template<class P> cv::Mat clarus::sampler::minchinton2bgr(const cv::Mat &sampling) const {
    cv::Mat samples(sampling.size(), CV_8UC3, cv::Scalar::all(0));

    std::set<Point>::iterator first = points.begin();
    std::set<Point>::iterator last = --(points.end());
    for (std::set<Point>::iterator k = first, n = last; k != n; ++k) {
        std::set<Point>::iterator next = k;
        next++;

        const Point &p1 = *k;
        const Point &p2 = *next;
        if (sampling.at<P>(p1[0], p1[1]) > sampling.at<P>(p2[0], p2[1])) {
            samples.at<cv::Vec3b>(p1[0], p1[1]) = cv::Vec3b(0, 255, 0);
        }
        else {
            samples.at<cv::Vec3b>(p1[0], p1[1]) = cv::Vec3b(0, 0, 255);
        }
    }

    const Point &p1 = *last;
    const Point &p2 = *first;
    if (sampling.at<P>(p1[0], p1[1]) > sampling.at<P>(p2[0], p2[1])) {
        samples.at<cv::Vec3b>(p1[0], p1[1]) = cv::Vec3b(0, 255, 0);
    }
    else {
        samples.at<cv::Vec3b>(p1[0], p1[1]) = cv::Vec3b(0, 0, 255);
    }

    return samples;
}

#endif
