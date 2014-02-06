/*
Copyright (c) Helio Perroni Filho <xperroni@gmail.com>

This file is part of Dejavu.

Dejavu is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Dejavu is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Dejavu. If not, see <http://www.gnu.org/licenses/>.
*/

#include "mode.hpp"

#include "change.hpp"

bool dejavu::UNKNOWN(const cv::Rect &r1, const cv::Rect &r2) {
    return true;
}

bool dejavu::SIDEWAYS_LEFT(const cv::Rect &r1, const cv::Rect &r2) {
    Change sample(r1, r2);
    int w = sample[0];
    int h = sample[1];
    int u = sample[2];
    int v = sample[3];
    return (abs(w) < 50) && (abs(h) < 50) && (u > 0) && (2 * abs(v) < abs(u));
}

bool dejavu::SIDEWAYS_RIGHT(const cv::Rect &r1, const cv::Rect &r2) {
    Change sample(r1, r2);
    int w = sample[0];
    int h = sample[1];
    int u = sample[2];
    int v = sample[3];
    return (abs(w) < 50) && (abs(h) < 50) && (u < 0) && (2 * abs(v) < abs(u));
}

bool dejavu::TURNING_LEFT(const cv::Rect &r1, const cv::Rect &r2) {
    Change sample(r1, r2);
    int w = sample[0];
    int h = sample[1];
    int u = sample[2];
    int v = sample[3];
    return (abs(w) < 50) && (abs(h) < 50) && (u > 0) && (2 * abs(v) < abs(u));
}

bool dejavu::TURNING_RIGHT(const cv::Rect &r1, const cv::Rect &r2) {
    Change sample(r1, r2);
    int w = sample[0];
    int h = sample[1];
    int u = sample[2];
    //int v = sample[3];
    return (abs(w) < 50) && (abs(h) < 50) && (u <= 0); // && (2 * abs(v) <= abs(u));
}

dejavu::FORWARD::FORWARD(const Parameters &_p):
    p(_p)
{
    // Nothing to do.
}

bool dejavu::FORWARD::operator () (const cv::Rect &r1, const cv::Rect &r2) {
    Change sample(r1, r2);

    const Sighting &original = sample.sighting(0);
    size_t b = p.band(original[2]);
    size_t m = p.bands() / 2;

    int w = sample[0];
    int u = sample[2];

    return (w >= 0) && ((b == m) || (b < m) == (u < 0));
}

dejavu::BACKWARD::BACKWARD(const Parameters &_p):
    p(_p)
{
    // Nothing to do.
}

bool dejavu::BACKWARD::operator () (const cv::Rect &r1, const cv::Rect &r2) {
    Change sample(r1, r2);

    const Sighting &original = sample.sighting(0);
    size_t b = p.band(original[2]);
    size_t m = p.bands() / 2;

    int w = sample[0];
    int u = sample[2];

    return (w <= 0) && ((b == m) || (b < m) != (u < 0));
}
