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

#include <clarus/core/index.hpp>
#include <clarus/core/list.hpp>

#include <boost/function.hpp>
#include <opencv2/opencv.hpp>

#include <set>

struct point: public Index<2> {
    class cluster: public List<point> {
        typedef ListIterator<point> PointIterator;

        typedef ListIteratorConst<point> PointIteratorConst;

        int range;

        int i_min;

        int i_max;

        int j_min;

        int j_max;

        bool add(const point &p);

    public:
        cluster(int range, int i0, int j0);

        bool operator < (const cluster &other) const;

        bool add(int i, int j);

        void mask_points(cv::Mat &data, int label) const;

        void mask_region(cv::Mat &data) const;

        void draw(cv::Mat &image) const;

        cv::Mat extract(const cv::Mat &src) const;

        bool inside(int i, int j) const;

        bool inside(const cluster &other) const;

        bool merge(const cluster &other);
    };

    class cloud: public List<cluster> {
        typedef ListIterator<cluster> ClusterIterator;

        typedef ListIteratorConst<cluster> ClusterIteratorConst;

        int range;

        void merge(ClusterIterator &added);

    public:
        cloud(int range);

        void add(int i, int j);

        cv::Mat mask(const cv::Size &size, size_t threshold = 0) const;

        void draw(cv::Mat &image, size_t size = 0) const;
    };

    typedef boost::function<std::set<point>(size_t)> distribution;

    class gaussian {
        double sigma;

        cv::Size size;

        int sample(int c, int n) const;

    public:
        gaussian(double sigma, const cv::Size &size);

        point operator () () const;

        std::set<point> operator () (size_t n) const;
    };

    class sampler {
        std::set<point> points;

    public:
        sampler();

        sampler(size_t n, distribution p);

        template<class P> cv::Mat sample(const cv::Mat &sampling) const;

        template<class P> cv::Mat sample2d(const cv::Mat &sampling) const;

        template<class P> cv::Mat minchinton2bgr(const cv::Mat &sampling) const;
    };

    struct range;

    point(size_t i, size_t j);

    point(const cv::Rect &rect);

    operator cv::Point () const;
};

struct point::range {
    point p0;

    point pc;

    point pn;

    range();

    operator cv::Rect () const;

    range(size_t i0, size_t j0, size_t in, size_t jn);

    bool contains(const point &p) const;

    bool intersects(const range &that) const;
};

template<class P> cv::Mat point::sampler::sample(const cv::Mat &sampling) const {
    cv::Mat samples(points.size(), 1, sampling.type(), cv::Scalar::all(0));

    int l = 0;
    for (std::set<point>::iterator k = points.begin(), n = points.end(); k != n; ++k) {
        const point &p = *k;
        samples.at<P>(l++) = sampling.at<P>(p[0], p[1]);
    }

    return samples;
}

template<class P> cv::Mat point::sampler::sample2d(const cv::Mat &sampling) const {
    cv::Mat samples(sampling.size(), sampling.type(), cv::Scalar::all(0));

    for (std::set<point>::iterator k = points.begin(), n = points.end(); k != n; ++k) {
        const point &p = *k;
        samples.at<P>(p[0], p[1]) = sampling.at<P>(p[0], p[1]);
    }

    return samples;
}

template<class P> cv::Mat point::sampler::minchinton2bgr(const cv::Mat &sampling) const {
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
