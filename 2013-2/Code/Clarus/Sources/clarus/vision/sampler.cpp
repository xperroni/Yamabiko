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

#include "sampler.hpp"

clarus::sampler::gaussian::gaussian(double _sigma, const cv::Size &_size):
    sigma(_sigma),
    size(_size)
{
    // Nothing to do.
}

int clarus::sampler::gaussian::sample(int c, int n) const {
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

clarus::Point clarus::sampler::gaussian::operator () () const {
    int in = size.height;
    int ic = in / 2;

    int jn = size.width;
    int jc = jn / 2;

    return Point(
        sample(ic, in),
        sample(jc, jn)
    );
}

clarus::sampler::sampler() {
    // Nothing to do.
}

clarus::sampler::sampler(size_t n, Distribution distribution) {
    while (points.size() < n) {
        Point p = distribution();
        points.insert(p);
    }
}
