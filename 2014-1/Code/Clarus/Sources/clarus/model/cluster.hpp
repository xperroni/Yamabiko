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

#ifndef CLARUS_MODEL_CLUSTER_HPP
#define CLARUS_MODEL_CLUSTER_HPP

#include <clarus/core/list.hpp>
#include <clarus/model/point.hpp>

namespace clarus {
    class Cluster;

    double angle2d(const Cluster &cluster);

    double distance(const Cluster &cluster);

    double distance(const Cluster &c1, const Cluster &c2);

    Point median(const Cluster &cluster, int axis);
}

class clarus::Cluster: public List<Point> {
public:
    /*
    Default constructor.
    */
    Cluster();

    /*
    Virtual destructor. Enforces polymorphism. Do not remove.
    */
    virtual ~Cluster();

    Cluster clone() const;

    Point center() const;

    Point closest() const;

    Point farthest() const;

    double spread() const;
};

#endif

