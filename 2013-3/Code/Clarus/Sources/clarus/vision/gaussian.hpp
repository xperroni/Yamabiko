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

#ifndef CLARUS_VISION_GAUSSIAN_HPP
#define CLARUS_VISION_GAUSSIAN_HPP

#include <opencv2/opencv.hpp>

namespace gaussian {
    /*
    Performs Gaussian blur on the input image. This is essentialy the direct application
    of OpenCV's GaussianBlur() function, with the only differences being the default
    Border Type (BORDER_CONSTANT instead of BORDER_DEFAULT) and the use of a return
    value instead of output parameter.

    Multi-channel images are processed channel-wise.
    */
    cv::Mat blur(
        const cv::Mat &image,
        const cv::Size &size,
        double sigmaX,
        double sigmaY = 0,
        int borderType = cv::BORDER_CONSTANT
    );

    /*
    Performs a Gaussian blur on the given image, subject to the following directives:

    1. The standard deviation across both horizontal and vertical drections is set to w;
    2. The size of the kernel across both dimensions is set to 3 * w + (w % 2 == 0 ? 1 : 0).

    Multi-channel images are processed channel-wise.
    */
    cv::Mat blur(const cv::Mat &image, double w, int borderType = cv::BORDER_CONSTANT);

    /*
    Performs a Difference of Gaussians on the input image, using the second form of
    the blur() function (with the w argument instead of separate size and standard
    deviations).

    Multi-channel images are processed channel-wise.
    */
    cv::Mat difference(const cv::Mat &image, double w0, double w1);
}

#endif
