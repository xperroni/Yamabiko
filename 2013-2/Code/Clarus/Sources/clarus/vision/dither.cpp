/*
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
along with Clarus.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <clarus/vision/dither.hpp>

const double floyd_1_2 = 7.0 / 16.0;
const double floyd_2_0 = 3.0 / 16.0;
const double floyd_2_1 = 5.0 / 16.0;
const double floyd_2_2 = 1.0 / 16.0;

inline void update(uchar &pixel, double u) {
    double a = (double) pixel;
    if (u > 0) {
        pixel = (uchar) std::min(255.0, a + u);
    }
    else {
        pixel = (uchar) std::max(0.0, a + u);
    }
}

inline double step(uchar value, uchar scale, uchar threshold) {
    uchar pixel = (value / scale) * scale;
    return (pixel < threshold ? pixel : 255);
}

cv::Mat dither::transform(const cv::Mat &image, uchar bits) {
    int rows = image.rows;
    int cols = image.cols;

    cv::Mat dithered;
    cv::copyMakeBorder(image, dithered, 0, rows + 1, 0, cols + 1, cv::BORDER_CONSTANT, cv::Scalar::all(0));

    uchar depth = pow(2, bits);
    uchar scale = 255 / depth;
    uchar threshold = 254 - scale; // I do mean 254, it's not a typo

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            double pix = dithered.at<uchar>(i, j);
            double sub = step(pix, scale, threshold);
            dithered.at<uchar>(i, j) = (uchar) sub;
            double e = pix - sub;
            update(dithered.at<uchar>(i + 0, j + 1), e * floyd_1_2);
            update(dithered.at<uchar>(i + 1, j - 1), e * floyd_2_0);
            update(dithered.at<uchar>(i + 1, j + 0), e * floyd_2_1);
            update(dithered.at<uchar>(i + 1, j + 1), e * floyd_2_2);
        }
    }

    return dithered(cv::Rect(0, 0, cols, rows));
}
