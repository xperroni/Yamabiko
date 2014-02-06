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

#include "parameters.hpp"

#define CHANNELS 4

zeta::Parameters::Parameters(
    size_t length,
    int samples,
    double sigma,
    const cv::Size &images,
    const cv::Size &depths
):
    r(length),
    m_N(depths.height),
    n_N(samples),
    m_H(depths.height),
    n_H(depths.width),
    m_P(images.height / m_H),
    n_P(images.width / n_H)
{
    cv::Size field(n_P, m_P);
    for (int i = 0; i < m_N; i++) {
        for (int j = 0; j < n_N; j++) {
            for (int k = 0; k < CHANNELS; k++) {
                synapses[S(i, j, k)] = clarus::sampler(
                    length / CHANNELS,
                    clarus::sampler::gaussian(sigma, field)
                );
            }
        }
    }
}
