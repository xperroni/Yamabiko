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

#ifndef CLARUS_VISION_FILTERS_HPP
#define CLARUS_VISION_FILTERS_HPP

#include <clarus/core/list.hpp>
#include <clarus/vision/kernel.hpp>

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <opencv2/opencv.hpp>

#include <set>
#include <string>

/*
Convenience macro for applying the channel-wise meta-filter.
*/
#define CHANNEL_WISE(F, SRC, ...) \
    if (SRC.channels() > 1) return filter::channelwise(boost::bind(F, _1, ##__VA_ARGS__), SRC);

namespace filter {
    /*
    Filter function prototype.
    */
    typedef boost::function<cv::Mat(const cv::Mat&)> Filter;

    /*
    Applies a filter to  multi-channel image channel-wise.

    Given a multi-channel image and a filter, this function performs the following steps:

    1. Split image across channels;
    2. Apply filter to each separate channel;
    3. Assemble result image from channel outputs.

    Use boost::bind() to pass filter functions with extra parameters.
    */
    cv::Mat channelwise(Filter f, const cv::Mat &image);

    cv::Mat convolve(const cv::Mat &data, const cv::Mat &kernel);

    cv::Mat energy(const cv::Mat &data, size_t w);

    cv::Mat gamma(const cv::Mat &src, double g);

    cv::Mat gradients(const cv::Mat &l);

    clarus::List<cv::Mat> laws(const cv::Mat &data, size_t w);

    cv::Mat normalize(const cv::Mat &data, size_t w);

    cv::Mat otsu(cv::Mat image);

    clarus::List<cv::Mat> prewitt(const cv::Mat &l);

    cv::Mat sobel(const cv::Mat &l);

    cv::Mat suppress(const cv::Mat &edges);

    cv::Mat tantriggs(
        const cv::Mat &image,
        double a = 0.1,
        double t = 10.0,
        double g = 0.2,
        double w0 = 1.0,
        double w1 = 2.0
    );

    template<class T> T consensus(const cv::Mat &patch, const T &fallback);

    template<class T> cv::Mat trends(const cv::Mat &data, size_t w);

    template<class T> cv::Mat trends(const cv::Mat &data, size_t w, const T &fallback);
}

template<class T> T filter::consensus(const cv::Mat &patch, const T &fallback) {
    std::map<T, int> votes;
    int rows = patch.rows;
    int cols = patch.cols;
    double n = rows * cols;
    double lead = 0;
    const T *winner = NULL;

    for (size_t i = 0; i < rows; i++) {
        for (const T *j = patch.ptr<T>(i), *n = j + cols; j < n; j++) {
            int &c = votes[*j];
            c++;

            if (c > lead) {
                lead = c;
                winner = j;
            }
        }
    }

    if ((lead / n) > 0.5) {
        return *winner;
    }

    return fallback;
}

template<class T> cv::Mat filter::trends(const cv::Mat &data, size_t w) {
    cv::Mat c = cv::Mat_<T>(data.size());

    size_t m = data.rows;
    size_t n = data.cols;
    size_t u = w / 2;
    for (size_t i = 0; i < m; i++) {
        for (size_t j = 0; j < n; j++) {
            size_t x = (j < u ? j : j - u);
            size_t y = (i < u ? i : i - u);
            size_t width = (x + w > n ? n - x : w);
            size_t height = (y + w > m ? m - y : w);

            cv::Rect roi(x, y, width, height);
            cv::Mat patch(data, roi);

            const T &t = data.at<T>(i, j);
            c.at<T>(i, j) = consensus(patch, t);
        }
    }

    return c;
}

template<class T> cv::Mat filter::trends(const cv::Mat &data, size_t w, const T &fallback) {
    cv::Mat trend = cv::Mat_<T>(data.size());

    size_t m = data.rows;
    size_t n = data.cols;
    size_t u = w / 2;
    for (size_t i = 0; i < m; i++) {
        T *cell = trend.ptr<T>(i);
        for (size_t j = 0; j < n; j++, cell++) {
            size_t x = (j < u ? j : j - u);
            size_t y = (i < u ? i : i - u);
            size_t width = (x + w > n ? n - x : w);
            size_t height = (y + w > m ? m - y : w);

            cv::Rect roi(x, y, width, height);
            cv::Mat patch(data, roi);

            *cell = consensus(patch, fallback);
        }
    }

    return trend;
}

#endif
