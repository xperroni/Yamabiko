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

#include <clarus/core/domain.hpp>

#include <boost/shared_ptr.hpp>
#include <opencv2/opencv.hpp>

#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <vector>

namespace clarus {
    template<class T> class List;

    template<class T> size_t argmin(const List<T> &list);

    template<class T, class F> void sort(List<T> &list);

    template<class T, class F> void sort(List<T> &list, F cmp);

    template<class T> List<T> sorted(const List<T> &list);

    template<class T, class F> List<T> sorted(const List<T> &list, F cmp);
}

template<class T> class clarus::List {
    typedef std::vector<T> Buffer;

    boost::shared_ptr<Buffer> buffer;

public:
    List();

    List(size_t length, const T &value = T());

    List(const Buffer &that);

    /*
    Virtual destructor. Enforces polymorphism. Do not remove.
    */
    virtual ~List();

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

    T &insert(int index, const T &value = T());

    void extend(const List &that);

    void clear();

    bool contains(const T &value);

    T remove(int index);

    T &first();

    const T &first() const;

    T &last();

    const T &last() const;

    size_t size() const;
};

#include <clarus/core/list_iterator.hpp>
#include <clarus/core/list_iterator_const.hpp>

template<class T> clarus::List<T>::List():
    buffer(new Buffer())
{
    // Nothing to do.
}

template<class T> clarus::List<T>::List(size_t length, const T &value):
    buffer(new Buffer(length, value))
{
    // Nothing to do.
}

template<class T> clarus::List<T>::List(const Buffer &that):
    buffer(new Buffer(that))
{
    // Nothing to do.
}

template<class T> clarus::List<T>::~List() {
    // Nothing to do.
}

template<class T> clarus::List<T> clarus::List<T>::clone() const {
    return List(*buffer);
}

template<class T> T &clarus::List<T>::operator [] (int index) {
    return (*buffer)[index];
}

template<class T> const T &clarus::List<T>::operator [] (int index) const {
    return (*buffer)[index];
}

template<class T> bool clarus::List<T>::operator == (const List &that) const {
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

template<class T> bool clarus::List<T>::operator != (const List &that) const {
    return !(*this == that);
}

template<class T> bool clarus::List<T>::operator < (const List &that) const {
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

template<class T> typename clarus::List<T>::Buffer& clarus::List<T>::operator * () {
    return *buffer;
}

template<class T> const typename clarus::List<T>::Buffer& clarus::List<T>::operator * () const {
    return *buffer;
}

template<class T> typename clarus::List<T>::Buffer *clarus::List<T>::operator -> () {
    return buffer.get();
}

template<class T> const typename clarus::List<T>::Buffer *clarus::List<T>::operator -> () const {
    return buffer.get();
}

template<class T> T &clarus::List<T>::at(int index) {
    return buffer->at(index);
}

template<class T> const T &clarus::List<T>::at(int index) const {
    return buffer->at(index);
}

template<class T> T &clarus::List<T>::append(const T &value) {
    buffer->push_back(value);
    return buffer->back();
}

template<class T> T &clarus::List<T>::insert(int index, const T &value) {
    if (index < 0 || index >= size()) {
        throw std::runtime_error("Invalid index: " + domain::to_string(index));
    }

    typename std::vector<T>::iterator i = buffer->begin();
    for (int j = 0; j < index; ++j) {
        ++i;
    }

    buffer->insert(i, value);

    return at(index);
}

template<class T> void clarus::List<T>::extend(const List &that) {
    for (ListIteratorConst<T> i(that); i.more();) {
        append(i.next());
    }
}

template<class T> void clarus::List<T>::clear() {
    buffer->clear();
}

template<class T> bool clarus::List<T>::contains(const T &value) {
    for (ListIteratorConst<T> i(*this); i.more();) {
        if (i.next() == value) {
            return true;
        }
    }

    return false;
}

template<class T> T clarus::List<T>::remove(int index) {
    if (index < 0 || index >= size()) {
        throw std::runtime_error("Invalid index: " + domain::to_string(index));
    }

    typename std::vector<T>::iterator i = buffer->begin();
    for (int j = 0; j < index; ++j) {
        ++i;
    }

    T value = *i;
    buffer->erase(i);
    return value;
}

template<class T> T &clarus::List<T>::first() {
    return buffer->front();
}

template<class T> const T &clarus::List<T>::first() const {
    return buffer->front();
}

template<class T> T &clarus::List<T>::last() {
    return buffer->back();
}

template<class T> const T &clarus::List<T>::last() const {
    return buffer->back();
}

template<class T> size_t clarus::List<T>::size() const {
    return buffer->size();
}

template<class T> size_t clarus::argmin(const List<T> &list) {
    T least = list[0];
    size_t w = 0;
    for (size_t i = 1, n = list.size(); i < n; i++) {
        const T &value = list[i];
        if (value < least) {
            least = value;
            w = i;
        }
    }

    return w;
}

template<class T> void clarus::sort(List<T> &list) {
    std::vector<T> &v = *list;
    std::sort(v.begin(), v.end());
}

template<class T, class F> void clarus::sort(List<T> &list, F cmp) {
    std::vector<T> &v = *list;
    std::sort(v.begin(), v.end(), cmp);
}

template<class T> clarus::List<T> clarus::sorted(const List<T> &list) {
    List<T> cloned = list.clone();
    sort(cloned);
    return cloned;
}

template<class T, class F> clarus::List<T> clarus::sorted(const List<T> &list, F cmp) {
    List<T> cloned = list.clone();
    sort(cloned, cmp);
    return cloned;
}

template<class T> std::ostream &operator << (std::ostream &out, const clarus::List<T> &list) {
    if (list.size() == 0) {
        out << "[]";
        return out;
    }

    out << "[";
    for (clarus::ListIteratorConst<T> i(list);;) {
        out << i.next();
        if (!i.more()) {
            break;
        }

        out << ", ";
    }

    out << "]";

    return out;
}

#endif
