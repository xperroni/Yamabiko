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

#ifndef DEJAVU_LANDMARKS_HPP
#define DEJAVU_LANDMARKS_HPP

#include <clarus/clarus.hpp>

namespace dejavu {
    class Scene_WATERSHED;
}

class dejavu::Scene_WATERSHED {
    List<cv::Rect> rois;

    List<cv::Mat> views;

public:
    Scene_WATERSHED(const cv::Mat &bgr, int lowest = 256);

    const cv::Mat &landmark(int index) const;

    const cv::Rect &region(int index) const;

    int size() const;
};

#endif
