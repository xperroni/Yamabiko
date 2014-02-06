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

#include <clarus/vision/filters.hpp>

cv::Mat sobel::filter(const cv::Mat &l) {
    cv::Mat g, x, y, e;

    // Gradient X
    cv::Sobel(l, g, CV_16S, 1, 0, 3, 1, 0, cv::BORDER_DEFAULT);
    cv::convertScaleAbs(g, x);

    // Gradient Y
    cv::Sobel(l, g, CV_16S, 0, 1, 3, 1, 0, cv::BORDER_DEFAULT);
    cv::convertScaleAbs(g, y);

    // Total Gradient (approximate)
    cv::addWeighted(x, 0.5, y, 0.5, 0, g);
    cv::convertScaleAbs(g, e);

    return e;
}

static int gauss(double sigma, int c, int n) {
    static cv::RNG rng;

    int i = rng.gaussian(sigma) + c;
    if (i < 0) {
        return 0;
    }
    else if (i >= n) {
        return n - 1;
    }

    return i;
}

gaussian::sampler::sampler() {
    // Nothing to do.
}

gaussian::sampler::sampler(size_t n, double sigma, const cv::Size &size) {
    int in = size.height;
    int jn = size.width;
    int ic = in / 2;
    int jc = jn / 2;

    while (points.size() < n) {
        point p(
            gauss(sigma, ic, in),
            gauss(sigma, jc, jn)
        );

        points.insert(p);
    }
}
