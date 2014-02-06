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

#ifndef ZETA_NEURONS_HPP
#define ZETA_NEURONS_HPP

#include "parameters.hpp"

namespace zeta {
    template<class B> class Neurons;
}

template<class B> class zeta::Neurons: public std::map<L, vgram::neuron<B, G> > {
public:
    Neurons();

    Neurons(const Parameters &p, Neurons &N_z, const F &f, const G &g, const J &j);

    Neurons(const Parameters &p, const Neurons &N_z, const F &f, const J &j);
};

template<class B> zeta::Neurons<B>::Neurons() {
    // Nothing to do.
}

template<class B> zeta::Neurons<B>::Neurons(
    const Parameters &p,
    Neurons &N_z,
    const F &f,
    const G &g,
    const J &j
) {
    static cv::RNG rng;

    size_t i = j[0];
    size_t range = p.n_N;
    for (int j = 0, n = range / 10; j < n; j++) {
        L l(i, rng(range));
        (*this)[l] = N_z[l];
    }
}

template<class B> zeta::Neurons<B>::Neurons(
    const Parameters &p,
    const Neurons &N_z,
    const F &f,
    const J &j
) {
    size_t i = j[0];
    typename Neurons::const_iterator z = N_z.end();
    for (int j = 0, n = p.n_N; j < n; j++) {
        L l(i, j);
        typename Neurons::const_iterator nl = N_z.find(l);
        if (nl != z) {
            (*this)[l] = nl->second;
        }
    }
}

#endif
