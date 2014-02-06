/*
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
along with Clarus.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CLARUS_VGRAM_NETWORK_HPP
#define CLARUS_VGRAM_NETWORK_HPP

#include <clarus/vgram/neuron.hpp>

namespace vgram {
    template<class W> class network;
}

template <class W> class vgram::network {
    typedef typename W::X X;

    typedef typename W::Y Y;

    typedef typename W::B B;

    typedef typename W::F F;

    typedef typename W::G G;

    typedef typename W::J J;

    typedef typename W::L L;

    typedef typename W::N N;

    typedef typename W::O O;

    typedef typename W::P P;

    typedef typename W::alpha alpha;

    typedef typename W::beta beta;

    typedef typename W::gamma gamma;

    typedef typename W::omega omega;

    P p;

    N N_z;

public:
    network(const P &p);

    O get(const X &x) const;

    void set(const X &x, const Y &y);
};

template<class W> vgram::network<W>::network(const P &_p):
    p(_p)
{
    // Nothing to do.
}

template<class W> typename W::O vgram::network<W>::get(const X &x) const {
    alpha a(p, x);
    omega w(p);

    const List<J> &m = a.indices();
    for (typename List<J>::const_iterator i = m.begin(), n = m.end(); i != n; ++i) {
        const J &j = *i;
        F f = a(j);

        gamma c(p, N_z, f, j);
        for (typename gamma::iterator c_l = c.begin(), c_n = c.end(); c_l != c_n; ++c_l) {
            const L &l = c_l->first;
            neuron<B, G> &nl = c_l->second;
            typename neuron<B, G>::response r = nl.get(a(l, f, j));
            w.add(l, j, r.g, r.d);
        }
    }

    return w();
}

template<class W> void vgram::network<W>::set(const X &x, const Y &y) {
    alpha a(p, x);
    beta b(p, y);

    const List<J> &m = a.indices();
    for (typename List<J>::const_iterator i = m.begin(), n = m.end(); i != n; ++i) {
        const J &j = *i;
        F f = a(j);
        G g = b(j);

        gamma c(p, N_z, f, g, j);
        for (typename gamma::iterator c_l = c.begin(), c_n = c.end(); c_l != c_n; ++c_l) {
            const L &l = c_l->first;
            neuron<B, G> &nl = c_l->second;
            nl.set(a(l, f, j), g);
        }
    }
}

#endif
