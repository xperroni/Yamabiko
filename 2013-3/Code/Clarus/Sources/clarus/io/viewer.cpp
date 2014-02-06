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

#include <clarus/io/viewer.hpp>
#include <clarus/vision/colors.hpp>

void viewer::show(const std::string &title, const cv::Mat &data, int x, int y, bool normalize) {
    cv::Mat pixels = (normalize ? colors::discrete(data) : data);

    cv::imshow(title, pixels);

    if (x > -1 && y > -1) {
        cvMoveWindow(title.c_str(), x, y);
    }

    cv::waitKey(1);
}

void viewer::plot(const std::string &title, const cv::Mat &data, int x, int y) {
    show(title, data, x, y, true);
}

viewer::window::window(const std::string &_name):
    name(_name)
{
    cv::namedWindow(name);
}

viewer::window::~window() {
    close();
}

void viewer::window::operator () (const cv::Mat &mat, int x, int y, bool normalize) {
    show(name, mat, x, y, normalize);
}

void viewer::window::close() {
    cv::destroyWindow(name);
}
