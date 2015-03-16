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

#include <clarus/vision/images.hpp>

#include <clarus/vision/cvmat.hpp>
#include <clarus/vision/colors.hpp>

#include <cstring>
#include <iostream>

cv::Mat images::absdiff(const cv::Mat &a, const cv::Mat &b) {
    cv::Mat c;
    cv::absdiff(a, b, c);
    return c;
}

cv::Mat images::convert(const cv::Mat &image, int type) {
    cv::Mat converted;
    image.convertTo(converted, type);
    return converted;
}

cv::Mat images::convert3Dto2D(const cv::Mat &image) {
    int t = image.type();
    cv::Mat i2d(image.size(), t);
    memcpy(i2d.data, image.data, image.total() * cvmat::type::length(t));
    return i2d;
}

cv::Mat images::scale(const cv::Mat &image, const cv::Size &size) {
    cv::Mat scaled;
    cv::resize(image, scaled, size);
    return scaled;
}

cv::Mat images::load(const std::string &path, int flags) {
    cv::Mat image = cv::imread(path, flags);
    if (image.empty()) {
        throw std::ios_base::failure("Could not load \"" + path + "\": file not found");
    }

    return image;
}

cv::Mat images::load(
    const std::string &folder,
    const std::string &name,
    const std::string &ext,
    int flags
) {
    return load(folder + '/' + name + '.' + ext, flags);
}

void images::save(const cv::Mat &data, const std::string &path, bool normalize) {
    cv::Mat pixels = (normalize ? colors::discrete(data) : data);

    cv::imwrite(path, pixels);
}

void images::save(
    const cv::Mat &data,
    const std::string &folder,
    const std::string &name,
    const std::string &ext,
    bool normalize
) {
    save(data, folder + '/' + name + '.' + ext, normalize);
}
