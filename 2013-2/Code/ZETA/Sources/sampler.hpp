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

#ifndef ZETA_SAMPLER_HPP
#define ZETA_SAMPLER_HPP

#include "parameters.hpp"

#include <stdexcept>

namespace zeta {
    template<class B> class Sampler;
}

template<class B> class zeta::Sampler {
    const Parameters &p;

    List<X> x;

public:
    Sampler(const Parameters &p, const X &x);

    B operator () (const L &l, const J &j) const;
};

template<class B> zeta::Sampler<B>::Sampler(const Parameters &_p, const X &_x):
    p(_p)
{
    cv::split(_x, x);

    cv::Mat hls;
    std::vector<cv::Mat> channels;
    cv::cvtColor(_x, hls, CV_BGR2HLS);
    cv::split(hls, channels);

    x.push_back(channels[1]);
}

template<class B> B zeta::Sampler<B>::operator () (const L &l, const J &j) const {
    B bits;
    int d = 0;
    cv::Rect roi(j[1] * p.n_P, j[0] * p.m_P, p.n_P, p.m_P);
    for (int k = 0, n = x.size(); k < n; k++) {
        S s(l[0], l[1], k);
        std::map<S, clarus::sampler>::const_iterator si = p.synapses.find(s);
        if (si == p.synapses.end()) {
            throw std::runtime_error(std::string("No synapses found for index ") + (std::string) s);
        }

        const clarus::sampler &sampler = si->second;
        cv::Mat c_k(x[k], roi);
        cv::Mat f_k = fourier::transform(c_k);
        cv::Mat p_k = fourier::magnitude(f_k);
        cv::Mat samples = sampler.sample<uchar>(p_k);
        d = vgram::minchinton<B, uchar>(bits, d, samples);
    }

    return bits;
}

#endif
