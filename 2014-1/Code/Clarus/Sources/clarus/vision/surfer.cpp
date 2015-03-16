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

#include <clarus/vision/surfer.hpp>
using clarus::List;
using clarus::Surfer;
using clarus::Point;
using clarus::Point2D;

#include <clarus/vision/colors.hpp>

#include <vector>
using std::vector;

Surfer::Descriptor::Descriptor(Surfer &surfer, const cv::Mat &_image):
    image(_image)
{
    surfer.detector.detect(image, *points);
    surfer.extractor.compute(image, *points, features);
}

static bool cmp(const cv::DMatch &m1, const cv::DMatch &m2) {
    return (m1.distance < m2.distance);
}

// http://robocv.blogspot.jp/2012/02/real-time-object-detection-in-opencv.html
Surfer::Matches::Matches(
    Surfer &surfer,
    const Surfer::Descriptor &target,
    const Surfer::Descriptor &scene,
    bool sorted
) {
    closest = DBL_MAX;
    std::vector<std::vector<cv::DMatch> > matches;
    surfer.matcher.knnMatch(target.features, scene.features, matches, 2);
    for(int i = 0, n = std::min(scene.features.rows - 1, (int) matches.size()); i < n; i++) {
        std::vector<cv::DMatch> &match = matches[i];
        if (matches.size() < 2) {
            continue;
        }

        cv::DMatch &match0 = match[0];
        cv::DMatch &match1 = match[1];
        double d = match0.distance;
        if (d < 0.6 * match1.distance) {
            closest = std::min(d, closest);
            append(match0);
        }
    }

    if (sorted) {
        sort(*this, cmp);
    }
/*
    List<cv::DMatch> &matches = *this;
    surfer.matcher.match(target.features, scene.features, *matches);
    if (sorted) {
        sort(*this, cmp);
        closest = matches[0].distance;
        return;
    }

    closest = DBL_MAX;
    for(int i = 0, n = matches.size(); i < n; i++) {
        double d = matches[i].distance;
        closest = std::min(d, closest);
    }
*/
}

Surfer::Surfer(int hessian):
    detector(hessian),
    extractor(),
    matcher()
{
    // Nothing to do.
}

Surfer::~Surfer() {
    // Nothing to do.
}

List<Point> Surfer::match(const cv::Mat &target, const cv::Mat &scene, bool sorted) {
    return match(Descriptor(*this, target), Descriptor(*this, scene), sorted);
}

List<Point> Surfer::match(const Descriptor &target, const cv::Mat &scene, bool sorted) {
    return match(target, Descriptor(*this, scene), sorted);
}

// http://docs.opencv.org/2.4.9/doc/tutorials/features2d/feature_flann_matcher/feature_flann_matcher.html
List<Point> Surfer::match(const Descriptor &target, const Descriptor &scene, bool sorted) {
    Matches matches(*this, target, scene, sorted);

    List<Point> points;
    double d_max = 3 * matches.closest;
    const List<cv::KeyPoint> &keypoints = scene.points;
    for(ListIteratorConst<cv::DMatch> i(matches); i.more();) {
        const cv::DMatch &match = i.next();
        if (match.distance > d_max) {
            continue;
        }

        int j = match.trainIdx;
        const cv::Point2f &p = keypoints[j].pt;
        points.append(Point2D(p));
    }

    return points;
}

List<Point> Surfer::track(const cv::Mat &target, const cv::Mat &scene) {
    return track(Descriptor(*this, target), Descriptor(*this, scene));
}

List<Point> Surfer::track(const Descriptor &target, const cv::Mat &scene) {
    return track(target, Descriptor(*this, scene));
}

inline List<cv::Point2f> target_corners(const cv::Mat &l) {
    List<cv::Point2f> corners(4);
    corners[0] = cv::Point2f(0, 0);
    corners[1] = cv::Point2f(l.cols, 0);
    corners[2] = cv::Point2f(l.cols, l.rows);
    corners[3] = cv::Point2f(0, l.rows);
    return corners;
}

// http://docs.opencv.org/doc/tutorials/features2d/feature_homography/feature_homography.html
// http://robocv.blogspot.jp/2012/02/real-time-object-detection-in-opencv.html
List<Point> Surfer::track(const Descriptor &target, const Descriptor &scene) {
    Matches matches(*this, target, scene);
    if (matches.size() < 4) {
        return List<Point>();
    }

    List<cv::Point2f> points_t;
    List<cv::Point2f> points_s;
    const List<cv::KeyPoint> &keypoints_t = target.points;
    const List<cv::KeyPoint> &keypoints_s = scene.points;
    for(ListIteratorConst<cv::DMatch> i(matches); i.more();) {
        const cv::DMatch &match = i.next();
        points_t.append(keypoints_t[match.queryIdx].pt);
        points_s.append(keypoints_s[match.trainIdx].pt);
    }

    List<cv::Point2f> corners;
    cv::Mat h = cv::findHomography(*points_t, *points_s, CV_RANSAC);
    cv::perspectiveTransform(*target_corners(target.image), *corners, h);
    return Point2D(corners);
}
