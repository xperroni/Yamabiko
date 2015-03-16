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

#include <clarus/core/bag.hpp>
using clarus::Bag;
using std::string;

Bag::Bag() {
    // Nothing to do.
}

Bag::~Bag() {
    // Nothing to do.
}

bool Bag::has(const string &name) const {
    if (entries.count(name) == 0) {
        return false;
    }

    const Entry::P &entry = entries.find(name)->second;
    return (entry.get() != NULL);
}
