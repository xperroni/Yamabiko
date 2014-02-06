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

#ifndef CLARUS_VISION_SNAPSHOT_HPP
#define CLARUS_VISION_SNAPSHOT_HPP

#include <opencv2/opencv.hpp>

namespace fourier {
    cv::Size fit(const cv::Size &size);

    cv::Mat convolve(const cv::Mat &data, const cv::Mat &kernel);

    cv::Mat transform(const cv::Mat &data, const cv::Size &optimal = cv::Size(0, 0));

    cv::Mat inverse(const cv::Mat &fourier, const cv::Size &optimal = cv::Size(0, 0));

    cv::Mat magnitude(const cv::Mat &fourier);

    cv::Mat phase(const cv::Mat &fourier);

    cv::Mat bgr(const cv::Mat &fourier);
}

#endif
