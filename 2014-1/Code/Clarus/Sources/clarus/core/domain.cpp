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

#include <clarus/core/domain.hpp>

cv::Mat::MSize domain::size3D(size_t width, size_t height, size_t depth) {
    int *buffer = new int[3];
    buffer[0] = height;
    buffer[1] = width;
    buffer[2] = depth;
    return cv::Mat::MSize(buffer);
}

template<> void domain::from_string(bool &value, const std::string &str) {
    value = (str == "true");
}

template<> std::string domain::to_string(const bool &value) {
    return (value ? "true" : "false");
}
