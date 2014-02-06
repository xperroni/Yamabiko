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

#include "camera.hpp"

#include <stdexcept>

Camera::Camera() {
    camera.reset();
}

Camera::Camera(int index) {
    camera.reset();
    start(index);
}

void Camera::start(int index) {
    camera.reset(new cv::VideoCapture(index));
    if(!camera->isOpened()) {
        throw std::runtime_error("Could not connect to camera");
    }

    // Hack to avoid the first few frames to be the same.
    for (int i = 0; i < 10; i++) {
        grab();
    }
}

void Camera::stop() {
    camera.reset();
}

cv::Mat Camera::grab() {
    cv::Mat frame;
    locked_ptr<cv::VideoCapture> locked = locker.write(*camera);
    locked->read(frame);
    return frame;
}
