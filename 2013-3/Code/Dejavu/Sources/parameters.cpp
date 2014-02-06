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

#include "parameters.hpp"

#include <stdexcept>

dejavu::Parameters::Parameters() {
    bandmarkers.append(71);
    bandmarkers.append(142);
    bandmarkers.append(213);
    bandmarkers.append(284);
    bandmarkers.append(355);
    bandmarkers.append(426);
    bandmarkers.append(497);
    bandmarkers.append(568);
}

size_t dejavu::Parameters::band(size_t u) const {
    if (u < bandmarkers.first()) {
        return 0;
    }

    if (u >= bandmarkers.last()) {
        return bandmarkers.size();
    }

    for (size_t i = 1, n = bandmarkers.size(); i < n; i++) {
        if (bandmarkers[i - 1] <= u && u < bandmarkers[i]) {
            return i;
        }
    }

    throw std::runtime_error("Could not find band for index " + types::to_string(u));
}

size_t dejavu::Parameters::bands() const {
    return bandmarkers.size() + 1;
}
