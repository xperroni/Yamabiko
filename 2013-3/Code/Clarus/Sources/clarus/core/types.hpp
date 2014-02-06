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

#ifndef CLARUS_CORE_TYPES_HPP
#define CLARUS_CORE_TYPES_HPP

#include <sstream>
#include <string>

namespace types {
    template<class T> void from_string(T &value, const std::string &str) {
        std::istringstream stream(str);
        stream >> value;
    }

    template<> void from_string(bool &value, const std::string &str);

    template<class T> T from_string(const std::string &str) {
        T value;
        from_string(value, str);
        return value;
    }

    template<class T> std::string to_string(const T &value) {
        std::ostringstream stream;
        stream << value;
        return stream.str();
    }

    template<> std::string to_string(const bool &value);
}

#endif
