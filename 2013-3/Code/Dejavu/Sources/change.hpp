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

#ifndef DEJAVU_CHANGE_HPP
#define DEJAVU_CHANGE_HPP

#include "sighting.hpp"

#include <clarus/clarus.hpp>

#include <boost/function.hpp>

namespace dejavu {
    class Change;
}

class dejavu::Change: public Tuple<int, 4> {
    int e;

    List<Sighting> sightings;

public:
    Change();

    Change(const cv::Rect &roi1, const cv::Rect &roi2);

    Change(int e, const cv::Rect &roi1, const cv::Rect &roi2);

    const Sighting &sighting(size_t index) const;

    int error() const;
};

#endif
