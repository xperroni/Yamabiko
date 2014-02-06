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

#ifndef ZETA_SAMPLER_DITHER_HPP
#define ZETA_SAMPLER_DITHER_HPP

#include "channels.hpp"

#include <stdexcept>

namespace zeta {
    template<class B> class DitherSampler;
}

template<class B> class zeta::DitherSampler: public Channels {
    std::map<J, B> bitstrings;

public:
    DitherSampler(const Parameters &p, const X &x);

    F operator () (const J &j) const;

    const B &operator () (const L &l, const F &f, const J &j) const;
};

template<class B> zeta::DitherSampler<B>::DitherSampler(const Parameters &p, const X &_x):
    Channels(p, _x)
{
    std::vector<cv::Mat> d;
    for (int i = 0; i < CHANNELS; i++) {
        d.push_back(dither::transform(channel(i), 1));
    }

    for (int i = 0; i < p.m_H; i++) {
        for (int j = 0; j < p.n_H; j++) {
            size_t b = 0;
            B &bits = bitstrings[J(i, j)];
            for (std::vector<cv::Mat>::const_iterator k = d.begin(), z = d.end(); k != z; ++k) {
                const cv::Mat &dithered = *k;
                for (int u = i * side, m = u + side; u < m; u++) {
                    for (int v = j * side, n = v + side; v < n; v++) {
                        bits.set(b++, dithered.at<uchar>(u, v) > 0);
                    }
                }
            }
        }
    }
}

template<class B> zeta::F zeta::DitherSampler<B>::operator () (const J &j) const {
    return Channels::operator () (j);
}

template<class B> const B &zeta::DitherSampler<B>::operator () (const L &l, const F &f, const J &j) const {
    typename std::map<J, B>::const_iterator i = bitstrings.find(j);
    if (i == bitstrings.end()) {
        throw std::runtime_error(std::string("No patch found for index ") + (std::string) j);
    }

    return i->second;
}

#endif
