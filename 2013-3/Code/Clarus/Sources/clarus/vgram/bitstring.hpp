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

#ifndef CLARUS_VGRAM_BIT_STRING_HPP
#define CLARUS_VGRAM_BIT_STRING_HPP

#include <boost/shared_ptr.hpp>
#include <opencv2/opencv.hpp>

#include <bitset>

namespace vgram {
    template<size_t r> class bitstring;

    template<class B, class T> size_t minchinton(B &bits, size_t offset, const cv::Mat &input);
}

template<size_t r> class vgram::bitstring {
    typedef std::bitset<r> buffer;

    boost::shared_ptr<buffer> bits;

public:
    typedef typename buffer::reference reference;

    bitstring();

    bool operator < (const bitstring &b) const;

    bool operator [] (size_t pos) const;

    reference operator [] (size_t pos);

    bitstring operator ^ (const bitstring &b) const;

    bitstring clone() const;

    size_t count() const;

    size_t distance(const bitstring& b) const;

    bitstring& set(size_t pos, bool val = true);

    size_t size() const;
};

template<size_t r> vgram::bitstring<r>::bitstring():
    bits(new buffer())
{
    // Nothing to do.
}

template<size_t r> bool vgram::bitstring<r>::operator < (const bitstring<r> &b) const {
    buffer &b1 = *bits;
    buffer &b2 = *(b.bits);

    if (b1.count() < b2.count()) {
        return true;
    }

    if (b1.count() != b2.count()) {
        return false;
    }

    if (b1 == b2) {
        return false;
    }

    for (size_t i = 0; i < r; i++) {
        if (b1[i] != b2[i]) {
            return !b1[i] && b2[i];
        }
    }

    return false;
}

template<size_t r> bool vgram::bitstring<r>::operator [] (size_t pos) const {
    return (*bits)[pos];
}

template<size_t r> typename vgram::bitstring<r>::reference vgram::bitstring<r>::operator [] (size_t pos) {
    return (*bits)[pos];
}

template<size_t r> vgram::bitstring<r> vgram::bitstring<r>::operator ^ (const bitstring &b) const {
    bitstring<r> c;
    *(c.bits) = *bits ^ *(b.bits);
    return c;
}

template<size_t r> size_t vgram::bitstring<r>::count() const {
    return bits->count();
}

template<size_t r> vgram::bitstring<r> vgram::bitstring<r>::clone() const {
    bitstring<r> b;
    *(b.bits) = *bits;
    return b;
}

template<size_t r> size_t vgram::bitstring<r>::distance(const bitstring<r>& b) const {
    return (*bits ^ *(b.bits)).count();
}

template<size_t r> vgram::bitstring<r>& vgram::bitstring<r>::set(size_t pos, bool val) {
    bits->set(pos, val);
    return *this;
}

template<size_t r> size_t vgram::bitstring<r>::size() const {
    return bits->size();
}

template<class B, class T> size_t vgram::minchinton(B &bits, size_t offset, const cv::Mat &input) {
    size_t size = input.total();
    size_t last = size - 1;
    for (size_t k = 0; k < last; k++) {
        T a = input.at<T>(k);
        T b = input.at<T>(k + 1);
        bits.set(offset + k, a > b);
    }

    bits.set(offset + last, input.at<T>(last) > input.at<T>(0));
    return offset + size;
}

#endif
