#ifndef CLARUS_VISION_FEATURES_HPP
#define CLARUS_VISION_FEATURES_HPP

#include <clarus/core/list.hpp>

#include <opencv2/opencv.hpp>

namespace features {
    /*
    Implements the 8-bit Local Binary Pattern (LBP) feature descriptor as described
    in Tan, Triggs (2007), "Enhanced Local Texture Feature Sets for Face Recognition
    Under Difficult Lighting Conditions".
    */
    cv::Mat LBP(const cv::Mat &image);

    cv::Mat FREAK(const cv::Mat &image, float size, int step, int bytes = 64);

    clarus::List<cv::Mat> splitFREAK(const cv::Mat &image, float size, int step);

    cv::Mat minchinton(const cv::Mat &image, int side);
}

#endif
