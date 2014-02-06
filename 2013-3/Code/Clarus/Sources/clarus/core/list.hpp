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

#ifndef CLARUS_CORE_LIST_HPP
#define CLARUS_CORE_LIST_HPP

#include <boost/shared_ptr.hpp>
#include <opencv2/opencv.hpp>

#include <cstddef>
#include <vector>

template<class T> class List {
    typedef std::vector<T> Buffer;

    boost::shared_ptr<Buffer> buffer;

public:
    List();

    List(int length);

    List(const Buffer &that);

    List clone() const;

    T &operator [] (int index);

    const T &operator [] (int index) const;

    bool operator == (const List &that) const;

    bool operator != (const List &that) const;

    bool operator < (const List &that) const;

    Buffer &operator * ();

    const Buffer &operator * () const;

    Buffer *operator -> ();

    const Buffer *operator -> () const;

    T &at(int index);

    const T &at(int index) const;

    T &append(const T &value = T());

    void clear();

    bool contains(const T &value);

    T &first();

    const T &first() const;

    T &last();

    const T &last() const;

    size_t size() const;
};

#include <clarus/core/list_iterator.hpp>
#include <clarus/core/list_iterator_const.hpp>

template<class T> List<T>::List():
    buffer(new Buffer())
{
    // Nothing to do.
}

template<class T> List<T>::List(int length):
    buffer(new Buffer(length))
{
    // Nothing to do.
}

template<class T> List<T>::List(const Buffer &that):
    buffer(new Buffer(that))
{
    // Nothing to do.
}

template<class T> List<T> List<T>::clone() const {
    return List(buffer);
}

template<class T> T &List<T>::operator [] (int index) {
    return (*buffer)[index];
}

template<class T> const T &List<T>::operator [] (int index) const {
    return (*buffer)[index];
}

template<class T> bool List<T>::operator == (const List &that) const {
    if (size() != that.size()) {
        return false;
    }

    for (ListIteratorConst<T> i(*this), j(that); i.more();) {
        const T &a = i.next();
        const T &b = j.next();
        if (a != b) {
            return false;
        }
    }

    return true;
}

template<class T> bool List<T>::operator != (const List &that) const {
    return !(*this == that);
}

template<class T> bool List<T>::operator < (const List &that) const {
    for (ListIteratorConst<T> i(*this), j(that); i.more() && j.more();) {
        const T &a = i.next();
        const T &b = j.next();
        if (a < b) {
            return true;
        }
        else if (b < a) {
            return false;
        }
    }

    return (size() < that.size());
}

template<class T> typename List<T>::Buffer& List<T>::operator * () {
    return *buffer;
}

template<class T> const typename List<T>::Buffer& List<T>::operator * () const {
    return *buffer;
}

template<class T> typename List<T>::Buffer *List<T>::operator -> () {
    return buffer.get();
}

template<class T> const typename List<T>::Buffer *List<T>::operator -> () const {
    return buffer.get();
}

template<class T> T &List<T>::at(int index) {
    return buffer->at(index);
}

template<class T> const T &List<T>::at(int index) const {
    return buffer->at(index);
}

template<class T> T &List<T>::append(const T &value) {
    buffer->push_back(value);
    return buffer->back();
}

template<class T> void List<T>::clear() {
    buffer->clear();
}

template<class T> bool List<T>::contains(const T &value) {
    for (ListIteratorConst<T> i(*this); i.more();) {
        if (i.next() == value) {
            return true;
        }
    }

    return false;
}

template<class T> T &List<T>::first() {
    return buffer->front();
}

template<class T> const T &List<T>::first() const {
    return buffer->front();
}

template<class T> T &List<T>::last() {
    return buffer->back();
}

template<class T> const T &List<T>::last() const {
    return buffer->back();
}

template<class T> size_t List<T>::size() const {
    return buffer->size();
}

#endif
