/*
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
along with Clarus.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GNUPLOT_HPP
#define GNUPLOT_HPP

#include <clarus/io/command.hpp>

#include <cstdio>
#include <string>

class Gnuplot: public OutputCommand {
public:
    Gnuplot();
};

void plot2d(Gnuplot &gnuplot, const std::string &path, int c0, int c1);

/*
Set some sensible defaults.
*/
void set_defaults(Gnuplot &gnuplot);

#endif // GNUPLOT_HPP
