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

#include "assembler.hpp"

zeta::Assembler::Assembler(const Parameters &_p):
    p(_p)
{
    for (int z = 0, n = p.n_N; z < n; z++) {
        depths.push_back(cv::Mat::zeros(p.m_H, p.n_H, CV_32F));

        cv::Mat e(p.m_H, p.n_H, CV_32S, cv::Scalar(std::numeric_limits<int>::max()));
        errors.push_back(e);
    }
}

void zeta::Assembler::add(const L &l, const J &j, const G &g, size_t d) {
    size_t a = j[0];
    size_t b = j[1];
    size_t z = l[1];

    depths[z].at<G>(a, b) = g;

    errors[z].at<size_t>(a, b) = d;
}

zeta::Output zeta::Assembler::operator () () const {
    boost::shared_ptr<Y> y(new cv::Mat_<G>(p.m_H, p.n_H));
    size_t d = 0.0;

    for (int a = 0; a < p.m_H; a++) {
        for (int b = 0; b < p.n_H; b++) {
            G g_k = 0.0;
            G w_k = 0.0;

            for (int z = 0; z < p.n_N; z++) {
                G g = depths[z].at<G>(a, b);
                size_t d_z = errors[z].at<size_t>(a, b);
                G w = 1.0 / ((G) (d_z + 1));
                d += d_z;
                g_k += g * w;
                w_k += w;
            }

            y->at<G>(a, b) = g_k / w_k;
        }
    }

    d /= p.m_H * p.n_H * p.n_N;

    return Output(y, d);
}
