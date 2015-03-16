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

#include <clarus/core/math.hpp>
using clarus::List;
using clarus::ListIteratorConst;

#include <cmath>

List<double> clarus::vectorize(BinaryOp op, const List<double> &a, const List<double> &b) {
    List<double> c;
    for (ListIteratorConst<double> i(a), j(b); i.more() && j.more();) {
        double value = op(i.next(), j.next());
        c.append(value);
    }

    return c;
}

List<double> clarus::vectorize(UnaryOp op, const List<double> &a) {
    List<double> b;
    for (ListIteratorConst<double> i(a); i.more();) {
        double value = op(i.next());
        b.append(value);
    }

    return b;
}

double clarus::add(double a, double b) {
    return a + b;
}

double clarus::sub(double a, double b) {
    return a - b;
}

double clarus::mul(double a, double b) {
    return a * b;
}

double clarus::div(double a, double b) {
    return a / b;
}

double clarus::log(double x, double base) {
    return ::log(x) / ::log(base);
}

double clarus::log2(double x) {
    static double BASE2 = 1.0 / ::log(2.0);

    return ::log(x) * BASE2;
}

