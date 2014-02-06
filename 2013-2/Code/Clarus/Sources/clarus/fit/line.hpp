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

#ifndef LINE_HPP
#define LINE_HPP

#include <string>
#include <vector>

class LineModel {
public:
    typedef double Domain;

    typedef std::pair<Domain, Domain> Datum;

    typedef std::vector<Datum> Data;

    typedef Domain Distance;

    Domain a;

    Domain b;

    Domain min_xd;

    double fitness;

    LineModel();

    LineModel(const Domain &min_xd);

    double angle();

    void fit(Data &data);

    Distance distance(const Datum &datum) const;

    Distance distance(const LineModel &that) const;

    Domain eval(const Domain &x) const;

    std::string cartesian();

    std::string parametric();
};

#endif
