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

#include <clarus/vision/dither.hpp>

#include <clarus/vision/filters.hpp>

cv::Mat dither::binary(const cv::Mat &image, double threshold) {
    CHANNEL_WISE(binary, image, threshold);

    cv::Mat dithered;
    cv::threshold(image, dithered, threshold, 255, cv::THRESH_BINARY);
    return dithered;
}

/*
Calculates a table of 256 assignments with as many distinct values as the given bit depth.

Values are taken at equal intervals from the ranges [0, 128) and [128, 256),
such that both 0 and 255 are always included in the range.
*/
static cv::Mat lookup(uchar bits) {
    static std::map<uchar, cv::Mat> tables;
    if (tables.count(bits) > 0) {
        return tables[bits];
    }

    int factor = 256 / pow(2, bits);
    cv::Mat table(1, 256, CV_8U);
    uchar *p = table.data;

    for(int i = 0; i < 128; ++i) {
        p[i] = factor * (i / factor);
    }

    for(int i = 128; i < 256; ++i) {
        p[i] = factor * (1 + (i / factor)) - 1;
    }

    tables[bits] = table;
    return table;
}

cv::Mat dither::threshold(const cv::Mat &image, uchar bits) {
    CHANNEL_WISE(threshold, image, bits);

    cv::Mat table = lookup(bits);
    cv::Mat dithered;
    cv::LUT(image, table, dithered);
    return dithered;
}

inline void update(uchar &pixel, double u) {
    pixel = (uchar) (u > 0 ? std::min(255.0, u + pixel) : std::max(0.0, u + pixel));
}

inline uchar& neighbor(uchar *cell, int cols, int i, int j) {
    return *(cell + i * cols + j);
}

cv::Mat dither::diffusion(const cv::Mat &image, uchar bits) {
    static const double quant_1_2 = 7.0 / 16.0;
    static const double quant_2_0 = 3.0 / 16.0;
    static const double quant_2_1 = 5.0 / 16.0;
    static const double quant_2_2 = 1.0 / 16.0;

    CHANNEL_WISE(diffusion, image, bits);

    int rows = image.rows;
    int cols = image.cols;

    cv::Mat dithered;
    cv::copyMakeBorder(image, dithered, 0, rows + 1, 0, cols + 1, cv::BORDER_CONSTANT, cv::Scalar(0));

    cv::Mat table = lookup(bits);

    for (int i = 0; i < rows; i++) {
        uchar *cell = dithered.ptr<uchar>(i);
        for (int j = 0; j < cols; j++, cell++) {
            double pix = *cell;
            double dit = table.at<uchar>(pix);
            double err = pix - dit;

            *cell = (uchar) dit;

            update(neighbor(cell, cols, i + 0, j + 1), err * quant_1_2);
            update(neighbor(cell, cols, i + 1, j - 1), err * quant_2_0);
            update(neighbor(cell, cols, i + 1, j + 0), err * quant_2_1);
            update(neighbor(cell, cols, i + 1, j + 1), err * quant_2_2);
        }
    }

    return dithered(cv::Rect(0, 0, cols, rows));
}
