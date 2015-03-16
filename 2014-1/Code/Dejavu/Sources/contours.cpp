#include "contours.hpp"

Contours::Contours() {
    // Nothing to do.
}

#include <clarus/vision/images.hpp>

Contours::Contours(const cv::Mat &entropy, double a) {
    static cv::Mat element5 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
    static cv::Mat element10 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(10, 10));

    cv::Mat interest;
    cv::dilate(entropy, interest, element5);
    cv::erode(interest, interest, element10);

    // Calculates contour vectors for the boundaries extracted above.
    Contours &contours = *this;
    cv::findContours(interest, *contours, *(contours.hierarchy), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

    List<int> &selected = contours.selected;
    for (int i = 0, n = contours.size(); i < n; i++) {
        const Contour &contour = contours[i];
        double r = cv::contourArea(contour);
        if (a <= r) {
            selected.append(i);
        }
    }
}
