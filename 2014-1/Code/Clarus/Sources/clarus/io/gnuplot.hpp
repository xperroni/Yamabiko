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

#ifndef GNUPLOT_HPP
#define GNUPLOT_HPP

#include <clarus/io/command.hpp>
#include <clarus/model/point.hpp>

#include <boost/function.hpp>

#include <cstdio>
#include <string>

namespace clarus {
    class Gnuplot;

    void line2d(Gnuplot &gnuplot, int index, const clarus::Point &p0, const clarus::Point &p1);

    void plot2d(Gnuplot &gnuplot, const std::string &path, int c0, int c1);
}

class clarus::Gnuplot: public clarus::OutputCommand {
public:
    typedef boost::function<void(Gnuplot&)> S;

    static void NOTHING(Gnuplot& gnuplot);

    /*
    Set some sensible defaults.
    */
    static void DEFAULTS(Gnuplot &gnuplot);

    Gnuplot(S setup = NOTHING);
};

#endif
