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

#include <clarus/io/camera.hpp>
using clarus::locked_ptr;
using clarus::Loop;
using clarus::Looper;
using clarus::Camera;
using std::string;

#include <stdexcept>

Camera::Camera(int index) {
    looper.set<int>("index", index);
}

Camera::~Camera() {
    // Nothing to do.
}

static void display_feed(Looper &looper, Loop &loop) {
    while (loop.running()) {
        string &title = loop.get<string>("title");
        locked_ptr<cv::VideoCapture> camera = looper.write<cv::VideoCapture>("camera");

        cv::Mat frame;
        camera->read(frame);
        cv::imshow(title, frame);
        cv::waitKey(30);
    }
}

void Camera::display(const string &title) {
    if (!running()) {
        start();
    }

    looper.set<string>("title", title);
    looper.start(display_feed);
}

void Camera::hide() {
    looper.stop(true);
    string title = looper.fetch<string>("title");
    cv::destroyWindow(title);
}

void Camera::start() {
    int index = looper.fetch<int>("index");
    cv::VideoCapture *camera = new cv::VideoCapture(index);
    if(!camera->isOpened()) {
        delete camera;
        throw std::runtime_error("Could not connect to camera");
    }

    looper.set<cv::VideoCapture>("camera", camera);

    // Hack to avoid the first few frames to be the same.
    for (int i = 0; i < 10; i++) {
        grab();
    }
}

void Camera::stop() {
    hide();
    looper.set<cv::VideoCapture>("camera", NULL);
}

cv::Mat Camera::grab() {
    if (!running()) {
        start();
    }

    locked_ptr<cv::VideoCapture> locked = looper.write<cv::VideoCapture>("camera");

    cv::Mat frame;
    locked->read(frame);
    return frame;
}

bool Camera::running() {
    return looper.has("camera");
}
