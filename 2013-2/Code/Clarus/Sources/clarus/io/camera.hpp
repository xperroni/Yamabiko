/*
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
along with Clarus.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <clarus/core/locker.hpp>

#include <boost/shared_ptr.hpp>
#include <opencv2/opencv.hpp>

class Camera
{
    boost::shared_ptr<cv::VideoCapture> camera;

    Locker locker;

public:
    Camera();

    Camera(int index);

    void start(int index = 0);

    void stop();

    cv::Mat grab();
};

#endif // CAMERA_HPP
