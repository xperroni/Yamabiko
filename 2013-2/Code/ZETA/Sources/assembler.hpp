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

#ifndef ZETA_ASSEMBLER_HPP
#define ZETA_ASSEMBLER_HPP

#include "output.hpp"
#include "parameters.hpp"

namespace zeta {
    class Assembler;
}

class zeta::Assembler {
    const Parameters &p;

    List<cv::Mat> depths;

    List<cv::Mat> errors;

public:
    Assembler(const Parameters &p);

    void add(const L &l, const J &j, const G &g, size_t d);

    Output operator () () const;
};

#endif
