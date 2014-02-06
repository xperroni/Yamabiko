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

#ifndef CLARUS_CORE_INDEX_HPP
#define CLARUS_CORE_INDEX_HPP

#include <clarus/core/tuple.hpp>

template<size_t n> class Index: public Tuple<size_t, n> {
public:
    Index();

    Index(size_t i0, ...);
};

template<size_t n> Index<n>::Index():
    Tuple<size_t, n>()
{
    // Nothing to do.
}

template<size_t n> Index<n>::Index(size_t i0, ...) {
    va_list args;
    va_start(args, i0);
    Tuple<size_t, n>::init(i0, args);
    va_end(args);
}

#endif
