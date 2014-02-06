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

#ifndef DEJAVU_DIFFERENCE_HPP
#define DEJAVU_DIFFERENCE_HPP

#include "change.hpp"
#include "mode.hpp"
#include "movement.hpp"
#include "parameters.hpp"
#include "scene.hpp"

#include <boost/function.hpp>

namespace dejavu {
    class Difference;
}

class dejavu::Difference {
    Position p;

    List<Assignment> matches;

    List<Change> changes;

public:
    Difference(
        const Position &p,
        const Scene &s1,
        const Scene &s2,
        Mode mode = UNKNOWN,
        bool trim = false
    );

    const Change &change(size_t index) const;

    double error(size_t index) const;

    const Assignment &match(size_t index) const;

    const Position &position() const;

    size_t size() const;
};

#endif
