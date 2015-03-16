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

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <clarus/core/looper.hpp>

#include <opencv2/opencv.hpp>

#include <string>

namespace clarus {
    class Camera;
}

class clarus::Camera {
    Looper looper;

public:
    Camera(int index = 0);

    /*
    Object destructor. Enforces polymorphism. Do not remove.
    */
    virtual ~Camera();

    void display(const std::string &title = std::string("Camera"));

    void hide();

    void start();

    void stop();

    cv::Mat grab();

    bool running();
};

#endif
