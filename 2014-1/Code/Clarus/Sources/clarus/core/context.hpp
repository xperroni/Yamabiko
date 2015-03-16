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

#ifndef CLARUS_CORE_LOCKED_BAG_HPP
#define CLARUS_CORE_LOCKED_BAG_HPP

#include <clarus/core/locker.hpp>
#include <clarus/core/bag.hpp>

#include <string>

namespace clarus {
    class Context;
}

class clarus::Context {
    Context *parent;

    Bag bag;

protected:
    Locker locker;

public:
    /*
    Default constructor.
    */
    Context();

    /*
    Creates a new context tied to the given parent.
    */
    Context(Context *parent);

    /*
    Virtual destructor. Enforces polymorphism. Do not remove.
    */
    virtual ~Context();

    bool has(const std::string &name);

    template<class T> T fetch(const std::string &name);

    template<class T> T fetch(const std::string &name, const T &fallback);

    template<class T> boost::shared_ptr<T> get(const std::string &name);

    template<class T> boost::shared_ptr<T> get(const std::string &name, const T &fallback);

    template<class T> boost::shared_ptr<T> set(const std::string &name, const T &value);

    template<class T> boost::shared_ptr<T> set(const std::string &name, T *value);

    template<class T> boost::shared_ptr<T> set(const std::string &name, boost::shared_ptr<T> value);

    template<class T> locked_ptr<T> read(const std::string &name) {
        return locker.read(get<T>(name));
    }

    template<class T> locked_ptr<T> write(const std::string &name) {
        return locker.write(get<T>(name));
    }
};

template<class T> T clarus::Context::fetch(const std::string &name) {
    boost::shared_ptr<T> value = get<T>(name);
    return *value;
}

template<class T> T clarus::Context::fetch(const std::string &name, const T &fallback) {
    boost::shared_ptr<T> value = get<T>(name, fallback);
    return *value;
}

template<class T> boost::shared_ptr<T> clarus::Context::get(const std::string &name) {
    if (parent != NULL && parent->has(name)) {
        set<T>(name, parent->get<T>(name));
    }

    locked_ptr<Bag> locked = locker.read(bag);
    return locked->get<T>(name);
}

template<class T> boost::shared_ptr<T> clarus::Context::get(const std::string &name, const T &fallback) {
    if (parent != NULL && parent->has(name)) {
        set<T>(name, parent->get<T>(name));
    }

    locked_ptr<Bag> locked = locker.read(bag);
    return locked->get<T>(name, fallback);
}

template<class T> boost::shared_ptr<T> clarus::Context::set(const std::string &name, const T &value) {
    return locker.write(bag)->set<T>(name, value);
}

template<class T> boost::shared_ptr<T> clarus::Context::set(const std::string &name, T *value) {
    return locker.write(bag)->set<T>(name, value);
}

template<class T> boost::shared_ptr<T> clarus::Context::set(const std::string &name, boost::shared_ptr<T> value) {
    return locker.write(bag)->set<T>(name, value);
}

#endif
