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

#include <clarus/vision/logpolar.hpp>

#include <clarus/vision/cvmat.hpp>

cv::Mat logpolar::transform(const cv::Mat &image, int i_0, int j_0, int p_n, int t_n, bool slide) {
    // Original image dimensions.
    int i_n = image.rows;
    int j_n = image.cols;

    if (i_0 == -1) {
        // The default vertical center is on the middle of the image.
        i_0 = i_n / 2;
    }

    if (j_0 == -1) {
        // The default horizontal center is on the middle of the image.
        j_0 = j_n / 2;
    }

    // The distance d_c from the transform's focus (i_0, j_0) to the image's
    // farthest corner (i_c, j_c). This is used below as the default value for
    // p_n, and also to calculate the iteration step across the transform's p
    // dimension.
    int i_c = std::max(i_0, i_n - i_0);
    int j_c = std::max(j_0, j_n - j_0);
    double d_c = sqrt(pow(i_c, 2) + pow(j_c, 2));

    if (p_n == -1) {
        // The default value to p_n is defined as the distance d_c.
        p_n = (int) ceil(d_c);
    }

    if (t_n == -1) {
        // The default value to t_n is defined as the width of the image.
        t_n = j_n;
    }

    // The scale factors determine the size of each "step" along the transform.
    double p_s = log(d_c) / p_n;
    double t_s = 2.0 * M_PI / t_n;

    // The transform's pixels have the same type and depth as the input's.
    cv::Mat lp(cv::Size(t_n, p_n * 2), image.type(), cv::Scalar::all(0));

    int p_l = 0;
    for (int p = 0; p < p_n; p++) {
        double p_exp = exp(p * p_s);
        for (int t = 0; t < t_n; t++) {
            double t_rad = t * t_s;

            int i = i_0 + p_exp * sin(t_rad);
            int j = j_0 + p_exp * cos(t_rad);

            if (
                0 <= i && i < i_n &&
                0 <= j && j < j_n &&
                !(slide && cvmat::zero(image, i, j))
            ) {
                p_l = p_n - p - 1;
                cvmat::copy(lp, p_l, t, image, i, j);
            }
        }
    }

    return lp(cv::Rect(0, p_l, t_n, p_n));
}
