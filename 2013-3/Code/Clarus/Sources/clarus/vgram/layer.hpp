#ifndef CLARUS_VGRAM_LAYER_HPP
#define CLARUS_VGRAM_LAYER_HPP

#include <clarus/vgram/neuron.hpp>

#include <map>
#include <stdexcept>
#include <string>

namespace vgram {
    template<class W> class layer;
}

template<class W> class vgram::layer {
    typedef typename W::L L;

    typedef typename W::B B;

    typedef typename W::G G;

    typedef neuron<B, G> R;

    typedef std::map<L, R> Neurons;

    typedef output<G> O;

    Neurons N_z;

public:
    O get(const L &l, const B &b) const;

    void set(const L &l, const B &b, const G &g);
};


template<class W> typename vgram::layer<W>::O vgram::layer<W>::get(const L &l, const B &b) const {
    typename Neurons::const_iterator n = N_z.find(l);
    if (n == N_z.end()) {
        return O();
    }

    const R &r = n->second;
    return r.get(b);
}

template<class W> void vgram::layer<W>::set(const L &l, const B &b, const G &g) {
    N_z[l].set(b, g);
}

#endif
