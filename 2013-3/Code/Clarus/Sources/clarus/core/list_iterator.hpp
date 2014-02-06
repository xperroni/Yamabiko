#ifndef CLARUS_CORE_LIST_ITERATOR_HPP
#define CLARUS_CORE_LIST_ITERATOR_HPP

template<class T> class ListIterator
{
    List<T> list;

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

    bool more() const;

    T &current();

    T &next();
};

template<class T> ListIterator<T>::ListIterator() {
    // Nothing to do.
}

template<class T> ListIterator<T>::ListIterator(List<T> &_list):
    list(_list),
    i(list->begin()),
    n(list->end())
{
    // Nothing to do.
}

template<class T> bool ListIterator<T>::operator == (const ListIterator &that) const {
    return (this->i == that.i);
}

template<class T> bool ListIterator<T>::operator != (const ListIterator &that) const {
    return (this->i != that.i);
}

template<class T> T &ListIterator<T>::operator * () {
    return *i;
}

template<class T> T *ListIterator<T>::operator -> () {
    return i.operator->();
}

template<class T> void ListIterator<T>::erase() {
    list->erase(i);
}

template<class T> bool ListIterator<T>::more() const {
    return (i != n);
}

template<class T> T &ListIterator<T>::current() {
    return *i;
}

template<class T> T &ListIterator<T>::next() {
    T &value = *i;
    i++;
    return value;
}

#endif
