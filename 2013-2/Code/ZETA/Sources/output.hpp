/*
This file is part of ZETA.

ZETA is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

ZETA is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with ZETA.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ZETA_OUTPUT_HPP
#define ZETA_OUTPUT_HPP

#include "domain.hpp"

#include <boost/shared_ptr.hpp>

namespace zeta {
    class Output;
}

class zeta::Output {
    boost::shared_ptr<Y> y;

    size_t d;

public:
    Output(boost::shared_ptr<Y> &y, size_t d);

    const Y &output() const;

    size_t distance() const;
};

#endif
