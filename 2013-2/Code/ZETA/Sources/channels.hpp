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

#ifndef ZETA_CHANNELS_HPP
#define ZETA_CHANNELS_HPP

#include "parameters.hpp"

namespace zeta {
    const int CHANNELS = 4;

    class Channels;
}

class zeta::Channels {
    List<cv::Mat> x;

    List<J> x_j;

protected:
    const size_t side;

public:
    Channels(const Parameters &p, const X &x);

    F operator () (const J &j) const;

    const cv::Mat &channel(size_t i);

    const List<J> &indices();
};

#endif
