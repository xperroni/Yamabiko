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

#include <clarus/vision/bayer.hpp>

#include <clarus/vision/images.hpp>

inline int channel(int i, int j) {
    return (
        i % 2 != j % 2 ? 1 : // Green
        i % 2 == 0 ? 0 : // Blue
        2 // Red
    );
}

cv::Mat bayer::filter(const cv::Mat &image) {
    cv::Mat mosaic(image.size(), CV_8U, cv::Scalar(0));
    for (int i = 0; i < mosaic.rows; i++) {
        uchar *bayer = mosaic.ptr<uchar>(i);
        const cv::Vec3b *bgr = image.ptr<cv::Vec3b>(i);
        for (int j = 0; j < mosaic.cols; j++, bgr++, bayer++) {
            *bayer = (*bgr)[channel(i, j)];
        }
    }

    return mosaic;
}

cv::Mat bayer::filter(const std::string &path) {
    return filter(images::load(path));
}

cv::Mat bayer::bgr(const cv::Mat &mosaic) {
    cv::Mat image(mosaic.size(), CV_8UC3, cv::Scalar::all(0));
    for (int i = 0; i < mosaic.rows; i++) {
        const uchar *bayer = mosaic.ptr<uchar>(i);
        cv::Vec3b *bgr = image.ptr<cv::Vec3b>(i);
        for (int j = 0; j < mosaic.cols; j++, bgr++, bayer++) {
            (*bgr)[channel(i, j)] = *bayer;
        }
    }

    return image;
}
