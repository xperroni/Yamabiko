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

#include "change.hpp"

dejavu::Change::Change():
    Tuple<int, 4>(0, 0, 0, 0),
    e(std::numeric_limits<int>::max())
{
    // Nothing to do.
}

inline int delta_normalized(int a, int b) {
    float d = ((float) a - b) / ((float) std::min(a, b));
    return d * 100.0;
}

inline int delta_simple(int a, int b) {
    return a - b;
}

inline void absolute_change(dejavu::Change &c, const dejavu::Sighting &a, const dejavu::Sighting &b) {
    c[0] = delta_normalized(b[0], a[0]);
    c[1] = delta_normalized(b[1], a[1]);
    c[2] = delta_simple(b[2], a[2]);
    c[3] = delta_simple(b[3], a[3]);
}

dejavu::Change::Change(const cv::Rect &roi1, const cv::Rect &roi2):
    Tuple<int, 4>(0, 0, 0, 0),
    e(std::numeric_limits<int>::max())
{
    sightings.append(Sighting(roi1));
    sightings.append(Sighting(roi2));
    absolute_change(*this, sightings[0], sightings[1]);
}

dejavu::Change::Change(int error, const cv::Rect &roi1, const cv::Rect &roi2):
    Tuple<int, 4>(0, 0, 0, 0),
    e(error)
{
    sightings.append(Sighting(roi1));
    sightings.append(Sighting(roi2));
    absolute_change(*this, sightings[0], sightings[1]);
}

const dejavu::Sighting &dejavu::Change::sighting(size_t index) const {
    return sightings.at(index);
}

int dejavu::Change::error() const {
    return e;
}
