#ifndef CLARUS_MODEL_MUNKRES_HPP
#define CLARUS_MODEL_MUNKRES_HPP

#include <clarus/core/list.hpp>

#include <opencv2/opencv.hpp>

#include <utility>

typedef std::pair<size_t, size_t> Assignment;

/*
Implements the Hungarian algorithm [1]. Code adapted from [2].

The input matrix is expected to be of type CV_64F and w <= h for data.size() == cv::Size(w, h).

Assignments are returned in the form (row index, column index). To invert the order
of the terms, set the optional rowfirst attribute to false.

[1] http://en.wikipedia.org/wiki/Hungarian_algorithm
[2] https://github.com/saebyn/munkres-cpp
*/
List<Assignment> munkres(const cv::Mat &data, bool rowfirst = true);

#endif // CLARUS_MODEL_MUNKRES_HPP
