#ifndef CLARUS_VISION_SPARSE_HPP
#define CLARUS_VISION_SPARSE_HPP

#include <clarus/core/list.hpp>
#include <clarus/model/point.hpp>

#include <opencv2/opencv.hpp>

namespace sparse {
    clarus::List<clarus::Point> search(const cv::Mat &data, int spread = 32, int upto = INT_MAX);

    void mask(cv::Mat &mask, const clarus::Point &point, int spread = 32);

    cv::Mat mask(const cv::Size &size, const clarus::Point &point, int spread = 32);

    cv::Mat mask(const cv::Size &size, const clarus::List<clarus::Point> &points, int spread = 32);
}

#endif
