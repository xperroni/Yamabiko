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

#include "channels.hpp"

zeta::Channels::Channels(const Parameters &p, const X &_x):
    side(sqrt(p.r / CHANNELS))
{
    cv::Size scale(side * p.n_H, side * p.m_H);

    X bgr;
    cv::resize(_x, bgr, scale);

    cv::split(bgr, x);

    cv::Mat hls;
    cv::cvtColor(bgr, hls, CV_BGR2HLS);
    std::vector<cv::Mat> hls_channels;
    cv::split(bgr, hls_channels);

    x.push_back(hls_channels[1]);

    for (int i = 0; i < p.m_H; i++) {
        for (int j = 0; j < p.n_H; j++) {
            x_j.push_back(J(i, j));
        }
    }
}

zeta::F zeta::Channels::operator () (const J &j) const {
    cv::Rect roi(j[1] * side, j[0] * side, side, side);
    F patch;
    for (int i = 0; i < CHANNELS; i++) {
        cv::Mat c_k(x[i], roi);
        patch.push_back(c_k);
    }

    return patch;
}

const cv::Mat &zeta::Channels::channel(size_t i) {
    return x[i];
}

const List<zeta::J> &zeta::Channels::indices() {
    return x_j;
}
