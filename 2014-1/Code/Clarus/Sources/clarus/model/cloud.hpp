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

#ifndef CLARUS_MODEL_CLOUD_HPP
#define CLARUS_MODEL_CLOUD_HPP

#include <clarus/core/list.hpp>
#include <clarus/model/cluster.hpp>

namespace clarus {
    class Cloud;

    /*
    Returns the list of clusters in A whose closest cluster in B is at a
    minimum length T.
    */
    Cloud difference(const Cloud &a, const Cloud &b, double t);
}

class clarus::Cloud: public List<Cluster> {
public:
    /*
    Default constructor.
    */
    Cloud();

    /*
    Virtual destructor. Enforces polymorphism. Do not remove.
    */
    virtual ~Cloud();

    const Cluster &closest() const;
};

#endif
