#include <cight/transforms.hpp>

#include <clarus/vision/colors.hpp>
#include <clarus/vision/filters.hpp>

cv::Mat cight::binary_edges(const cv::Mat &bgr) {
    return filter::otsu(filter::sobel(colors::grayscale(bgr)));
}

cv::Mat cight::column_histogram(const cv::Mat &image, size_t bins) {
    cv::Mat data;
    if (image.channels() == 1) {
        data = image;
    }
    else {
        int rows = image.rows;
        data = image.reshape(1, image.channels());
        cv::reduce(data, data, 0, CV_REDUCE_SUM);
        data = data.reshape(1, rows);
    }

    size_t cols = data.cols / bins;
    size_t rows = data.rows;
    cv::Mat totals(1, bins, CV_32F, cv::Scalar::all(0));
    for (size_t i = 0; i < bins; i++) {
        cv::Rect bounds(cols * i, 0, cols, rows);
        cv::Mat section(data, bounds);
        totals.at<float>(0, i) = cv::norm(cv::sum(section), cv::NORM_L1);
    }

    return totals;
}

cv::Mat cight::upper_half(const cv::Mat &image) {
    cv::Rect roi(0, 0, image.cols, image.rows / 2);
    cv::Mat half(image, roi);
    return half;
}
