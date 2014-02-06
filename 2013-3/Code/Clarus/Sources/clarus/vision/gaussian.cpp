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

#include <clarus/vision/gaussian.hpp>

#include <clarus/vision/filters.hpp>

cv::Mat gaussian::blur(
    const cv::Mat &image,
    const cv::Size &size,
    double sigmaX,
    double sigmaY,
    int borderType
) {
    cv::Mat blurred;
    cv::GaussianBlur(image, blurred, size, sigmaX, sigmaY, borderType);
    return blurred;
}

cv::Mat gaussian::blur(const cv::Mat &image, double w, int borderType) {
    int size = 3 * w;
    if (size % 2 == 0) {
        size += 1;
    }

    return blur(image, cv::Size(size, size), w, w, borderType);
}

cv::Mat gaussian::difference(const cv::Mat &image, double w0, double w1) {
    CHANNEL_WISE(difference, image, w0, w1);

    cv::Mat blurred0 = blur(image, w0);
    cv::Mat blurred1 = blur(image, w1);
    return blurred0 - blurred1;
}
