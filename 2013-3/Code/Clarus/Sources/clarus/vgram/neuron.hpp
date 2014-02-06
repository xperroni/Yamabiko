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

#ifndef CLARUS_VGRAM_NEURON_HPP
#define CLARUS_VGRAM_NEURON_HPP

#include <clarus/vgram/output.hpp>

#include <boost/shared_ptr.hpp>

#include <limits>
#include <vector>

#include <cstdio>

namespace vgram {
    template<class B, class G> class neuron;
}

template<class B, class G> class vgram::neuron {
    typedef std::map<B, G> Memory;

    boost::shared_ptr<Memory> M;

public:
    neuron();

    output<G> get(const B &b) const;

    void set(const B &b, const G &g);
};

template<class B, class G> vgram::neuron<B, G>::neuron():
    M(new Memory())
{
    // Nothing to do.
}

template<class B, class G> vgram::output<G> vgram::neuron<B, G>::get(const B &b) const {
    typename Memory::const_iterator v = M->find(b);
    if (v != M->end()) {
        return output<G>(v->second, 0);
    }

    const G *g = &(M->begin()->second);
    size_t d = std::numeric_limits<size_t>::max();
    for (typename Memory::const_iterator i = M->begin(), n = M->end(); i != n; ++i) {
        const B &b_k = i->first;
        const G &g_k = i->second;
        size_t e = b.distance(b_k);
        if (e >= d) {
            continue;
        }

        g = &g_k;
        d = e;
    }

    return output<G>(*g, d);
}

template<class B, class G> void vgram::neuron<B, G>::set(const B &b, const G &g) {
    (*M)[b] = g;
}

#endif
