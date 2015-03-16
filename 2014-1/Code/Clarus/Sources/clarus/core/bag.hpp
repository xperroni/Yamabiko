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

#ifndef CLARUS_CORE_BAG_HPP
#define CLARUS_CORE_BAG_HPP

#include <boost/shared_ptr.hpp>

#include <map>
#include <stdexcept>
#include <string>

namespace clarus {
    class Bag;
}

class clarus::Bag {
    struct Entry {
        typedef boost::shared_ptr<Entry> P;

        /*
        Virtual destructor. Enforces polymorphism. Do not remove.
        */
        virtual ~Entry() {
            // Do nothing.
        }

        /*
        Returns a void pointer to the value encapsulated in this entry.
        */
        virtual const void *get() const = 0;
    };

    template<class T> struct Record: public Entry {
        boost::shared_ptr<T> value;

        Record(T *pointer)
        {
            value.reset(pointer);
        }

        Record(boost::shared_ptr<T> _value):
            value(_value)
        {
            // Nothing to do.
        }

        virtual const void *get() const {
            return value.get();
        }
    };

    std::map<std::string, Entry::P> entries;

public:
    /*
    Default constructor.
    */
    Bag();

    /*
    Virtual destructor. Enforces polymorphism. Do not remove.
    */
    virtual ~Bag();

    bool has(const std::string &name) const;

    /*
    Returns a reference to the value stored by the given name. If the name is not known,
    throws an exception.

    Be aware that this method is not thread-safe. If the record is overwritten or deleted
    by another thread, the reference returned by this method will become invalid, likely
    resulting in an application crash.
    */
    template<class T> T &fetch(const std::string &name);

    /*
    Returns a reference to the value stored by the given name. If the name is not known,
    associate it to the fall-back value, the return it.

    Be aware that this method is not thread-safe. If the record is overwritten or deleted
    by another thread, the reference returned by this method will become invalid, likely
    resulting in an application crash.
    */
    template<class T> T &fetch(const std::string &name, const T &fallback);

    template<class T> boost::shared_ptr<T> get(const std::string &name);

    template<class T> boost::shared_ptr<T> get(const std::string &name, const T &fallback);

    template<class T> boost::shared_ptr<T> set(const std::string &name, const T &value);

    template<class T> boost::shared_ptr<T> set(const std::string &name, T *value);

    template<class T> boost::shared_ptr<T> set(const std::string &name, boost::shared_ptr<T> value);
};

template<class T> T &clarus::Bag::fetch(const std::string &name) {
    return *(get<T>(name));
}

template<class T> T &clarus::Bag::fetch(const std::string &name, const T &fallback) {
    return *(get<T>(name, fallback));
}


template<class T> boost::shared_ptr<T> clarus::Bag::get(const std::string &name) {
    Record<T> *record = dynamic_cast<Record<T>*>(entries[name].get());
    if (record == NULL) {
        throw std::runtime_error(std::string("Record \"") + name + "\" not found");
    }

    return record->value;
}

template<class T> boost::shared_ptr<T> clarus::Bag::get(const std::string &name, const T &fallback) {
    return (has(name) ? get<T>(name) : set<T>(name, fallback));
}

template<class T> boost::shared_ptr<T> clarus::Bag::set(const std::string &name, const T &value) {
    return set<T>(name, new T(value));
}

template<class T> boost::shared_ptr<T> clarus::Bag::set(const std::string &name, T *value) {
    Record<T> *record = new Record<T>(value);
    entries[name].reset(record);
    return record->value;
}

template<class T> boost::shared_ptr<T> clarus::Bag::set(const std::string &name, boost::shared_ptr<T> value) {
    Record<T> *record = new Record<T>(value);
    entries[name].reset(record);
    return record->value;
}

#endif
