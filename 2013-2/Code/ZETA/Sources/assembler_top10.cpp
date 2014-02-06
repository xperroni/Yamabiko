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

#include "assembler_top10.hpp"

zeta::AssemblerTop10::AssemblerTop10(const Parameters &_p):
    p(_p)
{
    for (int z = 0, n = p.n_N; z < n; z++) {
        depths.push_back(cv::Mat::zeros(p.m_H, p.n_H, CV_32F));

        cv::Mat e(p.m_H, p.n_H, CV_32S, cv::Scalar(std::numeric_limits<int>::max()));
        errors.push_back(e);
    }
}

void zeta::AssemblerTop10::add(const L &l, const J &j, const G &g, size_t d) {
    size_t a = j[0];
    size_t b = j[1];
    size_t z = l[1];

    depths[z].at<G>(a, b) = g;

    errors[z].at<size_t>(a, b) = d;
}

zeta::Output zeta::AssemblerTop10::operator () () const {
    boost::shared_ptr<Y> y_k(new cv::Mat_<G>(p.m_H, p.n_H));
    size_t d_k = 0.0;

    for (int a = 0; a < p.m_H; a++) {
        for (int b = 0; b < p.n_H; b++) {
            std::map<size_t, G> top10;
            for (int z = 0; z < p.n_N; z++) {
                G g = depths[z].at<G>(a, b);
                size_t d = errors[z].at<size_t>(a, b);
                top10[d] = g;
            }

            G g_kj = 0.0;
            G w_kj = 0.0;
            size_t k = p.n_N / 10;
            for (
                std::map<size_t, G>::iterator i = top10.begin(), n = top10.end();
                (k--) > 0 && i != n;
                ++i
            ) {
                size_t d = i->first;
                d_k += d;

                G g = i->second;
                G w = 1.0 / ((G) (d + 1));
                g_kj += g * w;
                w_kj += w;
            }

            y_k->at<G>(a, b) = g_kj / w_kj;
        }
    }

    d_k /= p.m_H * p.n_H * (p.n_N / 10);

    return Output(y_k, d_k);
}
