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

#ifndef DEJAVU_MODE_HPP
#define DEJAVU_MODE_HPP

#include "parameters.hpp"

#include <opencv2/opencv.hpp>

namespace dejavu {
    typedef boost::function<bool(const cv::Rect &r1, const cv::Rect &r2)> Mode;

    bool UNKNOWN(const cv::Rect &r1, const cv::Rect &r2);

    bool SIDEWAYS_LEFT(const cv::Rect &r1, const cv::Rect &r2);

    bool SIDEWAYS_RIGHT(const cv::Rect &r1, const cv::Rect &r2);

    bool TURNING_LEFT(const cv::Rect &r1, const cv::Rect &r2);

    bool TURNING_RIGHT(const cv::Rect &r1, const cv::Rect &r2);

    class FORWARD;

    class BACKWARD;
}

class dejavu::FORWARD {
    const Parameters &p;

public:
    FORWARD(const Parameters &p);

    bool operator () (const cv::Rect &r1, const cv::Rect &r2);
};

class dejavu::BACKWARD {
    const Parameters &p;

public:
    BACKWARD(const Parameters &p);

    bool operator () (const cv::Rect &r1, const cv::Rect &r2);
};

#endif
