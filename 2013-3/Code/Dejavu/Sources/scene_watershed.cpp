/*
Copyright (c) Helio Perroni Filho <xperroni@gmail.com>

This file is part of Dejavu.

Dejavu is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Dejavu is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Dejavu. If not, see <http://www.gnu.org/licenses/>.
*/

#include "scene_watershed.hpp"

static cv::Mat boundaries(const cv::Mat &bgr, const cv::Mat &markers) {
    int rows = bgr.rows;
    int cols = bgr.cols;

    cv::watershed(bgr, markers);

    // Copy the boundaries of the objetcs segmented by cv::watershed().
    // Ensure there is a minimum distance of 1 pixel between boundary
    // pixels and the image border.
    cv::Mat borders(rows + 2, cols + 2, CV_8U, cv::Scalar::all(0));
    for (int i = 0; i < rows; i++) {
        uchar *u = borders.ptr<uchar>(i + 1) + 1;
        const int *v = markers.ptr<int>(i);
        for (int j = 0; j < cols; j++, u++, v++) {
            *u = (*v == -1);
        }
    }

    return borders;
}

typedef std::vector<cv::Point> Contour;

#define COMES_BEFORE(A, B, OP) do { \
    int U = A.OP; \
    int V = B.OP; \
    if (U > V) { \
        return true; \
    } \
    else if (U < V) { \
        return false; \
    } \
} while (false)

typedef bool (*Areas)(const cv::Rect &a, const cv::Rect &b);

static bool areas(const cv::Rect &a, const cv::Rect &b) {
    const cv::Size &u = a.size();
    const cv::Size &v = b.size();

    COMES_BEFORE(u, v, area());
    COMES_BEFORE(u, v, width);
    COMES_BEFORE(u, v, height);

    return false;
}

typedef std::map<cv::Rect, int, Areas> Ranking;

static void extract(
    const cv::Mat &bgr,
    const cv::Mat &borders,
    int lowest,
    List<cv::Rect> &rois,
    List<cv::Mat> &views
) {
    static cv::Scalar WHITE = cv::Scalar::all(255);

    // Calculate contour vectors for the boundaries extracted above.
    std::vector<Contour> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(borders, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

    const cv::Size &size = borders.size();
    int ceil = bgr.size().area();
    int rows = bgr.rows;
    int cols = bgr.cols;

    std::set<cv::Rect, Areas> unique(areas);
    for (int i = 0, n = contours.size(); i < n; i++) {
        // Ignores repeated contours.
        const Contour &contour = contours[i];
        cv::Rect rect = cv::boundingRect(contour);
        if (unique.count(rect) > 0) {
            continue;
        }

        // Ignores contours for which the bounding rectangle's
        // area equals the area of the original image, or is
        // lower than the assigned minimum.
        int area = rect.area();
        if (area == ceil || area < lowest) {
            continue;
        }

        // Records the bounds of the segmented object.
        unique.insert(rect);
        cv::Rect roi = rect - cv::Point(1, 1);
        rois.append(roi);

        // Draws the selected contour.
        cv::Mat region(size, CV_8U, cv::Scalar::all(0));
        cv::drawContours(region, contours, i, WHITE, CV_FILLED, 8, hierarchy, INT_MAX);

        // Removes the 1 pixel-thick border added when the boundaries
        // were first copied from the output of cv::watershed().
        cv::Mat mask(region, cv::Rect(1, 1, cols, rows));

        // Records the segmented object.
        cv::Mat object;
        bgr.copyTo(object, mask);
        views.append(object(roi));
    }
}

static cv::Mat saturation_mask(const cv::Mat &bgr) {
    cv::Mat grays = colors::saturation(bgr);
    cv::normalize(grays, grays, 0, 255, CV_MINMAX);

    cv::Mat binary;
    cv::threshold(grays, binary, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);

    cv::Mat edges = filter::sobel(grays);
    cv::threshold(edges, edges, 0, 255, cv::THRESH_BINARY_INV + cv::THRESH_OTSU);

    cv::Mat centers;
    binary.copyTo(centers, edges);

    cv::Mat foreground;
    cv::erode(centers, foreground, cv::Mat::ones(5, 5, CV_8U));

    cv::Mat background;
    cv::dilate(foreground, background, cv::Mat::ones(15, 15, CV_8U));
    cv::threshold(background, background, 1, 64, cv::THRESH_BINARY_INV);

    cv::Mat markers;
    cv::Mat mask = foreground + background;
    mask.convertTo(markers, CV_32S);

    return markers;
}

dejavu::Scene_WATERSHED::Scene_WATERSHED(const cv::Mat &bgr, int lowest) {
    cv::Mat m = saturation_mask(bgr);
    cv::Mat b = boundaries(bgr, m);
    extract(bgr, b, lowest, rois, views);
}

const cv::Mat &dejavu::Scene_WATERSHED::landmark(int index) const {
    return views.at(index);
}

const cv::Rect &dejavu::Scene_WATERSHED::region(int index) const {
    return rois.at(index);
}

int dejavu::Scene_WATERSHED::size() const {
    return rois.size();
}
