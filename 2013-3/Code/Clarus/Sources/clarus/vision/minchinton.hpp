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

#ifndef CLARUS_VISION_MINCHINTON_HPP
#define CLARUS_VISION_MINCHINTON_HPP

#include <opencv2/opencv.hpp>

namespace minchinton {
    template<class T> cv::Mat filter2d(const cv::Mat &data);

    template<class T> cv::Mat filter2d(const cv::Mat &data, size_t w);

    cv::Mat bgr(const cv::Mat &data);
}

template<class T> cv::Mat minchinton::filter2d(const cv::Mat &data) {
    size_t rows = data.rows;
    size_t cols = data.cols;

    size_t i_n = rows - 1;
    size_t j_n = cols - 1;

    cv::Mat cells(data.size(), CV_8U);
    for (size_t i_1 = 0; i_1 < rows; i_1++) {
        for (size_t j_1 = 0; j_1 < cols; j_1++) {
            size_t i_2 = (i_1 < i_n ? i_1 + 1 : 0);
            size_t j_2 = (j_1 < j_n ? j_1 + 1 : 0);

            T t = data.at<T>(i_1, j_1);
            bool bigger = (
                t > data.at<T>(i_1, j_2) &&
                t > data.at<T>(i_2, j_1) &&
                t > data.at<T>(i_2, j_2)
            );

            cells.at<uchar>(i_1, j_1) = (bigger ? 255 : 0);
        }
    }

    return cells;
}

template<class T> cv::Mat minchinton::filter2d(const cv::Mat &data, size_t w) {
    cv::Mat cells(data.size(), CV_8U);

    size_t u = w / 2;
    for (size_t i = 0, m = data.rows; i < m; i++) {
        for (size_t j = 0, n = data.cols; j < n; j++) {
            size_t x = (j < u ? j : j - u);
            size_t y = (i < u ? i : i - u);
            size_t width = (x + w > n ? n - x : w);
            size_t height = (y + w > m ? m - y : w);

            cv::Rect roi(x, y, width, height);
            cv::Mat patch(data, roi);

            cv::Mat mean;
            cv::Mat stdev;
            cv::meanStdDev(patch, mean, stdev);

            T val = data.at<T>(i, j);
            T avg = mean.at<T>(0);
            T std = stdev.at<T>(0);

            cells.at<uchar>(i, j) = (fabs(val - avg) > std ? 255 : 0);
        }
    }

    return cells;
}

#endif
