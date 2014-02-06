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

#ifndef CLARUS_VISION_KERNEL_HPP
#define CLARUS_VISION_KERNEL_HPP

#include <clarus/vision/fourier.hpp>

#include <cstdarg>

template<size_t s> struct Kernel: cv::Mat {
    Kernel(float k0, ...);

    cv::Mat operator () (const cv::Mat &data) const;
};

template<size_t s> Kernel<s>::Kernel(float k0, ...):
    cv::Mat(s, s, CV_32F)
{
    va_list args;
    va_start(args, k0);

    at<float>(0, 0) = k0;
    for (size_t i = 1, n = s * s; i < n; i++) {
        at<float>(i / s, i % s) = va_arg(args, double);
    }
}

template<size_t s> cv::Mat Kernel<s>::operator () (const cv::Mat &data) const {
    cv::Mat input;
    data.convertTo(input, CV_32F);

    cv::Mat output;
    cv::filter2D(input, output, -1, *((cv::Mat*) this));

    return output;
}

#endif
