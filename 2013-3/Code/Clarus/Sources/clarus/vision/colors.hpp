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

#ifndef CLARUS_VISION_COLORS_HPP
#define CLARUS_VISION_COLORS_HPP

#include <clarus/core/list.hpp>

#include <opencv2/opencv.hpp>

namespace colors {
    /*
    Decomposes a BGR image across the channels of a panchromatic filter.

    Reference: http://en.wikipedia.org/wiki/Bayer_filter#.22Panchromatic.22_cells
    */
    List<cv::Mat> BGRL(const cv::Mat &bgr);

    /*
    Performs the given conversion to the color space, then splits the result across channels.
    */
    List<cv::Mat> channels(const cv::Mat &image, int conversion);

    /*
    Splits the given input image across channels.
    */
    List<cv::Mat> channels(const cv::Mat &image);

    /*
    Convert the input data to unsigned single-byte type, possibly normalizing the
    input to the range [0, 255] before.

    If the input matrix is multi-channel, each channel is processed separately.
    */
    cv::Mat discrete(const cv::Mat &data, bool normalize = true);

    /*
    Converts the given BGR image to the HSL color space.
    */
    List<cv::Mat> HLS(const cv::Mat &bgr);

    /*
    Converts the given BGR image to the HSL color space, hen returns its Hue channel.
    */
    cv::Mat hue(const cv::Mat &bgr);

    /*
    Converts the given BGR image to the HSL color space, hen returns its Lightness channel.
    */
    cv::Mat lightness(const cv::Mat &bgr);

    /*
    Converts the given BGR image to the HSL color space, hen returns its Saturation channel.
    */
    cv::Mat saturation(const cv::Mat &bgr);
}

#endif
