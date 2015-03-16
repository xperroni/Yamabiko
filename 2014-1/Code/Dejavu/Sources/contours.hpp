#ifndef DEJAVU_CONTOURS_HPP
#define DEJAVU_CONTOURS_HPP

#include <clarus/core/list.hpp>

#include <opencv2/opencv.hpp>

#include <vector>

typedef std::vector<cv::Point> Contour;

struct Contours: public clarus::List<Contour> {
    List<cv::Vec4i> hierarchy;

    List<int> selected;

    Contours();

    Contours(const cv::Mat &entropy, double a);
};

#endif // DEJAVU_CONTOURS_HPP
