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

#include <clarus/io/gnuplot.hpp>
using clarus::Gnuplot;
using clarus::OutputCommand;

#include <fstream>

Gnuplot::Gnuplot(S setup):
    OutputCommand("cat %s | gnuplot")
{
    setup(*this);
}

void Gnuplot::NOTHING(Gnuplot& gnuplot) {
    // Nothing to do.
}

void Gnuplot::DEFAULTS(Gnuplot &gnuplot) {
    gnuplot("set terminal x11");   /* drawing destination */
    gnuplot("set grid");  /* draw grid */
    gnuplot("set mouse");  /* use mouse */
    gnuplot("set xlabel \"x\"");  /* label of x-axis */
    gnuplot("set ylabel \"y\"");  /* label of y-axis */
    gnuplot("set trange [-10:10]");  /* range of function parameter t */
    gnuplot("set xrange [-10:10]");  /* range of x-axis */
    gnuplot("set yrange [-10:10]");  /* range of y-axis */
    gnuplot("set size ratio -1");  /* aspect ratio */
    gnuplot("unset key");  /* hide graph legends */
}

void clarus::line2d(Gnuplot &gnuplot, int index, const clarus::Point &p0, const clarus::Point &p1) {
    std::string no = domain::to_string(index);
    std::string x1 = domain::to_string(p0[0]);
    std::string x2 = domain::to_string(p1[0]);
    std::string y1 = domain::to_string(p0[1]);
    std::string y2 = domain::to_string(p1[1]);

    std::string line = "set arrow " + no + " nohead from "
        + x1 + "," + y1
        + ",0 to "
        + x2 + "," + y2
        + ",0";

    gnuplot(line);
}

void clarus::plot2d(Gnuplot &gnuplot, const std::string &path, int c0, int c1) {
    gnuplot("plot '-' using %d:%d with points notitle", c0, c1);

    std::ifstream file(path.c_str());
    for (std::string line; std::getline(file, line) && line != "";) {
        gnuplot(line);
    }

    gnuplot("e");
    gnuplot.flush();
}
