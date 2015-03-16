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

#ifndef CLARUS_CORE_TUPLE_HPP
#define CLARUS_CORE_TUPLE_HPP

#include <cmath>
#include <cstddef>
#include <cstdarg>
#include <sstream>
#include <string>

template<class T, size_t n> class Tuple {
    T values[n];

protected:
    void init(T value, va_list &args);

public:
    Tuple();

    Tuple(T value, ...);

    Tuple(T value, va_list &args);

    Tuple operator + (const Tuple& that) const;

    Tuple operator - (const Tuple& that) const;

    Tuple operator - () const;

    Tuple operator * (const Tuple& that) const;

    Tuple operator / (const Tuple& that) const;

    T &operator [] (size_t index);

    const T &operator [] (size_t index) const;

    bool operator == (const Tuple &that) const;

    bool operator != (const Tuple &that) const;

    bool operator < (const Tuple &that) const;

    operator std::string() const;

    double distance(const Tuple &other) const;

    inline size_t size() const {
        return n;
    }
};

template<class T, size_t n> Tuple<T, n>::Tuple() {
    // Nothing to do.
}

template<class T, size_t n> Tuple<T, n>::Tuple(T value, ...) {
    va_list args;
    va_start(args, value);
    init(value, args);
    va_end(args);
}

template<class T, size_t n> Tuple<T, n>::Tuple(T value, va_list &args) {
    init(value, args);
}

template<class T, size_t n> void Tuple<T, n>::init(T value, va_list &args) {
    values[0] = value;
    for (size_t i = 1; i < n; i++) {
        values[i] = va_arg(args, T);
    }
}

template<class T, size_t n> Tuple<T, n> Tuple<T, n>::operator + (const Tuple& that) const {
    Tuple result;
    for (size_t i = 0; i < n; i++) {
        result.values[i] = this->values[i] + that.values[i];
    }

    return result;
}

template<class T, size_t n> Tuple<T, n> Tuple<T, n>::operator - (const Tuple& that) const {
    Tuple result;
    for (size_t i = 0; i < n; i++) {
        result.values[i] = this->values[i] - that.values[i];
    }

    return result;
}

template<class T, size_t n> Tuple<T, n> Tuple<T, n>::operator - () const {
    Tuple result;
    for (size_t i = 0; i < n; i++) {
        result.values[i] = -(this->values[i]);
    }

    return result;
}

template<class T, size_t n> Tuple<T, n> Tuple<T, n>::operator * (const Tuple& that) const {
    Tuple result;
    for (size_t i = 0; i < n; i++) {
        result.values[i] = this->values[i] * that.values[i];
    }

    return result;
}

template<class T, size_t n> Tuple<T, n> Tuple<T, n>::operator / (const Tuple& that) const {
    Tuple result;
    for (size_t i = 0; i < n; i++) {
        result.values[i] = this->values[i] / that.values[i];
    }

    return result;
}

template<class T, size_t n> T &Tuple<T, n>::operator [] (size_t index) {
    return values[index];
}

template<class T, size_t n> const T &Tuple<T, n>::operator [] (size_t index) const {
    return values[index];
}

template<class T, size_t n> bool Tuple<T, n>::operator == (const Tuple &that) const {
    for (size_t i = 0; i < n; i++) {
        const T &a = this->values[i];
        const T &b = that.values[i];
        if (a != b) {
            return false;
        }
    }

    return true;
}

template<class T, size_t n> bool Tuple<T, n>::operator != (const Tuple &that) const {
    for (size_t i = 0; i < n; i++) {
        const T &a = this->values[i];
        const T &b = that.values[i];
        if (a != b) {
            return true;
        }
    }

    return false;
}

template<class T, size_t n> bool Tuple<T, n>::operator < (const Tuple &that) const {
    for (size_t i = 0; i < n; i++) {
        const T &a = this->values[i];
        const T &b = that.values[i];
        if (a < b) {
            return true;
        }
        else if (b < a) {
            return false;
        }
    }

    return false;
}

template<class T, size_t n> Tuple<T, n>::operator std::string() const {
    std::ostringstream out;
    out << "(";
    for (size_t i = 0;;) {
        out << values[i];
        if (++i >= n) {
            break;
        }

        out << ", ";
    }

    out << ")";

    return out.str();
}

template<class T, size_t n> double Tuple<T, n>::distance(const Tuple &other) const {
    double d = 0.0;
    for (size_t i = 0; i < n; i++) {
        double a = (double) values[i];
        double b = (double) other.values[i];
        d += pow(a - b, 2.0);
    }

    return sqrt(d);
}

#endif
