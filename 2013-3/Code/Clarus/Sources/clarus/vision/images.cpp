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

#include <clarus/vision/colors.hpp>

#include <iostream>

cv::Mat images::load(const std::string &path) {
    cv::Mat image = cv::imread(path);
    if (image.empty()) {
        throw std::ios_base::failure("Could not load \"" + path + "\": file not found");
    }

    return image;
}

cv::Mat images::load(const std::string &folder, const std::string &name, const std::string &ext) {
    return load(folder + '/' + name + '.' + ext);
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
