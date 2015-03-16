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

#ifndef CLARUS_VISION_SURFER_HPP
#define CLARUS_VISION_SURFER_HPP

#include <clarus/core/list.hpp>
#include <clarus/model/point.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>

namespace clarus {
    class Surfer;
}

class clarus::Surfer {
    /* SURF features detector used by this object. */
    cv::SurfFeatureDetector detector;

    /* SURF features extractor used by this object. */
    cv::SurfDescriptorExtractor extractor;

    /* Feature matcher used by this object. */
    cv::FlannBasedMatcher matcher;

public:
    struct Descriptor {
        cv::Mat image;

        List<cv::KeyPoint> points;

        cv::Mat features;

        Descriptor(Surfer &surfer, const cv::Mat &image);
    };

    struct Matches: public List<cv::DMatch> {
        double closest;

        Matches(Surfer &surfer, const Descriptor &target, const Descriptor &scene, bool sorted = false);
    };

    friend struct Descriptor;

    friend struct Matches;

    Surfer(int hessian = 400);

    /*
    Virtual destructor. Enforces polymorphism. Do not remove.
    */
    virtual ~Surfer();

    List<Point> match(const cv::Mat &target, const cv::Mat &scene, bool sorted = false);

    List<Point> match(const Descriptor &target, const cv::Mat &scene, bool sorted = false);

    List<Point> match(const Descriptor &target, const Descriptor &scene, bool sorted = false);

    List<Point> track(const cv::Mat &target, const cv::Mat &scene);

    List<Point> track(const Descriptor &target, const cv::Mat &scene);

    List<Point> track(const Descriptor &target, const Descriptor &scene);

};

#endif
