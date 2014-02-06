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

#ifndef DEJAVU_MOVEMENT_HPP
#define DEJAVU_MOVEMENT_HPP

#include <clarus/clarus.hpp>

#include <iostream>

namespace dejavu {
    typedef Tuple<double, 3> Position;

    typedef Tuple<double, 3> Directions;

    class Movement;
}

class dejavu::Movement {
    Position p;

    List<Directions> guesses;

    List<double> errors;

public:
    Movement();

    Movement(const Position &p);

    Movement(const Position &p1, const Position &p2);

    Movement(const Movement &m1, const Movement &m2);

    Movement(const Movement &f, const Movement &s, const Movement &t);

    Movement operator - () const;

    void append(const Directions &d, double e);

    const Directions &directions(size_t index = 0) const;

    const Position &origin() const;

    Position destination() const;

    double error(size_t index = 0) const;

    size_t size() const;
};

std::ostream &operator << (std::ostream& out, const dejavu::Position& p);

#endif
