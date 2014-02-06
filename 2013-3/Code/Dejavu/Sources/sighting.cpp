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

#include "sighting.hpp"

dejavu::Sighting::Sighting():
    Tuple<int, 4>(0, 0, 0, 0)
{
    // Nothing to do.
}

dejavu::Sighting::Sighting(const cv::Rect &roi):
    Tuple<int, 4>(0, 0, 0, 0)
{
    Tuple<int, 4> &s = *this;
    s[0] = roi.width;
    s[1] = roi.height;
    s[2] = roi.x;
    s[3] = roi.y;
}
