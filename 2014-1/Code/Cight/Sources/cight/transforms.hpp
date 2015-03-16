#ifndef DEJAVU_TRANSFORMS_HPP
#define DEJAVU_TRANSFORMS_HPP

#include <opencv2/opencv.hpp>

namespace cight {
    /*
    Returns a binary edge map of the input BGR image.
    */
    cv::Mat binary_edges(const cv::Mat &bgr);

    /*
    Returns the vector of pixel sums over (bins) columns of equal width for the given
    image. Multichannel images are supported.
    */
    cv::Mat column_histogram(const cv::Mat &image, size_t bins);

    /*
    Returns the upper half of the input image.
    */
    cv::Mat upper_half(const cv::Mat &image);
}

#endif
