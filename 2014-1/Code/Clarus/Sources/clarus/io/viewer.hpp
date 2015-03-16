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

#ifndef CLARUS_IO_VIEWER_HPP
#define CLARUS_IO_VIEWER_HPP

#include <opencv2/opencv.hpp>

#include <string>

namespace viewer {
    void plot(const std::string &title, const cv::Mat &data, int x = -1, int y = -1);

    void show(const std::string &title, const cv::Mat &data, int x = -1, int y = -1, bool normalize = false);

    class window {
        const std::string name;

    public:
        window(const std::string &name);

        virtual ~window();

        void operator () (const cv::Mat &mat, int x = -1, int y = -1, bool normalize = false);

        void close();
    };
}

#endif
