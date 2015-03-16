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

#ifndef CLARUS_VISION_IMAGES_HPP
#define CLARUS_VISION_IMAGES_HPP

#include <opencv2/opencv.hpp>

#include <string>

namespace images {
    cv::Mat absdiff(const cv::Mat &a, const cv::Mat &b);

    cv::Mat convert(const cv::Mat &image, int type);

    cv::Mat convert3Dto2D(const cv::Mat &image);

    cv::Mat scale(const cv::Mat &image, const cv::Size &size);

    cv::Mat load(const std::string &path, int flags = 1);

    cv::Mat load(
        const std::string &folder,
        const std::string &name,
        const std::string &ext,
        int flags = 1
    );

    void save(const cv::Mat &data, const std::string &path, bool normalize = true);

    void save(
        const cv::Mat &data,
        const std::string &folder,
        const std::string &name,
        const std::string &ext,
        bool normalize = true
    );
};

#endif
