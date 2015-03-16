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

#include <clarus/model/cloud.hpp>
using clarus::distance2;
using clarus::Cloud;
using clarus::Cluster;
using clarus::ListIteratorConst;

Cloud::Cloud():
    List<Cluster>()
{
    // Nothing to do.
}

Cloud::~Cloud() {
    // Nothing to do.
}

const Cluster &Cloud::closest() const {
    double d = std::numeric_limits<double>::max();
    double j = 0;
    for (int i = 0, n = size(); i < n; i++) {
        double e = distance(at(i));
        if (e < d) {
            d = e;
            j = i;
        }
    }

    return at(j);
}

static bool no_close_matches(const clarus::Point &p, const clarus::Cloud &b, double t2) {
    for (ListIteratorConst<Cluster> j(b); j.more();) {
        const Cluster &v = j.next();
        if (distance2(p, v.center()) <= t2) {
            return false;
        }
    }

    return true;
}

Cloud clarus::difference(const Cloud &a, const Cloud &b, double t) {
    Cloud c;
    double t2 = t * t;
    for (ListIteratorConst<Cluster> i(a); i.more();) {
        const Cluster &u = i.next();
        if (no_close_matches(u.center(), b, t2)) {
            c.append(u);
        }
    }

    return c;
}
