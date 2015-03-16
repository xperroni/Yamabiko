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

#ifndef CLARUS_MODEL_BALLOT_WEIGHTED_HPP
#define CLARUS_MODEL_BALLOT_WEIGHTED_HPP

#include <map>

namespace ballot {
    template<class X> class weighted;
}

template<class X> class ballot::weighted {
    std::map<X, int> counts;

    std::map<X, double> values;

    std::map<X, double> errors;

    double lead;

    const X *best;

    double least;

public:
    weighted();

    void add(const X &x, double e);

    const X &winner() const;

    double error() const;

    double likelihood() const;
};

template<class X> ballot::weighted<X>::weighted():
    counts(),
    values(),
    errors(),
    lead(0.0),
    best(NULL),
    least(0.0)
{
    // Nothing to do.
}

template<class X> void ballot::weighted<X>::add(const X &x, double e) {
    int &count = counts[x];
    count += 1;

    double &value = values[x];
    value += 1.0 / (1 + e);

    double &error = errors[x];
    error += (1.0 / count) * (e - error);

    if (value > lead) {
        lead = value;
        best = &(values.find(x)->first);
        least = error;
    }
}

template<class X> const X &ballot::weighted<X>::winner() const {
    return *best;
}

template<class X> double ballot::weighted<X>::error() const {
    return least;
}

template<class X> double ballot::weighted<X>::likelihood() const {
    return lead;
}

#endif
