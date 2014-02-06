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

#ifndef CLARUS_VISION_DITHER_HPP
#define CLARUS_VISION_DITHER_HPP

#include <opencv2/opencv.hpp>

namespace dither {
    /*
    Performs binary dithering in the input image, using the given threshold.

    Arguments:

    image
        Input image. If multi-channel, dithering is performed channel-wise.

    threshold
        Pixel values bigger than the threshold are converted to 255, the others to 0.

    Returns:

    Pixel matrix of type CV_8UC* with the values dithered down to the binary set {0, 255}.
    */
    cv::Mat binary(const cv::Mat &image, double threshold);

    /*
    Dithers the image to the given bit depth using the Floyd-Steinberg error diffusion
    algorithm.

    Arguments:

    image
        Input image. If multi-channel, dithering is performed channel-wise.

    bits
        "Virtual" bit depth of the output image (or its channels if multi-channel).
        This bit depth is "virtual" in the sense that the output *type* is actually
        unsigned char (and hence 8 bits long), but the number of distinct *values*
        returned is such that they could be indexed using this many bits.

    Returns:

    Pixel matrix of type CV_8UC* with the values dithered down to the given bit depth.
    */
    cv::Mat diffusion(const cv::Mat &image, uchar bits);


    /*
    Dithers an imeg to the given bit depth using a set of equally-spaced thresholds.

    Arguments:

    image
        Input image. If multi-channel, dithering is performed channel-wise.

    bits
        "Virtual" bit depth of the output image (or its channels if multi-channel).
        This bit depth is "virtual" in the sense that the output *type* is actually
        unsigned char (and hence 8 bits long), but the number of distinct *values*
        returned is such that they could be indexed using this many bits.

    Returns:

    Pixel matrix of type CV_8UC* with the values dithered down to the given bit depth.
    */
    cv::Mat threshold(const cv::Mat &image, uchar bits);
}

#endif
