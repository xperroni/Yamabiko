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

#include <clarus/vision/minchinton.hpp>

cv::Mat minchinton::bgr(const cv::Mat &data) {
    cv::Mat image(data.size(), CV_8UC3);
    for (int i = 0; i < data.rows; i++) {
        for (int j = 0; j < data.cols; j++) {
            uchar value = (data.at<bool>(i, j) ? 255 : 0);
            image.at<cv::Vec3b>(i, j) = cv::Vec3b(0, value, 255 - value);
        }
    }

    return image;
}
