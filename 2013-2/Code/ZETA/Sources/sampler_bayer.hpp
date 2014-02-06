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

#ifndef ZETA_SAMPLER_BAYER_HPP
#define ZETA_SAMPLER_BAYER_HPP

#include "parameters.hpp"

namespace zeta {
    template<class B> class BayerSampler;
}

template<class B> class zeta::BayerSampler {
    const Parameters &p;

    X x;

    Index<2> offset;

public:
    BayerSampler(const Parameters &p, const X &x);

    B operator () (const L &l, const J &j) const;
};

template<class B> zeta::BayerSampler<B>::BayerSampler(const Parameters &_p, const X &_x):
    p(_p),
    x(_x),
    offset((p.m_P - 16) / 2, (p.n_P - 16) / 2)
{
    // Nothing to do.
}

template<class B> B zeta::BayerSampler<B>::operator () (const L &l, const J &j) const {
    B bits;
    size_t i = 0;
    int c = 0;
    int k[] = {0, 2};
    for (int u = p.m_P * j[0] + offset[0], m = u + 16; u < m; u++) {
        for (int v = p.n_P * j[1] + offset[1], n = v + 16; v < n; v++) {
            cv::Vec3b pixel = x.at<cv::Vec3b>(u, v);
            bits.set(i++, pixel[1] > pixel[k[c]]);
            c ^= 1;
        }
    }

    return bits;
}

#endif
