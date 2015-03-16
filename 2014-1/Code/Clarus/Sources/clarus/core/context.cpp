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

#include <clarus/core/context.hpp>
using clarus::Context;
using std::string;

Context::Context():
    parent(NULL)
{
    // Nothing to do.
}

Context::Context(Context *_parent):
    parent(_parent)
{
    // Nothing to do.
}

Context::~Context() {
    // Nothing to do.
}

bool Context::has(const string &name) {
    bool found = locker.read(bag)->has(name);
    if (!found && parent != NULL) {
        return parent->has(name);
    }

    return found;
}
