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

#ifndef CLARUS_MODEL_MUNKRES_HPP
#define CLARUS_MODEL_MUNKRES_HPP

#include <clarus/core/list.hpp>
#include <clarus/model/point.hpp>

#include <opencv2/opencv.hpp>

#include <utility>

namespace clarus {
    /*
    Implements the Hungarian algorithm [1]. Code adapted from [2].

    The input matrix is expected to be of type CV_64F and w <= h for data.size() == cv::Size(w, h).

    Assignments are returned in the form (row index, column index). To invert the order
    of the terms, set the optional rowfirst attribute to false.

    [1] http://en.wikipedia.org/wiki/Hungarian_algorithm
    [2] https://github.com/saebyn/munkres-cpp
    */
    List<Point> munkres(const cv::Mat &data, bool rowfirst = true);
}

#endif
