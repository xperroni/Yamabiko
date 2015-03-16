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

#ifndef CLARUS_CORE_LIST_ITERATOR_HPP
#define CLARUS_CORE_LIST_ITERATOR_HPP

namespace clarus {
    template<class T> class ListIterator;
}

template<class T> class clarus::ListIterator
{
    List<T> list;

    typename std::vector<T>::iterator a;

    typename std::vector<T>::iterator i;

    typename std::vector<T>::iterator n;

public:
    ListIterator();

    ListIterator(List<T> &list);

    bool operator == (const ListIterator &that) const;

    bool operator != (const ListIterator &that) const;

    T &operator * ();

    T *operator -> ();

    void erase();

    bool first() const;

    bool more() const;

    T &current();

    T &next();

    T &previous();
};

template<class T> clarus::ListIterator<T>::ListIterator() {
    // Nothing to do.
}

template<class T> clarus::ListIterator<T>::ListIterator(List<T> &_list):
    list(_list),
    a(list->begin()),
    i(list->begin()),
    n(list->end())
{
    // Nothing to do.
}

template<class T> bool clarus::ListIterator<T>::operator == (const ListIterator &that) const {
    return (this->i == that.i);
}

template<class T> bool clarus::ListIterator<T>::operator != (const ListIterator &that) const {
    return (this->i != that.i);
}

template<class T> T &clarus::ListIterator<T>::operator * () {
    return *i;
}

template<class T> T *clarus::ListIterator<T>::operator -> () {
    return i.operator->();
}

template<class T> void clarus::ListIterator<T>::erase() {
    list->erase(i);
}

template<class T> bool clarus::ListIterator<T>::first() const {
    return (i == a);
}

template<class T> bool clarus::ListIterator<T>::more() const {
    return (i != n);
}

template<class T> T &clarus::ListIterator<T>::current() {
    return *i;
}

template<class T> T &clarus::ListIterator<T>::next() {
    T &value = *i;
    ++i;
    return value;
}

template<class T> T &clarus::ListIterator<T>::previous() {
    return *(--i);
}

#endif
